#ifndef UTILS_HPP
#define UTILS_HPP

#define DEG_TO_RAD(x) (((x) * M_PI) / 180)
#define RAD_TO_DEG(x) (((x) * 180) / M_PI)

#define MM_TO_PX(x) (0.5f * (x))
#define PX_TO_MM(x)(2.f * (x))

#define SIGN(x) ((x) < 0 ? -1 : 1)
#define ABS(x) ((x) < 0 ? (-(x)) : (x))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

// Delta below which two floats are considered equal
#define EPSILON 0.2f

class Utils {
public:
	/**
	 * @brief Convert an angle (in degree) into [0:360]
	 */
	static float normalizeAngle(float angle);
};

#endif // UTILS_HPP
