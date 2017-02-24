#include "fast_combine.h"

VALUE rb_mFastCombine;

static VALUE method_combine(VALUE self, VALUE array, VALUE mappings);
static VALUE combine_element(VALUE element, VALUE groups, VALUE mappings);
static VALUE find_group_for_element(VALUE element, VALUE candidates, VALUE mapping);
static VALUE group_by_value(VALUE element, VALUE key, int argc, VALUE* argv);

void Init_fast_combine(void)
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

static VALUE combine_element(VALUE src, VALUE groups, VALUE mappings) {
  VALUE element = rb_hash_dup(src);

  for (long i = 0; i < RARRAY_LEN(groups); i++) {
    VALUE candidates = rb_ary_entry(groups, i),
      mapping = rb_ary_entry(mappings, i),
      key = rb_ary_entry(mapping, 0);

    rb_hash_aset(element, key, find_group_for_element(element, candidates, mapping));
  }

  return element;
}

static VALUE find_group_for_element(VALUE element, VALUE candidates, VALUE mapping) {
  VALUE key = rb_ary_entry(mapping, 0),
    keys = rb_ary_entry(mapping, 1),
    child_key = rb_ary_shift(rb_funcall(keys, rb_intern("values"), 0)),
    pk_name = rb_ary_shift(rb_funcall(keys, rb_intern("keys"), 0)),
    pkey_value = rb_hash_aref(element, pk_name),
    grouped = rb_block_call(candidates, rb_intern("group_by"), 0, NULL, group_by_value, child_key);
  return rb_hash_aref(grouped, pkey_value);
}

static VALUE group_by_value(VALUE element, VALUE key, int argc, VALUE* argv) {
  return rb_hash_aref(element, key);
}
