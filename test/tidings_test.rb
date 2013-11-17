require File.expand_path('test_helper', __dir__)

describe Tidings do
  it "returns its version" do
    Tidings::VERSION.should =~ /\d+\.\d+\.\d+/
  end
end