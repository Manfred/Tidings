require 'peck'
require 'peck/delegates'
require 'peck/counter'
require 'peck/context'
require 'peck/specification'
require 'peck/expectations'
require 'peck/notifiers/default'

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

$read, $write = IO.pipe

if fork
  $write.close

  describe Tidings::Watcher do
    before do
      TestFiles.clear!
    end

    it "reports changes to the filesystem and its flags" do
      TestFiles.touch('config/database.yml')
      events = read_events
      event = events[0]
      event[0].should.start_with(TestFiles.watch_path)
      event[1].should.include(:created)
    end

    def wait(counter)
      while((counter -= 1) > 0)
        yield(counter)
        sleep 0.1
      end
    end

    def read_events(counter=4)
      buffer = ''
      wait(counter) { buffer << $read.read }
      buffer.split("\n").map { |l| Marshal.load(l) }
    end
  end

  sleep 0.1

  Peck::Notifiers::Default.use
  Peck.run

else
  $read.close

  TestFiles.clear!
  Tidings.watch(File.expand_path('../tmp', __dir__)) do |path, flags|
    $write.puts(Marshal.dump([path, flags]))
    exit
  end

end

