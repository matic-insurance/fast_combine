#include "fast_combine.h"

#define FASTCOMBINE_MAP(ary, collect, iterator) do {  \
  long i; \
  collect = rb_ary_new2(RARRAY_LEN(ary)); \
  for (i = 0; i < RARRAY_LEN(ary); i++) { \
    VALUE element = RARRAY_AREF(ary, i); \
    VALUE new_element; \
    iterator; \
    rb_ary_push(collect, new_element);              \
  } \
}while(0)

VALUE rb_mFastCombine;

static VALUE fastcombine_method_combine(VALUE self, VALUE array, VALUE mappings);

void
Init_fast_combine(void)
{
  rb_mFastCombine = rb_define_module("FastCombine");
  rb_define_singleton_method(rb_mFastCombine, "combine", fastcombine_method_combine, 2);
}

static VALUE fastcombine_method_combine(VALUE self, VALUE array, VALUE mappings)
{
  VALUE root = rb_ary_entry(array, 0),
    groups = rb_ary_subseq(array, 1, RARRAY_LEN(array));

  VALUE to_return;
  FASTCOMBINE_MAP(root, to_return, {
      VALUE copy = rb_hash_dup(element);
      rb_hash_aset(copy, rb_str_new2("tasks"), rb_ary_new());
      new_element = copy;
  });
  return to_return;
}
