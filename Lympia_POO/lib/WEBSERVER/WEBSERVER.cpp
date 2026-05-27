#include "WEBSERVER.h"
#include "LOGBUFFER.h"
#include "index_html.h"
#include "logs_html.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include <string.h>
#include <stdlib.h>

#define AP_SSID    "Masajeador"
#define AP_CHANNEL 1
#define AP_MAX_STA 4

float    WebServer::_pct            = 0.0f;
int      WebServer::_vProm          = 0;
uint32_t WebServer::_minutos        = 0;
uint32_t WebServer::_segundos       = 0;
char     WebServer::_estabilidad[8] = "baja";

WebServer::WebServer() : _server(nullptr) {}
WebServer::~WebServer() {}

/* ── WiFi event handler (file-local) ── */
static void _wifiEventHandler(void* arg, esp_event_base_t base,
                               int32_t id, void* data) {
    if (id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* e = (wifi_event_ap_staconnected_t*)data;
        LogBuffer::log("INFO",
            "Cliente conectado: %02x:%02x:%02x:%02x:%02x:%02x AID=%d",
            e->mac[0], e->mac[1], e->mac[2], e->mac[3], e->mac[4], e->mac[5],
            (int)e->aid);
    } else if (id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* e = (wifi_event_ap_stadisconnected_t*)data;
        LogBuffer::log("INFO",
            "Cliente desconectado: %02x:%02x:%02x:%02x:%02x:%02x AID=%d razon=%d",
            e->mac[0], e->mac[1], e->mac[2], e->mac[3], e->mac[4], e->mac[5],
            (int)e->aid, (int)e->reason);
    }
}

/* ── _initWifi ── */
void WebServer::_initWifi() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        LogBuffer::log("WARN", "NVS corrupta, borrando y re-inicializando");
        nvs_flash_erase();
        ret = nvs_flash_init();
        if (ret != ESP_OK) {
            LogBuffer::log("ERROR", "NVS init fallo tras borrado: %d", (int)ret);
        }
    }

    esp_netif_init();
    ret = esp_event_loop_create_default();
    /* ESP_ERR_INVALID_STATE means loop already exists — safe to ignore */
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        LogBuffer::log("ERROR", "Event loop create fallo: %d", (int)ret);
    }
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) {
        LogBuffer::log("ERROR", "esp_wifi_init fallo: %d", (int)ret);
        return;
    }

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                &_wifiEventHandler, nullptr);

    wifi_config_t ap_cfg;
    memset(&ap_cfg, 0, sizeof(ap_cfg));
    memcpy(ap_cfg.ap.ssid, AP_SSID, strlen(AP_SSID));
    ap_cfg.ap.ssid_len       = (uint8_t)strlen(AP_SSID);
    ap_cfg.ap.channel        = AP_CHANNEL;
    ap_cfg.ap.authmode       = WIFI_AUTH_OPEN;
    ap_cfg.ap.max_connection = AP_MAX_STA;

    ret = esp_wifi_set_mode(WIFI_MODE_AP);
    if (ret != ESP_OK) {
        LogBuffer::log("ERROR", "esp_wifi_set_mode fallo: %d", (int)ret);
        return;
    }
    ret = esp_wifi_set_config(WIFI_IF_AP, &ap_cfg);
    if (ret != ESP_OK) {
        LogBuffer::log("ERROR", "esp_wifi_set_config fallo: %d", (int)ret);
        return;
    }
    ret = esp_wifi_start();
    if (ret != ESP_OK) {
        LogBuffer::log("ERROR", "esp_wifi_start fallo: %d", (int)ret);
        return;
    }
    LogBuffer::log("INFO", "AP iniciado — SSID: %s, canal: %d", AP_SSID, AP_CHANNEL);
}

void WebServer::_initSpiffs() {
    /* not used — pages are embedded in firmware as C strings */
}

/* ── HTTP handlers ── */
esp_err_t WebServer::_handleData(httpd_req_t* req) {
    const char* estado;
    float pct = _pct;

    if (pct < 30.0f)      estado = "insuficiente";
    else if (pct < 80.0f) estado = "\xc3\xb3ptimo";  /* óptimo UTF-8 */
    else                  estado = "excesivo";

    char json[192];
    snprintf(json, sizeof(json),
        "{\"pct\":%.1f,\"vProm\":%d,\"estado\":\"%s\","
        "\"minutos\":%lu,\"segundos\":%lu,\"estabilidad\":\"%s\"}",
        pct, _vProm, estado, (unsigned long)_minutos, (unsigned long)_segundos, _estabilidad);

    httpd_resp_set_type(req, "application/json; charset=utf-8");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    esp_err_t err = httpd_resp_sendstr(req, json);
    if (err != ESP_OK) {
        LogBuffer::log("WARN", "/data envio fallo: %d", (int)err);
    }
    return err;
}

