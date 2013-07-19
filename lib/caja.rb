require "caja/version"
require "caja_store"
require 'file'

module Caja

  def valid_new_file?(path)
    dirname = File.dirname(path)
    if File.exist?(path)
      return false
    else
      return File.exist?(dirname)
    end
  end

  class Box
    attr_reader :path

    def initialize(path)
      unless valid_new_file(path) do
        raise IOError, "invalid path"
      end
      @store = Caja::Store.new(path)
      @path = path
    end

    def store(key, value)
      @store.store(key, value)
    end

    def append(key, value)
      @store.append(key, value)
    end

    def fetch(key)
      @store.fetch(key)
    end

    def delete(key)
      @store.delete(key)
    end

  end

end
