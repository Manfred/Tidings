require 'minitest/autorun'

%w(lib ext).each do |path|
  $:.unshift(File.expand_path("../#{path}", __dir__))
end

require 'tidings'