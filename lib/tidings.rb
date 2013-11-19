case RUBY_PLATFORM
when /darwin/
  require 'fs_event'
when /linux/
  require 'inotify'
else
  raise RuntimeError, "Your platform is currently not supported (#{RUBY_PLATFORM})"
end

module Tidings
  autoload :VERSION, 'tidings/version'
  autoload :Watcher, 'tidings/watcher'

  def self.log(message)
    $stderr.puts('Tidings: ' + message.to_s) if $DEBUG
  end

  def self.watch(path, processor=nil, &block)
    watcher = Tidings::Watcher.new(path, processor || block)
    watcher.start
    watcher
  end
end