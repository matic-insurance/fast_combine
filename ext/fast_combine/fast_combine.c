#include "fast_combine.h"

VALUE rb_mFastCombine;

static VALUE method_combine(VALUE self, VALUE array, VALUE mappings);
static VALUE combine_element(VALUE element, VALUE groups, VALUE mappings);

void
Init_fast_combine(void)
{
  rb_mFastCombine = rb_define_module("FastCombine");
  rb_define_singleton_method(rb_mFastCombine, "combine", method_combine, 2);
}

static VALUE method_combine(VALUE self, VALUE array, VALUE mappings)
{
  VALUE root = rb_ary_entry(array, 0),
    groups = rb_ary_subseq(array, 1, RARRAY_LEN(array));

  VALUE final_root = rb_ary_new2(RARRAY_LEN(root));
  for (long i = 0; i < RARRAY_LEN(root); i++) {
    VALUE element = RARRAY_AREF(root, i);
    VALUE new_element = combine_element(element, groups, mappings);
    rb_ary_push(final_root, new_element);
  }
  return final_root;
}

static VALUE combine_element(VALUE element, VALUE groups, VALUE mappings) {
  VALUE copy = rb_hash_dup(element);
  rb_hash_aset(copy, rb_str_new2("tasks"), rb_ary_new());
  return copy;
}
