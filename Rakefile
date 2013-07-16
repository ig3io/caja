require 'rake/extensiontask'
require "bundler/gem_tasks"
require 'rspec/core/rake_task'

RSpec::Core::RakeTask.new(:spec)

task :default => :spec

spec = Gem::Specification.load('caja.gemspec')
Rake::ExtensionTask.new('caja_store', spec)
