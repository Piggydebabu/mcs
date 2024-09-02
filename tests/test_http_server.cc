/**
 * @file test_http_server.cc
 * @brief HttpServer测试
 * @version 0.1
 * @date 2021-09-28
 */
#include "mcs/mcs.h"

static mcs::Logger::ptr g_logger = MCS_LOG_ROOT();

#define XX(...) #__VA_ARGS__

mcs::IOManager::ptr worker;

void run() {
    g_logger->setLevel(mcs::LogLevel::INFO);
    //mcs::http::HttpServer::ptr server(new mcs::http::HttpServer(true, worker.get(), mcs::IOManager::GetThis()));
    mcs::http::HttpServer::ptr server(new mcs::http::HttpServer(true));
    mcs::Address::ptr addr = mcs::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while (!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    sd->addServlet("/mcs/xx", [](mcs::http::HttpRequest::ptr req, mcs::http::HttpResponse::ptr rsp, mcs::http::HttpSession::ptr session) {
        rsp->setBody(req->toString());
        return 0;
    });

    sd->addGlobServlet("/mcs/*", [](mcs::http::HttpRequest::ptr req, mcs::http::HttpResponse::ptr rsp, mcs::http::HttpSession::ptr session) {
        rsp->setBody("Glob:\r\n" + req->toString());
        return 0;
    });

    sd->addGlobServlet("/mcsx/*", [](mcs::http::HttpRequest::ptr req, mcs::http::HttpResponse::ptr rsp, mcs::http::HttpSession::ptr session) {
        rsp->setBody(XX(<html>
                                <head><title> 404 Not Found</ title></ head>
                                <body>
                                <center><h1> 404 Not Found</ h1></ center>
                                <hr><center>
                                    nginx /
                                1.16.0 <
                            / center >
                            </ body>
                            </ html> < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >));
        return 0;
    });

    server->start();
}

int main(int argc, char **argv) {
    mcs::EnvMgr::GetInstance()->init(argc, argv);
    mcs::Config::LoadFromConfDir(mcs::EnvMgr::GetInstance()->getConfigPath());
    
    mcs::IOManager iom(1, true, "main");
    worker.reset(new mcs::IOManager(3, false, "worker"));
    iom.schedule(run);
    return 0;
}
