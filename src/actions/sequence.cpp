#include "ax12/AX12.hpp"
#include "motion/AbsoluteMotion.hpp"
#include "actions/paths.hpp"
#include "board.h"

int sequenceStep = 0;

void nextStepCallback() {
	sequenceStep++;
}

#define FIRSTSTEP if((step = 0) == sequenceStep)
#define STEP else if(++step == sequenceStep)
void sequenceUpdate() {
	int step;

	FIRSTSTEP {
		if(digitalRead(START_JACK) == LOW) {
			digitalWrite(GREEN_LED, LOW);
			digitalWrite(YELLOW_LED, HIGH);
			sequenceStep++;
		}
	} STEP {
		if(digitalRead(START_JACK) == HIGH) {
			digitalWrite(YELLOW_LED, LOW);
			digitalWrite(GREEN_LED, HIGH);
			motion->followPath(homologPath, nextStepCallback);
			sequenceStep++;
		}
	} STEP {} STEP {
		sequenceStep = 0;
	}
}
