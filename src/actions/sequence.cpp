#include "ax12/AX12.hpp"
#include "actions/paths.hpp"
#include "actions/robot.hpp"
#include "display/SevenSegDisplay.h"
#include "board.h"

static int sequenceStep = 0;
static int actionStep = -1;
static uint32_t startTime = 0;
static int iterationCounter = 0;

void performNextAction() {
	actionStep++;
	switch (paths[getTeam()][actionStep].type) {
	case SEQUENCE_ELEMENT_TYPE_ACTION:
		paths[getTeam()][actionStep].action.cb(performNextAction);
		break;
	case SEQUENCE_ELEMENT_TYPE_MOTION_ELEMENT:
		motion->followPath(paths[getTeam()][actionStep].action.motion, performNextAction);
		break;
	}
}

void sequenceUpdate() {
	// end of the match
	if(micros() - startTime > 99000000 && sequenceStep > 1) {
		sequenceStep = 0;
		digitalWrite(GREEN_LED, LOW);
	}

	if (sequenceStep == 0) { // ready
		if(iterationCounter % 200 == 0) {
			display.show(getBatteryVoltage()/10);
		}

		if(digitalRead(START_JACK) == LOW) { // Jack inserted
			digitalWrite(GREEN_LED, LOW);
			digitalWrite(YELLOW_LED, HIGH);

			initPosition(paths[getTeam()][0].action.motion);

			sequenceStep++;
		}
	} else if (sequenceStep == 1) { // jack removed
		if(digitalRead(START_JACK) == HIGH) {
			digitalWrite(YELLOW_LED, LOW);
			digitalWrite(GREEN_LED, HIGH);

			sequenceStep++;
			sampleInputs();
			startTime = micros();
		}
	} else if (sequenceStep == 2) {
		performNextAction();
		sequenceStep++;
	}

	iterationCounter++;
}
