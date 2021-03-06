require File.expand_path('../../../../../spec_helper', __FILE__)
require 'net/http'
require File.expand_path('../fixtures/http_server', __FILE__)

describe "Net::HTTP.get when passed URI" do
  before(:all) do
    NetHTTPSpecs.start_server
  end

  after(:all) do
    NetHTTPSpecs.stop_server
  end

  describe "when passed URI" do
    it "returns the body of the specified uri" do
      Net::HTTP.get(URI.parse("http://127.0.0.1:#{NetHTTPSpecs.server_port}/")).should == "This is the index page."
    end
  end

  describe "when passed host, path, port" do
    it "returns the body of the specified host-path-combination" do
      Net::HTTP.get('127.0.0.1', "/", NetHTTPSpecs.server_port).should == "This is the index page."
    end
  end

  ruby_version_is ''...'1.9.3' do
    describe "when passed path in version 1.1 mode" do
      before(:each) do
        Net::HTTP.version_1_1
        @http = Net::HTTP.start("127.0.0.1", NetHTTPSpecs.server_port)
      end

      after(:each) do
        Net::HTTP.version_1_2
      end

      it "returns the response and the body for the passed path" do
        response, body = @http.get("/")
        response.should be_kind_of(Net::HTTPResponse)

        body.should == "This is the index page."
        response.body.should == "This is the index page."
      end

      it "yields each read part of the body to the passed block when passed a block" do
        buf = ""
        @http.get("/") { |s| buf << s }
        buf.should == "This is the index page."
      end
    end
  end
end
