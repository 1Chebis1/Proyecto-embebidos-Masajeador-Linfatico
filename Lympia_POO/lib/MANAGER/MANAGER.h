#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "FSR.h"
#include "MOTOR.h"
#include "FEEDBACK.h"
#include "UART.h"
#include "WEBSERVER.h"

class Manager {
    private:
        Fsr _fsrDer; // Retained only the right FSR
        Motor _motor;
        FeedbackSystem _ui;
        Uart _uart;
        WebServer _webserver;

        int _vMin;
        int _vMax; 
        uint8_t _motorIntensidad;
        uint32_t    _ticksEnZonaOptima;
        float       _historial[10];
        uint8_t     _histIdx;
        const char* _zonaAnterior;
        const char* _estabilidadAnterior;

        
    public: 
        Manager(
            adc_oneshot_unit_handle_t adc_handle, 
            uint8_t pinFsrDer, // Removed the first FSR pin 
            uint8_t pinMotor, 
            ledc_channel_t motorChannel, 
            uint8_t ledV, uint8_t ledA, uint8_t ledR,
            uint8_t buzzer
        );
        ~Manager();

        void setConfig(int vMinMv, int vMaxMv, uint8_t intensidadFija); 
        void init();
        void update();
};

#endif