esp_err_t WebServer::_handleIndex(httpd_req_t* req) {
    httpd_resp_set_type(req, "text/html; charset=utf-8");
    esp_err_t err = httpd_resp_send(req, INDEX_HTML, HTTPD_RESP_USE_STRLEN);
    if (err != ESP_OK) {
        LogBuffer::log("WARN", "/ envio fallo: %d", (int)err);
    }
    return err;
}

esp_err_t WebServer::_handleLogs(httpd_req_t* req) {
    httpd_resp_set_type(req, "text/html; charset=utf-8");
    esp_err_t err = httpd_resp_send(req, LOGS_HTML, HTTPD_RESP_USE_STRLEN);
    if (err != ESP_OK) {
        LogBuffer::log("WARN", "/logs envio fallo: %d", (int)err);
    }
    return err;
}

esp_err_t WebServer::_handleLogsData(httpd_req_t* req) {
    char* buf = (char*)malloc(8192);
    if (!buf) {
        LogBuffer::log("ERROR", "/logs-data sin memoria heap");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "OOM");
        return ESP_FAIL;
    }
    int len = LogBuffer::fillJson(buf, 8192);
    httpd_resp_set_type(req, "application/json; charset=utf-8");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    esp_err_t err = httpd_resp_send(req, buf, len);
    free(buf);
    if (err != ESP_OK) {
        LogBuffer::log("WARN", "/logs-data envio fallo: %d", (int)err);
    }
    return err;
}

/* ── _startServer ── */
void WebServer::_startServer() {
    httpd_config_t config   = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    esp_err_t err = httpd_start(&_server, &config);
    if (err != ESP_OK) {
        LogBuffer::log("ERROR", "HTTP server no pudo iniciar: %d", (int)err);
        return;
    }
    LogBuffer::log("INFO", "HTTP server iniciado en puerto %d", (int)config.server_port);

    httpd_uri_t u_data;
    memset(&u_data, 0, sizeof(u_data));
    u_data.uri     = "/data";
    u_data.method  = HTTP_GET;
    u_data.handler = _handleData;
    if (httpd_register_uri_handler(_server, &u_data) != ESP_OK)
        LogBuffer::log("ERROR", "Registro /data fallo");

    httpd_uri_t u_index;
    memset(&u_index, 0, sizeof(u_index));
    u_index.uri     = "/";
    u_index.method  = HTTP_GET;
    u_index.handler = _handleIndex;
    if (httpd_register_uri_handler(_server, &u_index) != ESP_OK)
        LogBuffer::log("ERROR", "Registro / fallo");

    httpd_uri_t u_logs;
    memset(&u_logs, 0, sizeof(u_logs));
    u_logs.uri     = "/logs";
    u_logs.method  = HTTP_GET;
    u_logs.handler = _handleLogs;
    if (httpd_register_uri_handler(_server, &u_logs) != ESP_OK)
        LogBuffer::log("ERROR", "Registro /logs fallo");

    httpd_uri_t u_ldat;
    memset(&u_ldat, 0, sizeof(u_ldat));
    u_ldat.uri     = "/logs-data";
    u_ldat.method  = HTTP_GET;
    u_ldat.handler = _handleLogsData;
    if (httpd_register_uri_handler(_server, &u_ldat) != ESP_OK)
        LogBuffer::log("ERROR", "Registro /logs-data fallo");
}

/* ── public ── */
void WebServer::init() {
    _initWifi();
    _startServer();
}

void WebServer::setData(float pct, int vProm, uint32_t minutos, uint32_t segundos,
                        const char* estabilidad) {
    _pct      = pct;
    _vProm    = vProm;
    _minutos  = minutos;
    _segundos = segundos;
    strncpy(_estabilidad, estabilidad, sizeof(_estabilidad) - 1);
    _estabilidad[sizeof(_estabilidad) - 1] = '\0';
}
