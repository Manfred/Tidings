require 'mkmf'

if RUBY_PLATFORM =~ /darwin/
  with_ldflags("-framework CoreServices") do
    create_makefile("tidings")
  end
end