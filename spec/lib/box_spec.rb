require 'spec_helper'
require 'pathname'


describe Caja::Box do
  let(:bad_path) { "./not-existing/database.db"}
  let(:good_path) { "./caja.db" }
  
  it "should raise an error if the data store can not be created" do
    expect { Caja::Box.new(bad_path) }.to raise_error(IOError)
    Pathname::File.exist?(bad_path).should_not be_true
  end

  context "initialization succesful" do
    before { Pathname::File.delete(good_path) if Pathname::File.exist?(good_path) }
    after { Pathname::File.delete(good_path) }
    it "should create a new file after successful store operation" do
      box = Caja::Box.new(good_path)
      box.store("test key", "test value")
      Pathname::File.exist?(good_path).should be_true
    end
  end
  
end
