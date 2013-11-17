require 'rdoc/task'

task :default => :test
task :compile => "extconf:compile"

task :version => :compile do
  $:.unshift(File.expand_path('../ext', __FILE__))
  require 'tidings'
  puts Tidings::VERSION
end

desc "Run all tests"
task :test => :compile do
  Dir[File.dirname(__FILE__) + '/test/**/*_test.rb'].each do |file|
    load file
  end
end

namespace :profile do
  desc "Profile memory"
  task :memory => :compile do
    sh 'valgrind --tool=memcheck --leak-check=yes --num-callers=10 --track-fds=yes ruby test/profile/memory.rb'
  end
end

task :clean do
  crap = "*.{bundle,so,o,obj,log}"
  ["*.gem", "ext/**/#{crap}", "ext/**/Makefile"].each do |glob|
    Dir.glob(glob).each do |file|
      rm(file)
    end
  end
end

namespace :extconf do
  task :makefile do
    Dir.chdir('ext') do
      ruby "extconf.rb"
    end
  end

  task :make => :makefile do
    Dir.chdir('ext') do
      sh(RUBY_PLATFORM =~ /win32/ ? 'nmake' : 'make') do |ok, res|
        if !ok
          require "fileutils"
          FileUtils.rm_rf(Dir.glob('*.{so,o,dll,bundle}'))
        end
      end
    end
  end

  desc "Compile the Ruby extension"
  task :compile => :make do
    if Dir.glob("ext/tidings.{o,so,dll}").length == 0
      $stderr.puts("Failed to build Tidings.")
      exit(1)
    end
  end
end

namespace :docs do
  Rake::RDocTask.new('generate') do |rdoc|
    rdoc.title = 'Tidings'
    rdoc.main = "README.rdoc"
    rdoc.rdoc_files.include('README.rdoc', 'ext/tidings.c')
    rdoc.options << "--all" << "--charset" << "utf-8"
  end
end