#include <stdio.h>
#include <ruby.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>

static VALUE processor;

VALUE fs_event_event_type(const FSEventStreamEventFlags eventFlags)
{
  VALUE event_flags = rb_ary_new();

  if (eventFlags & kFSEventStreamEventFlagMustScanSubDirs)
    rb_ary_push(event_flags, ID2SYM(rb_intern("must_scan")));
  if (eventFlags & kFSEventStreamEventFlagItemCreated)
    rb_ary_push(event_flags, ID2SYM(rb_intern("created")));
  if (eventFlags & kFSEventStreamEventFlagItemRemoved)
    rb_ary_push(event_flags, ID2SYM(rb_intern("removed")));
  if (eventFlags & kFSEventStreamEventFlagItemRenamed)
    rb_ary_push(event_flags, ID2SYM(rb_intern("renamed")));
  if (eventFlags & kFSEventStreamEventFlagItemModified)
    rb_ary_push(event_flags, ID2SYM(rb_intern("modified")));
  if (eventFlags & kFSEventStreamEventFlagItemIsFile)
    rb_ary_push(event_flags, ID2SYM(rb_intern("file")));
  if (eventFlags & kFSEventStreamEventFlagItemIsDir)
    rb_ary_push(event_flags, ID2SYM(rb_intern("dir")));

  return event_flags;
}

void fs_event_callback(
  ConstFSEventStreamRef streamRef,
  void *userData,
  size_t numberOfEvents,
  void *eventPaths,
  const FSEventStreamEventFlags eventFlags[],
  const FSEventStreamEventId eventIds[]
)
{
  char **paths = eventPaths;
  size_t index;

  for(index=0; index < numberOfEvents; index++) {
    rb_funcall(processor, rb_intern("call"), 2,
      rb_str_new2(paths[index]),
      fs_event_event_type(eventFlags[index])
    );
  }
}

/*
 *  call-seq:
 *    tidings_watch(path, processor)
 *
 *  Calls the block with the changed paths when something changes in the
 *  specified path. This method blocks the thread forever.
 *
 *    FSEvent.watch('/tmp', Proc.new { |files| p files })
 */
static VALUE fs_event_watch(VALUE self, VALUE path, VALUE proc)
{
  processor = proc;

  CFStringRef cfPaths[1];
  cfPaths[0] = CFStringCreateWithCString(
    kCFAllocatorSystemDefault,
    RSTRING_PTR(path),
    kCFStringEncodingUTF8
  );
  CFArrayRef paths = CFArrayCreate(kCFAllocatorSystemDefault, (const void **)cfPaths, 1, NULL);

  FSEventStreamRef streamRef = FSEventStreamCreate(
    NULL,
    &fs_event_callback,
    NULL,
    paths,
    kFSEventStreamEventIdSinceNow,
    (CFTimeInterval)0.5,
    kFSEventStreamCreateFlagWatchRoot | kFSEventStreamCreateFlagFileEvents
  );

#ifdef DEBUG
  FSEventStreamShow(streamRef);
#endif

  FSEventStreamScheduleWithRunLoop(streamRef, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
  FSEventStreamStart(streamRef);

  CFRunLoopRun();

  FSEventStreamFlushSync(streamRef);
  FSEventStreamStop(streamRef);

  return Qnil;
}

void Init_fs_event()
{
  VALUE mFSEvent;

  mFSEvent = rb_define_module("FSEvent");
  
  rb_define_module_function(mFSEvent, "watch", fs_event_watch, 2);
}