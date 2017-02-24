#include "fast_combine.h"

VALUE rb_mFastCombine;

static VALUE method_combine(VALUE self, VALUE array, VALUE mappings);

static VALUE combine(VALUE array, VALUE mappings);
static VALUE group_nodes(VALUE nodes, VALUE mappings);
static VALUE group_candidates(VALUE candidates, VALUE mapping);
static char is_array_of_hashes(VALUE array);
static VALUE group_by_value(VALUE element, VALUE key, int argc, VALUE* argv);
static VALUE group_by_values(VALUE element, VALUE keys, int argc, VALUE* argv);
static VALUE hash_values_at(VALUE element, VALUE keys);

static VALUE combine_element(VALUE element, VALUE candidates, VALUE mappings);
static VALUE find_candidates_for_element(VALUE element, VALUE candidates, VALUE keys);
static VALUE compose_candidates_key(VALUE element, VALUE keys);

static void assert_value(char);

void Init_fast_combine(void)
{
  rb_mFastCombine = rb_define_module("FastCombine");
  rb_define_singleton_method(rb_mFastCombine, "combine", method_combine, 2);
}

static VALUE method_combine(VALUE self, VALUE array, VALUE mappings) {
  return combine(array, mappings);
}

static VALUE combine(VALUE array, VALUE mappings)
{
  VALUE root = rb_ary_entry(array, 0),
    nodes = rb_ary_entry(array, 1);

  if(NIL_P(nodes) || RARRAY_LEN(nodes) == 0) return root;

  VALUE grouped_nodes = group_nodes(nodes, mappings);

  VALUE final_root = rb_ary_new2(RARRAY_LEN(root));
  for (long i = 0; i < RARRAY_LEN(root); i++) {
    VALUE element = RARRAY_AREF(root, i);
    VALUE new_element = combine_element(element, grouped_nodes, mappings);
    rb_ary_push(final_root, new_element);
  }
  return final_root;
}

static VALUE group_nodes(VALUE nodes, VALUE mappings) {
  VALUE grouped = rb_ary_new2(RARRAY_LEN(nodes));
  for (long i = 0; i < RARRAY_LEN(nodes); i++) {
    VALUE candidates = RARRAY_AREF(nodes, i),
      mapping = RARRAY_AREF(mappings, i);

    rb_ary_push(grouped, group_candidates(candidates, mapping));
  }
  return grouped;
}

static VALUE group_candidates(VALUE candidates, VALUE mapping) {
  VALUE keys = rb_ary_entry(mapping, 1),
    child_key = rb_funcall(keys, rb_intern("values"), 0);

  if(RARRAY_LEN(mapping) > 2) {
    candidates = combine(candidates, rb_ary_entry(mapping, 2));
  }

  if(RARRAY_LEN(child_key) == 1) {
    child_key = rb_ary_shift(child_key);
    return rb_block_call(candidates, rb_intern("group_by"), 0, NULL, group_by_value, child_key);
  } else {
    return rb_block_call(candidates, rb_intern("group_by"), 0, NULL, group_by_values, child_key);
  }
}

static char is_array_of_hashes(VALUE array) {
  return RARRAY_LEN(array) == 0 || TYPE(rb_ary_entry(array, 0)) == T_HASH;
}

static VALUE group_by_value(VALUE element, VALUE key, int argc, VALUE* argv) {
  return rb_hash_aref(element, key);
}

static VALUE group_by_values(VALUE element, VALUE keys, int argc, VALUE* argv) {
  return hash_values_at(element, keys);
}

static VALUE hash_values_at(VALUE element, VALUE keys) {
  VALUE result = rb_ary_new2(RARRAY_LEN(keys));
  for (long i=0; i < RARRAY_LEN(keys); i++) {
    rb_ary_push(result, rb_hash_aref(element, rb_ary_entry(keys, i)));
  }
  return result;
}

static VALUE combine_element(VALUE src, VALUE nodes, VALUE mappings) {
  VALUE element = rb_hash_dup(src);

  for (long i = 0; i < RARRAY_LEN(nodes); i++) {
    VALUE candidates = rb_ary_entry(nodes, i),
      mapping = rb_ary_entry(mappings, i),
      key = rb_ary_entry(mapping, 0),
      keys = rb_ary_entry(mapping, 1);

    assert_value(TYPE(candidates) != T_HASH);

    rb_hash_aset(element, key, find_candidates_for_element(element, candidates, keys));
  }

  return element;
}

static VALUE find_candidates_for_element(VALUE element, VALUE candidates, VALUE keys) {
  VALUE candidates_key = compose_candidates_key(element, keys),
    result = rb_hash_aref(candidates, candidates_key);

  return RTEST(result) ? result : rb_ary_new();
}

static VALUE compose_candidates_key(VALUE element, VALUE keys) {
  VALUE pk_name = rb_funcall(keys, rb_intern("keys"), 0);

  if(RARRAY_LEN(pk_name) == 1) {
    pk_name = rb_ary_shift(pk_name);
    return rb_hash_aref(element, pk_name);
  } else {
    return hash_values_at(element, pk_name);
  }
}

static void assert_value(char assertion) {
  if(assertion) rb_raise(rb_eArgError, "Unexpected value");
}
