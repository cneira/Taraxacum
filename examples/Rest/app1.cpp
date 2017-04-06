/*
 *  Simple example using the framework.
 *  Copyright (C) Carlos Neira <cneirabustos@gmail.com>
 *
 * This program is free software ; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation ; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY ; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the program ; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <Microservice.h>
#include <ServiceDiscovery.h>
#include <csignal>
#include <cstdlib>

namespace {
	const std::string GetHttp2(const std::string url, const std::string host, const std::string port) {
		beast::http::request<beast::http::empty_body> req;
		boost::asio::io_service ios;
		boost::asio::ip::tcp::resolver r{ios};
		boost::asio::ip::tcp::socket sock{ios};
		std::stringstream ss;

		std::cout << "Trying host " << host << " port " << port << "  url " << url << std::endl;
		try {
			boost::asio::connect(sock,
					     r.resolve(boost::asio::ip::tcp::resolver::query{
							     host, port}));
		} catch (...) {
			std::cout << "Err Cannot resolve host " << host << " at port "
				  << port << std::endl;
			return "{err: \" Cannot Resolve Host \" }";
		}


		std::cout << "Preparing request" << std::endl;
		// Send HTTP request using beast
		req.method = "GET";
		req.url = url;
		req.version = 11;
		req.fields.replace("Host", host + ":" +
				   boost::lexical_cast<std::string>(
					   sock.remote_endpoint().port()));
		req.fields.replace("User-Agent", "Taraxacum/Beast");

		beast::http::prepare(req);
		try {
			beast::http::write(sock, req);
			beast::streambuf sb;
			beast::http::response<beast::http::string_body> resp;
			beast::http::read(sock, sb, resp);
			std::cout << "resp:<" << resp.body << ">:end" << std::endl;
			return resp.body;
		} catch (std::runtime_error &e) {
			std::cout << "Stack:" << e.what() << std::endl;
			return "{ \"err\": \"Cannot get data from Consuld\"}";
		}

	}

}


namespace uRest {
  
	std::string data;
	Consuld consul;

	void biz(const Rest::Request &request, Http::ResponseWriter response) {

		std::string app2_response = GetHttp2("/app2",consul.gethost(),"8040");
		response.send(Http::Code::Ok, "{ \"status\": \"ok\", \"arf\": \"110020330303040430304@357212121212121212134434@357212121212121212134434@357212121212121212134434@357212121212121212134434@357212121212121212134434@357212121212121212134434@357212121212121212134434@357212121212121212134434@357212121212121212134434@357212121212121212134434@357212121212121212134434@357212121212121212134434@36100220303030403030403@36100220303030403030403@36100220303030403030403@36100220303030403030403@36100220303030403030403@" + app2_response +  "95003232323232323232332@\", \"microservice_name\": \"app1\"");


	};


	void signalHandler(int signum) {
		std::cout << "Interrupt signal (" << signum << ") received.\n";
		std::string v = consul.Upstream();
		consul.sethost(v) ;
		std::cout << "Reloading Consul data: New upstream " << v << std::endl;

	}
};


int main() {
	using namespace Taraxacum;
	auto usvc = Routing_Microservice<uRest::biz>();
	std::string port = "9032";
	if(const char* env_p = std::getenv("PORT"))
	{
		std::cout << "Your PORT is: " << env_p << '\n';
		port = env_p;
	}


	signal(SIGHUP, uRest::signalHandler);

	usvc.Answer(atoi(port.data()), 2, "/solution", HTTP_METHOD::GET);

	return 0;
}

