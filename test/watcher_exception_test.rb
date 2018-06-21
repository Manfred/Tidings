require_relative 'preamble'

class WatcherExceptionTest < Minitest::Test
  def setup
    TestFiles.clear!
  end

  def test_handles_exception_in_handler
    fork do
      sleep 1
      TestFiles.touch('config/database.yml')
    end

    Tidings.watch(File.expand_path('../tmp', __dir__)) do |path, flags|
      raise ArgumentError, "Something went wrong!"
    end
  end
end

Minitest.run