# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'caja/version'

Gem::Specification.new do |spec|
  spec.name          = "caja"
  spec.version       = Caja::VERSION
  spec.authors       = ["Ignacio Contreras Pinilla"]
  spec.email         = ["ignacio@ignacio.cat"]
  spec.description   = %q{UnQLite powered simple key-value storage}
  spec.summary       = %q{Simple key-value storage that uses the UnQLite NoSQL serverless library}
  spec.homepage      = ""
  spec.license       = "MIT"

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.3"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rspec"
end
