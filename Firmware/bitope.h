#ifndef BITOPE
#define BITOPE

#define _BV(n)                ((0x01) << (n))
#define bit_set(data, n)      (data) |= _BV(n)
#define bit_clear(data, n)    (data) &=~_BV(n)
#define is_bit_set(data, n)   (data) & _BV(n)
#define is_bit_clear(data, n) !is_bit_set(data,n)

#endif /* BITOPE */

