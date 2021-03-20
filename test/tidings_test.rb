# frozen_string_literal: true

require File.expand_path('test_helper', __dir__)

class TidingsTest < Minitest::Test
  def test_returns_its_version
    assert_match(/\d+\.\d+\.\d+/, Tidings::VERSION)
  end
end
