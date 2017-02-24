#include "fast_combine.h"

VALUE rb_mFastCombine;

void
Init_fast_combine(void)
{
  rb_mFastCombine = rb_define_module("FastCombine");
}
