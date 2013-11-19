module Tidings
  class Watcher
    def initialize(path, processor)
      @path, @processor, @pid = path, processor, nil
    end

    def start
      Signal.trap("SIGINT") { stop }
      @pid = fork { watch }
      Tidings.log("Tidings running on PID: #{@pid}")
      Process.wait
      Tidings.log("Tidings stopped running")
    end

    def stop
      Process.kill("KILL", @pid)
    end

    if const_defined?(:FSEvent)
      def watch
        FSEvent.watch(@path, @processor)
      end
    elsif const_defined?(:Inotify)
      def watch
        Inotify.watch(@path, @processor)
      end
    end
  end
end