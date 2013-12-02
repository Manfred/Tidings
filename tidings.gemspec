$:.unshift File.expand_path('../lib', __FILE__)
require 'tidings/version'

Gem::Specification.new do |spec|
  spec.name     = "tidings"
  spec.version  = Tidings::VERSION
  spec.date     = Date.today

  spec.summary  = "A platform optimized file-change watcher."
  spec.description = "Tidings lets you watch filesystem changes and act on them."
  spec.authors  = ["Manfred Stienstra"]
  spec.homepage = "http://github.com/Manfred/tidings"
  spec.email = ['manfred@fngtpspec.com']

  spec.extensions << 'ext/extconf.rb'
  spec.files = Dir.glob('{bin,lib,ext}/**/*') + [
    'README.rdoc',
    'COPYING'
  ]
  spec.extra_rdoc_files = %w(
    README.rdoc
    COPYING
  )

  spec.add_development_dependency("peck")
  spec.add_development_dependency("fakefs")
end

