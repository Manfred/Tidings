#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <ruby.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

VALUE inotify_event_flags(uint32_t mask)
{
  VALUE event_flags = rb_ary_new();

  if (mask & IN_ACCESS)
    rb_ary_push(event_flags, ID2SYM(rb_intern("accessed")));
  if (mask & IN_CREATE)
    rb_ary_push(event_flags, ID2SYM(rb_intern("created")));
  if (mask & IN_DELETE)
    rb_ary_push(event_flags, ID2SYM(rb_intern("removed")));
  if (mask & IN_MODIFY)
    rb_ary_push(event_flags, ID2SYM(rb_intern("modified")));
  if (mask & IN_ISDIR)
    rb_ary_push(event_flags, ID2SYM(rb_intern("dir")));
  else
    rb_ary_push(event_flags, ID2SYM(rb_intern("file")));

  return event_flags;
}

/*
 *  call-seq:
 *    inotify_watch(path, processor)
 *
 *  Calls the block with the changed paths when something changes in the
 *  specified path. This method blocks the thread forever.
 *
 *    Inotify.watch('/tmp', Proc.new { |files| p files })
 */
static VALUE inotify_watch(VALUE self, VALUE path, VALUE processor)
{
  int length, i;
  int notify;
  int watch;
  char buffer[BUF_LEN];

  fd_set rfds;
  struct timeval tv;
  int retval;

  notify = inotify_init();

  if (notify < 0) perror("inotify_init");

  watch = inotify_add_watch(notify, RSTRING_PTR(path), IN_MODIFY | IN_CREATE | IN_DELETE );

  for (;;)
  {
    FD_ZERO(&rfds);
    FD_SET(notify, &rfds);

    tv.tv_sec = 0;
    tv.tv_usec = 500;

    retval = select(notify + 1, &rfds, NULL, NULL, &tv);

    if (retval < 0) {
      perror("select()");
      return Qfalse;
    } else if (FD_ISSET(notify, &rfds)) {
      length = read(notify, buffer, BUF_LEN);
      if (length < 0) perror("read");
    
      i = 0;
      while(i < length) {
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
        if (event->len) {
          rb_funcall(processor, rb_intern("call"), 2,
            rb_str_new2(event->name),
            inotify_event_flags(event->mask)
          );
        }
        i += EVENT_SIZE + event->len;
      }
    }
  }

  inotify_rm_watch(notify, watch);
  close(notify);

  return Qnil;
}

void Init_inotify()
{
  VALUE mInotify;

  mInotify = rb_define_module("Inotify");

  rb_define_module_function(mInotify, "watch", inotify_watch, 2);
}