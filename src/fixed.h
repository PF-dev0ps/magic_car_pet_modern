#ifndef _FIXED_H_
#define _FIXED_H_
#include <cstdint>
#include <climits>

typedef int32_t fixed;

#define INT_TO_FIXED(i) (((fixed) (i)) << 8)
#define FIXED_TO_INT(f) ((f) >> 8)
#define FIXED_TO_FLOAT(f) (((double) (f)) / 256)
#define FLOAT_TO_FIXED(f) ((fixed) ((f) * 256.0))
#define FIXED_ABS(f) ((f) < 0 ? -(f) : (f))
#define FIXED_TRUNC(f) ((f) & 0xffff0000)
#define FIXED_SIGN(f) ((uint32_t) (f) >> 31)
#define FIXED_PRODUCT_SIGN(f, g) ((uint32_t) ((f) ^ (g)) >> 31)
#define FIXED_HALF(f) ((f) / 2)
#define FIXED_DOUBLE(f) ((f) << 1)
#define FIXED_SCALE(f, i) ((f) * ((fixed) (i)))
#define FIXED_ZERO     (INT_TO_FIXED(0))
#define FIXED_ONE      (INT_TO_FIXED(1))
#define FIXED_ONE_HALF (FIXED_HALF(FIXED_ONE))
#define FIXED_PI       (FLOAT_TO_FIXED(3.14159265))
#define FIXED_2PI      (FLOAT_TO_FIXED(6.28318531))
#define FIXED_HALF_PI  (FLOAT_TO_FIXED(1.57079633))
#define FIXED_MIN      INT32_MIN
#define FIXED_MAX      INT32_MAX
#define FIXED_EPSILON  ((fixed) 0x100)
#define TO_FIX_2_30(f)   ((f) << 14)
#define FROM_FIX_2_30(f) ((f) >> 14)
#define TO_FIX_8_24(f)   ((f) << 8)
#define FROM_FIX_8_24(f) ((f) >> 8)

static inline fixed fixmul(fixed r1, fixed r2) {
    return (fixed)(((int64_t)r1 * (int64_t)r2) >> 8);
}
static inline fixed fixmul2_30(fixed r1, fixed r2) {
    return (fixed)(((int64_t)r1 * (int64_t)r2) >> 30);
}
static inline fixed fixdiv(fixed dividend, fixed divisor) {
    return divisor ? (fixed)(((int64_t)dividend << 8) / divisor) : 0;
}
#endif
