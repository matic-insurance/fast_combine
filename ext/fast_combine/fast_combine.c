#include "fast_combine.h"

VALUE rb_mFastCombine;

static VALUE method_combine(VALUE self, VALUE array, VALUE mappings)
{
  return rb_ary_new();
}

void
Init_fast_combine(void)
{
  rb_mFastCombine = rb_define_module("FastCombine");
  rb_define_singleton_method(rb_mFastCombine, "combine", method_combine, 2);
}
