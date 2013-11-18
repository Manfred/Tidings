module Tidings
  class Watcher
    def initialize(path, processor)
      @path, @processor, @pid = path, processor, nil
    end

    def start
      Signal.trap("SIGINT") { stop }
      @pid = fork do
        FSEvent.watch(@path, @processor)
      end
      Tidings.log("Tidings running on PID: #{@pid}")
      Process.wait
      Tidings.log("Tidings stopped running")
    end

    def stop
      Process.kill("KILL", @pid)
    end
  end
end