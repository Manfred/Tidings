require 'mkmf'

if RUBY_PLATFORM =~ /darwin/
  with_ldflags("-framework CoreServices") do
    create_makefile("fs_event")
  end
end