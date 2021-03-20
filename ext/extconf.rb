# frozen_string_literal: true

require 'mkmf'

case RUBY_PLATFORM
when /darwin/
  with_ldflags('-framework CoreServices') do
    create_makefile('fs_event', 'fs_event')
  end
when /linux/
  create_makefile('inotify', 'inotify')
end
