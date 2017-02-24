# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'fast_combine/version'

Gem::Specification.new do |spec|
  spec.name          = 'fast_combine'
  spec.version       = FastCombine::VERSION
  spec.authors       = ['Sergey Kukunin']
  spec.email         = ['sergey.kukunin@gmail.com']

  spec.summary       = 'C implementation of #combine function'
  spec.description   = 'Native C implementation of #combine function used by rom-rb.'
  spec.homepage      = 'https://github.com/matic-insurance/fast_combine'

  spec.files = `git ls-files -z`.split("\x0").reject do |f|
    f.match(%r{^(test|spec|features)/})
  end
  spec.bindir        = 'exe'
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ['lib']
  spec.extensions    = ['ext/fast_combine/extconf.rb']

  spec.add_development_dependency 'bundler', '~> 1.14'
  spec.add_development_dependency 'rake', '~> 10.0'
  spec.add_development_dependency 'rake-compiler'
  spec.add_development_dependency 'rspec', '~> 3.0'
end
