# Tidings

A platform optimized file-change watcher.

## Installation

    gem install tidings

## Examples

Tidings registers with the operating system to receive callbacks for changes to the filesystem relative to a path. For every change it reports to anything that responds to: `call(file_or_path, flags)`. For example, a block:

```ruby
Tidings.watch('/Users/janice/Code') do |file_or_path, flags|
  if flags.include?(:dir)
    puts "Change to directory: #{file_or_path}"
  else
    puts "Change to file: #{file_or_path}"
  end
end
```

Or a class:

```ruby
module MyFileWatcher
  def self.call(file_or_path, flags)
    puts "#{file_or_path.inspect}:#{flags.inspect}"
  end
end

Tidings.watch('/Users/janice/Code', MyFileWatcher)
```

Tidings swallows exceptions raised by the watcher because that's useful for ad-hoc scripts. If you don't want this you can either catch them yourself or drop down to Tidings internals.

```ruby
Tidings.watch(__dir__) do |file|
  do_something(file)
rescue Exception => e
  $stderr.write("🤕 #{e.message}")
end
```

You can drop down to use the low-level implementation directly. Either `FSEvent` or `Inotify` will be defined depending on what compiled on your operating system.

```ruby
require 'tidings'

def watch(path, &block)
  if const_defined?(:FSEvent)
    def watch
      FSEvent.watch(path, block)
    end
  elsif const_defined?(:Inotify)
    def watch
      Inotify.watch(path, block)
    end
  end
end

watch(__dir__) do |file|
  puts "Change: #{file}"
end
```

## Copyright

MIT license, see COPYING.
