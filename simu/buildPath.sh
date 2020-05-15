#!/bin/sh

OUTPUT_FILE="src/actions/paths.hpp"

if [ $# != 2 ]; then
	echo "Usage: $0 <inputFileYellow> <inputFilePurple>"
	exit 1
fi

rm -f ${OUTPUT_FILE}
touch ${OUTPUT_FILE}

echo "#ifndef PATHS_HPP" >> ${OUTPUT_FILE}
echo "#define PATHS_HPP\n" >> ${OUTPUT_FILE}
echo "#include \"actions/path_type.h\"" >> ${OUTPUT_FILE}
echo "#include \"actions/robot.hpp\"\n" >> ${OUTPUT_FILE}

createSequenceElement () {
	INPUT_FILE=$1
	COLOR=$2

	MOTION_ID=0
	IN_MOTION=0

	while read -r LINE; do
		TOKEN_NB=$(echo ${LINE} | wc -w)
		if [ ${TOKEN_NB} = 5 ]; then
			if [ ${IN_MOTION} = 0 ]; then
				IN_MOTION=1
				echo "static const MotionElement path${COLOR}${MOTION_ID}[] = {" >> ${OUTPUT_FILE}
				MOTION_ID=$((MOTION_ID+1))
			fi
			POS_X=$(echo ${LINE} | cut -d' ' -f 1)
			POS_Y=$(echo ${LINE} | cut -d' ' -f 2)
			ANGLE=$(echo ${LINE} | cut -d' ' -f 3)
			SPEED=$(echo ${LINE} | cut -d' ' -f 4)
			MOVE_TYPE=$(echo ${LINE} | cut -d' ' -f 5)
			echo "	{${POS_X}, ${POS_Y}, ${ANGLE}, ${SPEED}, ${MOVE_TYPE}}," >> ${OUTPUT_FILE}
		fi
		if [ ${TOKEN_NB} = 1 ]; then
			if [ ${IN_MOTION} = 1 ]; then
				echo "	END_PATH" >> ${OUTPUT_FILE}
				echo "};" >> ${OUTPUT_FILE}
				IN_MOTION=0
			fi
		fi
	done < ${INPUT_FILE}

	if [ ${IN_MOTION} = 1 ]; then
		echo "	END_PATH" >> ${OUTPUT_FILE}
		echo "};" >> ${OUTPUT_FILE}
		IN_MOTION=0
	fi

	MOTION_ID=0
	echo "static const SequenceElement path${COLOR}[] = {" >> ${OUTPUT_FILE}
	while read -r LINE; do
		TOKEN_NB=$(echo ${LINE} | wc -w)
		if [ ${TOKEN_NB} = 5 ]; then
			if [ ${IN_MOTION} = 0 ]; then
				echo "	{SEQUENCE_ELEMENT_TYPE_MOTION_ELEMENT, {.motion=path${COLOR}${MOTION_ID}}}," >> ${OUTPUT_FILE}
				MOTION_ID=$((MOTION_ID+1))
				IN_MOTION=1
			fi
		fi
		if [ ${TOKEN_NB} = 1 ]; then
			IN_MOTION=0
			echo "	{SEQUENCE_ELEMENT_TYPE_ACTION, {.cb=${LINE}}}," >> ${OUTPUT_FILE}
		fi
	done < ${INPUT_FILE}
	echo "};\n" >> ${OUTPUT_FILE}
}

createSequenceElement $1 "Yellow"
createSequenceElement $2 "Purple"

echo "static const SequenceElement* const paths[] = {pathYellow, pathPurple};\n" >> ${OUTPUT_FILE}

echo "#endif" >> ${OUTPUT_FILE}
