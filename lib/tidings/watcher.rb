module Tidings
  class Watcher
    def initialize(path, processor)
      @path, @processor, @pid = path, processor, nil
    end

    def start
      Signal.trap("HUP") { stop }
      Signal.trap("SIGINT") { stop }
      @pid = fork do
        begin
          watch
        rescue Exception => exception
          Tidings.log(
            "Tidings encountered an uncaught exeption: #{exception.message}"
          )
          exit
        end
      end
      Tidings.log("Tidings running on PID: #{@pid}")
      Process.waitpid(@pid)
      Tidings.log("Tidings stopped running")
    end

    def stop
      Process.kill("KILL", @pid)
    rescue Errno::ESRCH
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