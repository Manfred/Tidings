# Tidings

A platform optimized file-change watcher.

## Installation

  $ gem install tidings

## Short example

```ruby
Tidings.watch('/Users/janice/Code') do |file_or_path, flags|
  if flags.include(:dir)
    puts "Change to directory: #{file_or_path}"
  else
    puts "Change to file: #{file_or_path}"
  end
end
```

## Long example

```ruby
module MyFileWatcher
  def self.call(file_or_path, flags)
    puts "#{file_or_path.inspect}:#{flags.inspect}"
  end
end

Tidings.watch('/Users/janice/Code', MyFileWatcher)
````
