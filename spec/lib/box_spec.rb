require 'spec_helper'
require 'pathname'


describe Caja::Box do
  let(:bad_path) { "~/i-dont-exist.db"}
  let(:good_path) { "./caja.db" }
  
  it "should raise an error if the data store can not be created" do
    expect { Caja::Box.new(bad_path) }.to raise_error(Caja::IOError)
    PathName::File.exist?(bad_path).should_not be_true
  end

  it "should create a new file in case the initialization is successful" do
    before { fail if PathName::File.exist?(good_path) }
    after { PathName::File.delete(good_path) }

    box = Caja::Box.new(good_path)
    PathName::File.exist?(good_path).should be_true
  end
  
end
