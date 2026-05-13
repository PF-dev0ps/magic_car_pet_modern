#ifndef _FIXED2_H_
#define _FIXED2_H_
#include <cstdint>
typedef int32_t fixed16;
static inline fixed16 fixmul16(fixed16 r1, fixed16 r2) {
    return (fixed16)(((int64_t)r1 * (int64_t)r2) >> 16);
}
static inline fixed16 fixdiv16(fixed16 dividend, fixed16 divisor) {
    return divisor ? (fixed16)(((int64_t)dividend << 16) / divisor) : 0;
}
#endif
