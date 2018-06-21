require 'minitest'

%w(lib ext).each do |path|
  $:.unshift(File.expand_path("../#{path}", __dir__))
end

require 'tidings'
require 'stringio'
require 'fileutils'

class TestFiles
  def self.watch_path
    File.expand_path('../tmp', __dir__)
  end

  def self.touch(path)
    filename = File.join(watch_path, path)
    FileUtils.mkdir_p(File.dirname(filename))
    FileUtils.touch(filename)
  end

  def self.clear!
    Dir.glob(File.join(watch_path, '*')).each do |path|
      FileUtils.rm_rf(path)
    end
  end
end