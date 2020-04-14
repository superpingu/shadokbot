#include "output.hpp"

Output::Output(std::fstream* file)
{
	mFile = file;
	if (mFile == NULL || !mFile->is_open()) {
		printf("Fail to open file\n");
	}
}

void Output::onEvent(Event* event)
{
	if (event->type == EVENT_CLOSE) {
		mFile->close();
		printf("File closed\n");
	} else if (event->type == EVENT_NEW_TARGET) {
		if (!mFile->is_open())
			return;

		mFile->seekp(0, std::ios::end);
		*mFile << event->targetEvent.x << " " << event->targetEvent.y << " " << event->targetEvent.angle << " 200 MOVE_TURN\n";
	}
}
