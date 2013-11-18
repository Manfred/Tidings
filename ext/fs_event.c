#include <stdio.h>
#include <ruby.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>

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

  fprintf(stderr, "number of events: %zd\n", numberOfEvents);
  for(index=0; index < numberOfEvents; index++) {
    fprintf(stderr, "event path: %s\n", paths[index]);
  }
}

/*
 *  call-seq:
 *    tidings_watch(path, &block)
 *
 *  Calls the block with the changed paths when something changes in the
 *  specified path. This method blocks the thread forever.
 *
 *    FSEvent.watch('/tmp') { |paths| p paths }
 */
static VALUE fs_event_watch(VALUE self, VALUE path, VALUE processor)
{
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
    kFSEventStreamCreateFlagWatchRoot
  );

  FSEventStreamShow(streamRef);
  fprintf(stderr, "\n");

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