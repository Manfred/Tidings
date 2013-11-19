#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <ruby.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

/*
 *  call-seq:
 *    inotify_watch(path, processor)
 *
 *  Calls the block with the changed paths when something changes in the
 *  specified path. This method blocks the thread forever.
 *
 *    Inotify.watch('/tmp', Proc.new { |files| p files })
 */
static VALUE inotify_watch(VALUE self, VALUE path, VALUE proc)
{
  int length, i;
  int notify;
  int watch;
  char buffer[BUF_LEN];

  notify = inotify_init();

  if (notify < 0) perror( "inotify_init" );

  watch = inotify_add_watch(notify, RSTRING_PTR(path), IN_MODIFY | IN_CREATE | IN_DELETE );

  length = read(notify, buffer, BUF_LEN);

  if (length < 0) perror( "read");

  i = 0;
  while(i < length) {
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
    if ( event->len ) {
      if ( event->mask & IN_CREATE ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "The directory %s was created.\n", event->name );       
        }
        else {
          printf( "The file %s was created.\n", event->name );
        }
      }
      else if ( event->mask & IN_DELETE ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "The directory %s was deleted.\n", event->name );       
        }
        else {
          printf( "The file %s was deleted.\n", event->name );
        }
      }
      else if ( event->mask & IN_MODIFY ) {
        if ( event->mask & IN_ISDIR ) {
          printf( "The directory %s was modified.\n", event->name );
        }
        else {
          printf( "The file %s was modified.\n", event->name );
        }
      }
    }
    i += EVENT_SIZE + event->len;
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