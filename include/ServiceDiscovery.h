    //
    // Created by cnb on 4/3/17.
    //

    #ifndef TARAXACUM_CONSUL_H
    #define TARAXACUM_CONSUL_H

    #include<string>
    #include<vector>
    #include<iostream>
    #include <beast/core/to_string.hpp>
    #include <beast/http.hpp>
    #include <boost/asio.hpp>
    #include <boost/lexical_cast.hpp>
    #include <rapidjson/filereadstream.h>
    #include <rapidjson/document.h>
    #include <rapidjson/error/en.h>


    const std::string GetHttp(const std::string url, const std::string host, const std::string port) {
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


    class Consuld {
    private:
        std::string host, port, url;
        std::vector<std::string> upstreams;

        void Loadconfig() {
            std::stringstream ss;
            std::cout << "loading Consul data" << std::endl;
            FILE* fp = nullptr;
                 if(const char* env_p = std::getenv("CONFIG"))
        {
            std::cout << "Your config.json PATH is: " << env_p << '\n';
            fp = fopen(env_p, "rb");

        } else {
            fp = fopen("./config.json", "rb");
            }
            if (fp == nullptr) {
                ss << "Exception opening/reading configuration file" << std::endl;
                ss << "Is config.json created ?" << std::endl;
                ss << "config.json should have a key named Influx :" << std::endl;
                ss << "{ \"\" ,... \"}\"" << std::endl;
                std::cout << ss.str() << std::endl;
                throw std::runtime_error(std::strerror(errno));
            }

            char readBuffer[1024];
            rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
            rapidjson::Document d;

            if (d.ParseStream(is).HasParseError()) {
                ss << "{\"errcode:\"" << (unsigned) d.GetErrorOffset()
                   << ", \"errmsg\":" << rapidjson::GetParseError_En(d.GetParseError())
                   << "}";
                std::cout << ss.str() << std::endl;
                fclose(fp);
            }


            if (d.HasMember("Consuld")) {

                const rapidjson::Value &influx_array = d["Consuld"];

                assert(influx_array.IsArray());
                auto v = influx_array.GetArray();
                host = v[0].GetString();
                port = v[1].GetString();
                url = v[2].GetString();

            } else {
                fclose(fp);
                std::cout << "config.json should have a key named Consuld with the "
                        "following format "
                          << std::endl;
                std::cout << "\"Consuld\": [\"host\",\"port\",\"mydb\",\"url\"]\" "
                          << std::endl;
            }
        }

        void getupstream(const std::string json) {
            rapidjson::Document d;
            std::stringstream ss;
    #ifdef XXX
            std::cout << "Json dump "<< std::endl;
            Taraxacum::Tools::hexdump(reinterpret_cast<void*>(const_cast<char*>(json.data())),json.size());
    #endif
            if (d.Parse<rapidjson::kParseStopWhenDoneFlag>(json.data()).HasParseError()) {
                std::cout << "{\"errcode:\"" << (unsigned) d.GetErrorOffset() << std::endl;
                std::cout << ", \"errmsg\":" << rapidjson::GetParseError_En(d.GetParseError()) << std::endl;
                std::cout << "}" << std::endl;
                std::cout << "Consul is down ? not updating upstream list" << std::endl;

            }
            assert(d.IsArray());


            if (d[0].HasMember("Service")) {
                std::cout << "getting service" << std::endl;
                const rapidjson::Value &v = d[0]["Service"];
                upstreams.push_back(v["Address"].GetString());
            }

        };
    public:
        Consuld() { Loadconfig(); }
        const std::string getport() { return port;}
        const std::string  geturl() {return url;}
        const std::string gethost() {return host;}
        void sethost(const std::string _host) { host = _host;}
        Consuld(const std::string url_, const std::string host_, const std::string port_) : url(url_), host(host_),
                                                                                            port(port_) {};

        const std::string Upstream() {
            getupstream(GetHttp(this->url, this->host, this->port));
            const std::string v = upstreams.front().data();
            return v;
        }

    };


    #endif //TARAXACUM_CONSUL_H
