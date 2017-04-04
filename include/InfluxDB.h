//
// Created by cnb on 3/30/17.
//

#ifndef TARAXACUM_INFLUXDB_H
#define TARAXACUM_INFLUXDB_H

#include <beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

class InfluxDB {

private:
    std::string influx_host, influx_db, influx_url, influx_body, influx_port;
    std::stringstream ss;

    void Loadconfig() {
        std::cout << "loading data" << std::endl;
        FILE *fp = fopen("/opt/app/bin/config.json", "rb");
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
        if (d.HasMember("Influx")) {

            const rapidjson::Value &influx_array = d["Influx"];

            assert(influx_array.IsArray());

            auto v = influx_array.GetArray();
            influx_host = v[0].GetString();
            influx_port = v[1].GetString();
            influx_db = v[2].GetString();


        } else {
            fclose(fp);
            std::cout << "config.json should have a key named Influx with the "
                    "following format "
                      << std::endl;
            std::cout << "\"Influx\": [\"host\",\"port\",\"mydb\",\"url\"]\" "
                      << std::endl;
        }
    }

public:
    InfluxDB() { Loadconfig(); }
    InfluxDB(const std::string host, const std::string db, const std::string port, const std::string url)
            : influx_host(host), influx_db(db), influx_port(port), influx_url(url) {};


    void Write(const std::string data) {
        beast::http::request<beast::http::string_body> req;
        boost::asio::io_service ios;
        boost::asio::ip::tcp::resolver r{ios};
            boost::asio::ip::tcp::socket sock{ios};
            try {
                boost::asio::connect(sock,
                                     r.resolve(boost::asio::ip::tcp::resolver::query{
                                             influx_host, influx_port}));
            } catch (...) {
                std::cout << "Err Cannot resolve host " << influx_host << " at port "
                          << influx_port << std::endl;
                return;
            }

            // Send HTTP request using beast
            req.method = "POST";
            req.url = "/write?db=" + influx_db ;
            req.version = 11;
            req.fields.replace("Host", influx_host + ":" +
                                       boost::lexical_cast<std::string>(
                                               sock.remote_endpoint().port()));
            req.fields.replace("User-Agent", "Beast");
            req.fields.insert("Content-Type", "application/x-www-form-urlencoded");
            req.fields.insert("Accept"," */*");
            req.body = data;

            std::cout << "Sending data " << data << " to InfluxDB" << std::endl;

            beast::http::prepare(req);
            try {
                beast::http::write(sock, req);
                // Receive and print HTTP response using beast
                beast::streambuf sb;
                beast::http::response<beast::http::streambuf_body> resp;
                beast::http::read(sock, sb, resp);
                std::cout << resp;
            } catch (std::runtime_error &e) {
                std::cout << "Cannot Write data to InfluxDB" << std::endl;
                std::cout << "Stack:" << e.what() << std::endl;
                return;
            }

    }


    const std::string Query(const std::string query) {
        beast::http::request<beast::http::string_body> req;
        boost::asio::io_service ios;
        boost::asio::ip::tcp::resolver r{ios};
        boost::asio::ip::tcp::socket sock{ios};
        try {
            boost::asio::connect(sock,
                                 r.resolve(boost::asio::ip::tcp::resolver::query{
                                         influx_host, influx_port}));
        } catch (...) {
            std::cout << "Err Cannot resolve host " << influx_host << " at port "
                      << influx_port << std::endl;
            return "{err: \" Cannot Resolve Host \" }";
        }

        // Send HTTP request using beast
        req.method = "POST";
        req.url = "/query";
        req.version = 11;
        req.body = query;
        req.fields.replace("Host", influx_host + ":" +
                                   boost::lexical_cast<std::string>(
                                           sock.remote_endpoint().port()));
        req.fields.replace("User-Agent", "Beast");

        beast::http::prepare(req);
        try {
            beast::http::write(sock, req);
            beast::streambuf sb;
            beast::http::response<beast::http::streambuf_body> resp;
            beast::http::read(sock, sb, resp);
            std::cout << resp;
        } catch (std::runtime_error &e) {
            std::cout << "Cannot Write data to InfluxDB" << std::endl;
            std::cout << "Stack:" << e.what() << std::endl;
            return "{ \"err\": \"Cannot Write to DB\"}";
        }

    }

};

#endif // TARAXACUM_INFLUXDB_H
