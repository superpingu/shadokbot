#include "ax12/AX12.hpp"
#include "motion/AbsoluteMotion.hpp"
#include "lidar/detection.hpp"
#include "actions/sequence.hpp"
#include "actions/robot.hpp"
#include "display/SevenSegDisplay.h"
#include "board.h"
#include "ranger/vl53l1x.hpp"

#include "shell/Shell.hpp"
#include "shell/commands.h"

#include "motion/TMC5130.hpp"

Shell* shell;
VL53L1X sensor;

// print shell invite, with battery voltage display
void onShellInvite() {
	int volts = getBatteryVoltage()/10;
	int integer_part = volts/100;
	int decimal_part =  volts - integer_part*100;
	Serial.print(integer_part);
	Serial.print('.');
	Serial.print(decimal_part);
	Serial.print("V > ");
}

// the setup function runs once when you press reset or power the board
void setup() {
	shell = new Shell(115200, getComms(), onShellInvite);

	Wire1.begin();
  Wire1.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  sensor.setDistanceMode(VL53L1X::Short);
  sensor.setMeasurementTimingBudget(50000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor.startContinuous(50);
}

#define LOOP_PERIOD_US 40000 // duration of each loop iteration
// the loop function runs over and over again forever
void loop() {
	unsigned long loopStart = micros();

	Serial.print(sensor.read());
	Serial.print('\n');
  if(sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

	unsigned long loopTime = micros() - loopStart;
	delayMicroseconds(loopTime > LOOP_PERIOD_US ? 0 : LOOP_PERIOD_US - loopTime);
}
