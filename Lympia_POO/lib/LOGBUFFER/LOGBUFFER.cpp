#include "LOGBUFFER.h"
#include "esp_timer.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

LogEntry          LogBuffer::_entries[LOG_CAP];
int               LogBuffer::_head  = 0;
int               LogBuffer::_count = 0;
SemaphoreHandle_t LogBuffer::_mutex = nullptr;

void LogBuffer::_ensureInit() {
    if (_mutex == nullptr) {
        _mutex = xSemaphoreCreateMutex();
    }
}

void LogBuffer::log(const char* level, const char* fmt, ...) {
    _ensureInit();

    char msg[LOG_MSG_MAX];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    printf("[%s] %s\n", level, msg);

    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(20)) != pdTRUE) return;

    int idx;
    if (_count < LOG_CAP) {
        idx = (_head + _count) % LOG_CAP;
        _count++;
    } else {
        idx = _head;
        _head = (_head + 1) % LOG_CAP;
    }

    _entries[idx].ms = (uint32_t)(esp_timer_get_time() / 1000ULL);
    strncpy(_entries[idx].level, level, sizeof(_entries[idx].level) - 1);
    _entries[idx].level[sizeof(_entries[idx].level) - 1] = '\0';
    strncpy(_entries[idx].msg, msg, sizeof(_entries[idx].msg) - 1);
    _entries[idx].msg[sizeof(_entries[idx].msg) - 1] = '\0';

    xSemaphoreGive(_mutex);
}

int LogBuffer::fillJson(char* buf, int bufLen) {
    _ensureInit();
    if (xSemaphoreTake(_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return snprintf(buf, bufLen, "[]");
    }

    int pos = 0;
    pos += snprintf(buf + pos, bufLen - pos, "[");
    bool first = true;

    for (int i = _count - 1; i >= 0 && pos < bufLen - 50; i--) {
        int idx = (_head + i) % LOG_CAP;
        if (!first) pos += snprintf(buf + pos, bufLen - pos, ",");
        pos += snprintf(buf + pos, bufLen - pos,
            "{\"ms\":%u,\"level\":\"%s\",\"msg\":\"",
            (unsigned)_entries[idx].ms, _entries[idx].level);
        for (const char* p = _entries[idx].msg; *p && pos < bufLen - 10; p++) {
            if (*p == '"' || *p == '\\') buf[pos++] = '\\';
            buf[pos++] = *p;
        }
        pos += snprintf(buf + pos, bufLen - pos, "\"}");
        first = false;
    }

    pos += snprintf(buf + pos, bufLen - pos, "]");
    xSemaphoreGive(_mutex);
    return pos;
}
