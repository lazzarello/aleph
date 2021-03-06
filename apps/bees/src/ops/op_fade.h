#ifndef _op_fade_H_
#define _op_fade_H_

#include "types.h"
#include "op.h"
#include "op_math.h"

//--- op_fade_t
typedef struct op_fade_struct {
  op_t super;
  volatile io_t val; 
  volatile io_t a;
  volatile io_t b;
  volatile io_t x;
  volatile io_t * in_val[3]; // a, b, x
  op_out_t outs[1];
} op_fade_t;
void op_fade_init(void* mem);

#endif // header guard
