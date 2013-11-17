#include <stdio.h>
#include <ruby.h>
#include <ruby/encoding.h>

#ifdef __APPLE__

/*
 *  call-seq:
 *    tidings_watch(*paths, responder, &block)
 *
 *  Calls #call on the responder object if present and otherwise calls the
 *  block with the changed paths.
 *
 *    Tidings.watch('/tmp') { |paths| p paths }
 */
static VALUE tidings_watch(int argc, VALUE *argv, VALUE self)
{
  int index;

  for(index=0; index < argc; index++) {
  }

  return Qnil;
}

#endif

#ifdef __gnu_linux__
#endif

void Init_tidings()
{
  VALUE mTidings;

  mTidings = rb_define_module("Tidings");

  int utf8_enc_index = rb_enc_find_index("UTF-8");
  VALUE version = rb_str_new2("0.1.0");
  rb_enc_associate_index(version, utf8_enc_index);

  rb_define_const(mTidings, "VERSION", version);
  rb_define_module_function(mTidings, "watch", tidings_watch, -1);
}