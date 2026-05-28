#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include "esp_http_server.h"

class WebServer {
private:
    httpd_handle_t _server;

    static float    _pct;
    static int      _vProm;
    static uint32_t _minutos;
    static uint32_t _segundos;
    static char     _estabilidad[8];
    static bool _stopActualizado;
    static bool _resumeActualizado;
    static bool _rutinaUpperActaulizada;
    static bool _rutinaMiddleActaulizada;
    static bool _rutinaLowerActaulizada;

    static esp_err_t _handleData(httpd_req_t* req);
    static esp_err_t _handleIndex(httpd_req_t* req);
    static esp_err_t _handleLogs(httpd_req_t* req);
    static esp_err_t _handleLogsData(httpd_req_t* req);
    static esp_err_t _handleControl(httpd_req_t* req);
    static esp_err_t _handleLanding(httpd_req_t* req);
    static esp_err_t _handleDashboard(httpd_req_t* req);
    
    void _initWifi();
    void _initSpiffs();
    void _startServer();

    

public:
    WebServer();
    ~WebServer();

    void init();
    void setData(float pct, int vProm, uint32_t minutos, uint32_t segundos, const char* estabilidad);

    bool getStop();
    bool getResume();
    bool getUpper();
    bool getMiddle();
    bool getLower();

};

#endif
