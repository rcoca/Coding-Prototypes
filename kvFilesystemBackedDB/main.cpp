#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <random>
#include "httplib.h"

#include <ext/stdio_filebuf.h>



class DBBackend
{
    std::filesystem::path db_path;
    std::filesystem::path tmp_path;
public:
    using key_type = std::string;
    using value_type = std::string;

    DBBackend(const std::filesystem::path& dbDir);
    std::vector<key_type> list(); //iterator based?
    value_type find(const key_type& key);
    bool insert(const key_type& key, const value_type& value);
    bool remove(const key_type& key);

};

DBBackend::DBBackend(const std::filesystem::path& dbDir):db_path{dbDir},tmp_path{std::filesystem::path{"tmp"}}
{    
    //throw if not dir!
    std::error_code ec;
    
    std::filesystem::create_directory(db_path,ec);
    std::cerr<<db_path<<ec<<std::endl;
    std::filesystem::create_directory(db_path/tmp_path,ec);
    std::cerr<<tmp_path<<ec<<std::endl;
    std::filesystem::current_path(db_path);
}

std::vector<DBBackend::key_type> DBBackend::list()
{
    std::vector<DBBackend::value_type> vals;
    std::filesystem::directory_iterator begin(".");
    std::filesystem::directory_iterator end;
    //std::filter_transform() - remove tmp dir
    for(auto it=begin;it!=end;++it){
        if(!std::filesystem::is_directory(it->path()))
            vals.push_back(it->path().filename().string());
    }
    return vals;
}

DBBackend::value_type DBBackend::find(const DBBackend::key_type& key)
{
    std::ifstream in(key,std::ios::binary);
    if(!in.good())
        throw std::runtime_error("cannot find "+key);
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

static std::string tempFilename(std::size_t len=32)
{
    static constexpr auto chars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    thread_local auto rng = std::default_random_engine();
    auto dist = std::uniform_int_distribution{{}, std::strlen(chars) - 1};
    auto result = std::string(len, '\0');
    std::generate_n(begin(result), len, [&]() { return chars[dist(rng)]; });
    return result;
}

bool DBBackend::insert(const DBBackend::key_type& key, const DBBackend::value_type& value)
{      
    std::string tmp_file=(tmp_path/std::filesystem::path{tempFilename()}).string();    
    try{        
        std::ofstream out(tmp_file,std::ios::out|std::ios::binary|std::ios::app);                    
        if(!out.is_open())
            throw std::runtime_error(" "+key);        
        out.write(value.data(),value.length());
        if(!out.good())
            throw std::runtime_error("couldn't write data "+key);    
        std::filesystem::rename({tmp_file},key);
    }
    catch (const std::exception& e){
        std::error_code ec;
        std::filesystem::remove({key},ec);
        std::filesystem::remove({tmp_file},ec);
        throw;
    }
    return true;
}
bool DBBackend::remove(const DBBackend::key_type& key)
{
    //if(within DB dir)
    return std::filesystem::remove({key});        
}


int main(int argc, char* argv[])
{
    DBBackend DB("DB");
    httplib::Server svr;
    size_t BUFSIZE = 256;

    svr.set_error_handler([](const auto& req, auto& res) {
        auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
        char buf[BUFSIZ];
        snprintf(buf, sizeof(buf), fmt, res.status);
        res.set_content(buf, "text/html");
    });
    svr.set_exception_handler([](const auto& req, auto& res, std::exception_ptr ep) {
        auto fmt = "<h1>Error 500</h1><p>%s</p>";
        char buf[BUFSIZ];
        try {
            std::rethrow_exception(ep);
        } catch (std::exception &e) {
            snprintf(buf, sizeof(buf), fmt, e.what());
        } catch (...) { // See the following NOTE
            snprintf(buf, sizeof(buf), fmt, "Unknown Exception");
        }
        res.set_content(buf, "text/html");
        res.status = httplib::StatusCode::InternalServerError_500;
    });

    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        res.set_content(R"(
            <form id="DBinsert" action="/insert" method="POST">
                        <input name="key" type="text">
                        <input name="value" type="text">
                        <input type="submit" name="insert" value="insert">
            </form>
            <form id="find" action="/find" method="GET">
                        <input name="key" type="text">
                        <input type="submit" name="find" value="find">
            </form>        
            <form id="list" action="/list" method="GET">
                        <input name="list" type="submit" value="list">
            </form>                            
                        <form id="remove" action="/remove" method="POST">
                        <input name="key" type="text">
                        <input type="submit" value="remove">
            </form>                            
            <form id="stop" action="/stop" method="GET">
                        <input name="stop" type="submit" value="stop">
            </form>                            
                        )",
                        "text/html");
    });

    svr.Get("/find",[&DB](const httplib::Request &req, httplib::Response &res){
        if (req.has_param("key")) {
            auto key = req.get_param_value("key");
            auto blob=DB.find(key);
            res.set_content(blob.c_str(),"text/plain");
            res.status = httplib::StatusCode::OK_200;
            return res.status;
        }        
        res.status = httplib::StatusCode::NoContent_204;
        return res.status;
    });

    svr.Get("/list",[&DB](const httplib::Request &req, httplib::Response &res){
        //fill values
        res.status = httplib::StatusCode::NotFound_404;
        auto listingContainer=DB.list();
        std::ostringstream ss;
        for(const auto& item:listingContainer)
        {
            ss<<item<<"\r\n";
        }
        ss<<std::endl;
        res.set_content(ss.str(),"text/plain");    
        res.status =  httplib::StatusCode::OK_200;
        return res.status;
    });

    svr.Post("/insert", [&DB](const auto& req, auto& res) {
        res.status = httplib::StatusCode::BadRequest_400;
        if (req.has_param("key")) 
        {
            auto key = req.get_param_value("key");
            auto val = req.get_param_value("value");
            DB.insert(key,val);
            res.status = httplib::StatusCode::OK_200;
        }        
        return res.status;
    });   
    svr.Post("/remove", [&DB](const auto& req, auto& res) {
        res.status = httplib::StatusCode::BadRequest_400;
        if (req.has_param("key")) 
        {
            auto key = req.get_param_value("key");            
            if(DB.remove(key))            
                res.status = httplib::StatusCode::OK_200;            
        }        
        return res.status;
    });
    svr.Get("/stop", [&svr](const httplib::Request& req, httplib::Response& res) {
        res.status = httplib::StatusCode::OK_200;
        svr.stop();
        return res.status;
    });

    svr.listen("0.0.0.0", 8080);
    return 0;
}