#include "Arduino.h"
#include "Motor.hpp"
#include "motionconf.h"

#define SIGN(a) (a > 0 ? 1 : -1)
#define ABS(a) ((a) > 0 ? (a) : -(a))

#define MOTOR_ACC_COEFF_FP 10 // fixed point position in accelerationCoeff

static void motorTimerCallback(void* motorInst) {
	Motor* motor = (Motor*) motorInst;

	if(motor->position < motor->goalPosition) {
		motor->setPulse((motor->position & 1) == 0); // toggle control signals
		motor->position++;
		motor->motorTimer->setPeriod(motor->period);
	} else {
		motor->motorTimer->stop();
	}
}

Motor::Motor(uint8_t _enablePin, uint8_t _dirPin, uint8_t _clockPin, bool _invDir) : enablePin(_enablePin), clockPin(_clockPin), dirPin(_dirPin), invDir(_invDir)
{
	goalPosition = 0;
	position = (uint32_t) -1;
	maxAcceleration = DEFAULT_MAX_ACCELERATION;
	minSpeed = DEFAULT_MIN_SPEED;
	recalSpeed = DEFAULT_RECAL_SPEED;
	recalDistance = DEFAULT_RECAL_DISTANCE;
	recalPhase = false;

	motorTimer = new Timer();
	motorTimer->setCallback(motorTimerCallback, this);

	pinMode(enablePin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(clockPin, OUTPUT);

	digitalWrite(enablePin, MOTOR_ACTIVE_LEVEL);
	digitalWrite(clockPin, MOTOR_IDLE_LEVEL);
}

void Motor::update() {
	// compute speed according to current position
	int32_t speed;

	if(recalPhase && position < goalPosition) { // recalibration phase
		speed = recalSpeed;
	} else if(position < accelerationDistance) { // acceleration phase
		speed = minSpeed + ((accelerationCoeff*position & 0xFFFFFC00) >> MOTOR_ACC_COEFF_FP);
	} else if(position < goalPosition - accelerationDistance) { // cruise phase (constant speed)
		speed = ABS(cruiseSpeed);
	} else if(position < goalPosition) { // deceleration phase
		speed = minSpeed + ((accelerationCoeff*(goalPosition-position) & 0xFFFFFC00) >> MOTOR_ACC_COEFF_FP);
		// recalibration move : constant speed until we hit the wall
		if(recalibration && speed <= recalSpeed) {
			speed = recalSpeed;
			recalPhase = true;
			goalPosition = goalPosition + 2*recalDistance;
		}
	} else { // goal position reached, stop
		speed = 0;
		recalPhase = false;
	}

	// update motor timer to adjust tick frequency to speed
	if(speed != 0)
		period = SPEEDTU_TO_S*1000000*US_TO_TIMER_PERIOD/speed;
}

void Motor::setPulse(bool active) {
	digitalWrite(clockPin, active ? MOTOR_ACTIVE_LEVEL : MOTOR_IDLE_LEVEL);
}

void Motor::move(int32_t speed, uint32_t distance, bool recal) {
	recalibration = recal;
	if(recal) // recalibration: slow down before reaching goal distance
		distance -= recalDistance;

	position = 0;
	cruiseSpeed = ABS(speed) < minSpeed ? minSpeed*SIGN(speed) : speed;
	// goalPosition is in half ticks (as position is incremented twice/tick when motor timer toggles output)
	goalPosition = distance & 0xFFFFFFFE;

	// compute the distance the motor runs while accelerating before reaching cruise speed
	accelerationDistance = (cruiseSpeed*cruiseSpeed - minSpeed*minSpeed)/(2*maxAcceleration);
	if(accelerationDistance > goalPosition/2) // small move where we don't reach target speed
		accelerationDistance = goalPosition/2;
	// pre-compute coeff used to compute speed from position during the move
	accelerationCoeff = accelerationDistance == 0 ? 0 :
		((ABS(cruiseSpeed) - minSpeed) << MOTOR_ACC_COEFF_FP)/accelerationDistance;

	// set initial motor timer period
	update();
	// start generating pulses to the stepper motor controller
	motorTimer->start();
	// set rotation direction
	digitalWrite(dirPin, (cruiseSpeed < 0) ^ invDir ? MOTOR_ACTIVE_LEVEL : MOTOR_IDLE_LEVEL);
}

void Motor::enable(bool enabled) {
	digitalWrite(enablePin, enabled ? MOTOR_IDLE_LEVEL : MOTOR_ACTIVE_LEVEL);
}
void Motor::stopRecal() {
	recalPhase = false;
	position = goalPosition;
}

bool Motor::finished() { return position >= goalPosition; }
bool Motor::recalibrating() { return recalPhase; }
