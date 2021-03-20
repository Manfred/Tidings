# frozen_string_literal: true

require 'minitest'

%w[lib ext].each do |path|
  $LOAD_PATH.unshift(File.expand_path("../#{path}", __dir__))
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

$read, $write = IO.pipe

if fork
  $write.close

  class WatcherTest < Minitest::Test
    def setup
      TestFiles.clear!
    end

    def test_reports_changes_to_the_filesystem_and_its_flags
      TestFiles.touch('config/database.yml')
      events = read_events
      event = events[0]
      assert event[0].start_with?(TestFiles.watch_path)
      assert event[1].include?(:created)
    end

    private

    def wait(counter)
      while (counter -= 1).positive?
        yield(counter)
        sleep 0.1
      end
    end

    def read_events(counter = 4)
      buffer = +''
      wait(counter) { buffer << $read.read }
      buffer.split("\n").map { |l| Marshal.load(l) }
    end
  end

  sleep 0.1

  Minitest.run

else

  $read.close
  TestFiles.clear!
  Tidings.watch(File.expand_path('../tmp', __dir__)) do |path, flags|
    $write.puts(Marshal.dump([path, flags]))
    exit
  end

end
