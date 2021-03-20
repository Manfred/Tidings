# frozen_string_literal: true

case RUBY_PLATFORM
when /darwin/
  require 'fs_event'
when /linux/
  require 'inotify'
else
  raise "Your platform is currently not supported (#{RUBY_PLATFORM})"
end

module Tidings
  autoload :VERSION, 'tidings/version'
  autoload :Watcher, 'tidings/watcher'

  def self.log(message)
    warn("Tidings: #{message}") if $DEBUG
  end

  def self.watch(path, processor = nil, &block)
    watcher = Tidings::Watcher.new(path, processor || block)
    watcher.start
    watcher
  end
end
