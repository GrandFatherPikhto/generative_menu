#ifndef COMMON_H
#define COMMON_H

#define SET_BIT(a, pos) ((a) |= (1 << pos))
#define RESET_BIT(a, pos)((a) &= ~(1 << pos))
#define TOGGLE_BIT(a, pos)((a) ^= (1 << pos))
#define IS_BIT_SET(a, pos) (((a) & (1 << pos)) == (1 << pos))
#define IS_BIT_RESET(a, pos) (((a) & (1 << pos)) == 0)

#endif // COMMON_H