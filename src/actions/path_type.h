#ifndef PATH_TYPE_H
#define PATH_TYPE_H

#include "motion/AbsoluteMotion.hpp"

enum SequenceElementType {
	SEQUENCE_ELEMENT_TYPE_MOTION_ELEMENT,
	SEQUENCE_ELEMENT_TYPE_ACTION
};

union SequenceElementAction {
	const MotionElement* motion;
	void (*cb)(void(*callback)());
};

struct SequenceElement {
	SequenceElementType type;
	SequenceElementAction action;
};

#endif // PATH_TYPE_H
