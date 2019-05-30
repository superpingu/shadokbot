#include "ax12/AX12.hpp"
#include "actions/paths.hpp"
#include "actions/robot.hpp"
#include "display/SevenSegDisplay.h"
#include "board.h"

static int sequenceStep = 0;
static uint32_t startTime = 0;
static int iterationCounter = 0;

void nextStepCallback() {
	if(sequenceStep > 0)
		sequenceStep++;
}

#define FIRSTSTEP if((step = 0) == sequenceStep)
#define STEP else if(++step == sequenceStep)
void sequenceUpdate() {
	int step;

	// end of the match
	if(micros() - startTime > 99000000 && sequenceStep > 1) {
		motion->enable(false);
		sequenceStep = 0;
		digitalWrite(GREEN_LED, LOW);
	}

	FIRSTSTEP { // ready
		if(iterationCounter % 200 == 0) {
			display.show(getBatteryVoltage()/10);
		}

		if(digitalRead(START_JACK) == LOW) {
			digitalWrite(GREEN_LED, LOW);
			digitalWrite(YELLOW_LED, HIGH);

			initPosition(paletsPath);
			motion->enable(true);

			sequenceStep++;
		}
	} STEP { // jack removed
		if(digitalRead(START_JACK) == HIGH) {
			digitalWrite(YELLOW_LED, LOW);
			digitalWrite(GREEN_LED, HIGH);

			sampleInputs();

			startTime = micros();
			followPath(paletsPath, nextStepCallback);
			sequenceStep++;
		}
	} STEP {} STEP { // palet finished
		deployArm(NULL);
		followPath(acceleratorPath, nextStepCallback);
		sequenceStep++;
	} STEP {} STEP { // accelerator finished
		deployArm(NULL);
		followPath(goldPath, nextStepCallback);
		sequenceStep++;
	} STEP {} STEP { // first move finished
		digitalWrite(GREEN_LED, LOW);
		sequenceStep = 0;
	}

	iterationCounter++;
}
