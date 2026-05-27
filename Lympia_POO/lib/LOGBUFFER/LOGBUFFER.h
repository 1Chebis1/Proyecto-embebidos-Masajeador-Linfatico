#ifndef __LOGBUFFER_H__
#define __LOGBUFFER_H__

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#define LOG_CAP     50
#define LOG_MSG_MAX 100

struct LogEntry {
    uint32_t ms;
    char     level[6];
    char     msg[LOG_MSG_MAX];
};

class LogBuffer {
public:
    static void log(const char* level, const char* fmt, ...);
    static int  fillJson(char* buf, int bufLen);
private:
    static LogEntry          _entries[LOG_CAP];
    static int               _head;
    static int               _count;
    static SemaphoreHandle_t _mutex;
    static void              _ensureInit();
};

#endif
