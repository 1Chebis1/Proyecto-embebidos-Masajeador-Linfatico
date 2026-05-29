    #include "MANAGER.h"
#include "LOGBUFFER.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

Manager::Manager(
    adc_oneshot_unit_handle_t adc_handle,
    uint8_t pinFsrDer, // Only the right FSR remains
    uint8_t pinMotor,
    ledc_channel_t motorChannel,
    uint8_t ledV, uint8_t ledA, uint8_t ledR,
    uint8_t buzzer
): _fsrDer(pinFsrDer, adc_handle),
   _motor(pinMotor, motorChannel),
   _ui(ledV, ledA, ledR, buzzer),
   _uart(UART_NUM_0, 1, 3)
{
    _vMin                = 0;
    _vMax                = 3300; // Updated to 3.3V (3300mV) as our 100% baseline
    _motorIntensidad     = 0;
    _ticksEnZonaOptima   = 0;
    _histIdx             = 0;
    _zonaAnterior        = nullptr;
    _estabilidadAnterior = nullptr;
    for (int i = 0; i < 10; i++) {
        _historial[i] = 0.0f;
    }
}

void Manager::init() {
    _fsrDer.init();
    _motor.init();
    _ui.init();
    _uart.init();

    _motor.setIntensity(_motorIntensidad);
    _webserver.init();
}

Manager::~Manager() {}

void Manager::setConfig(int vMinMv, int vMaxMv, uint8_t intensidadFija) {
    _vMin            = vMinMv;
    _vMax            = vMaxMv;
    _motorIntensidad = intensidadFija;
}

void Manager::update() {
    
    if (_webserver.getLower()){
        _motorIntensidad = 100;
        _motor.setIntensity(_motorIntensidad);
    }
    else if (_webserver.getMiddle()){
        _motorIntensidad = 30;
        _motor.setIntensity(_motorIntensidad);
    }
    else if (_webserver.getUpper()){
        _motorIntensidad = 5;
        _motor.setIntensity(_motorIntensidad);
    }

    if (_webserver.getStop()){
        _motor.setIntensity(0);
    }
    if (_webserver.getResume()){
        _motor.setIntensity(_motorIntensidad);
    }

    // Read only the right FSR
    int vDer = _fsrDer.read();
    if (vDer == -1) {
        LogBuffer::log("WARN", "FSR der fallo en lectura ADC, omitiendo ciclo");
        return;
    }

    float porcentaje = 0.0f;

    if (vDer > _vMin) {
        float valorMedido = (float)(vDer - _vMin);
        float rangoTotal  = (float)(_vMax - _vMin);
        porcentaje = (valorMedido / rangoTotal) * 100.0f;
    }

    if (porcentaje < 0.0f) porcentaje = 0.0f;
    if (porcentaje > 100.0f) {
        LogBuffer::log("WARN", "Presion saturada: vMedido=%d vMax=%d", vDer, _vMax);
        porcentaje = 100.0f;
    }

    /* update rolling history */
    _historial[_histIdx] = porcentaje;
    _histIdx = (_histIdx + 1) % 10;

    /* zona óptima timer: 3.0V (90.9%) to 3.25V (98.5%) */
    if (porcentaje >= 22.0f && porcentaje <= 99.9f) {
        _ticksEnZonaOptima++;
    }
    uint32_t totalSegundos = _ticksEnZonaOptima / 10;
    uint32_t minutos       = totalSegundos / 60;
    uint32_t segundos      = totalSegundos % 60;

    /* determine zone and log transitions */
    const char* zona;
    if (porcentaje < 22.0f)       zona = "insuficiente";
    else if (porcentaje <= 99.9f) zona = "optimo";
    else                          zona = "excesivo";

    if (_zonaAnterior == nullptr) {
        LogBuffer::log("INFO", "Zona inicial: %s (%.1f%%)", zona, porcentaje);
    } else if (strcmp(zona, _zonaAnterior) != 0) {
        LogBuffer::log("INFO", "Zona: %s -> %s (%.1f%%)", _zonaAnterior, zona, porcentaje);
    }
    _zonaAnterior = zona;

    char mensaje_uart[100];
    sprintf(mensaje_uart,
            "FSR Der: %d mV | Presion: %.1f%%\r\n",
            vDer, porcentaje);
    _uart.sendUart(mensaje_uart);

    if (porcentaje < 22.0f)       _ui.alertasLow();
    else if (porcentaje <= 99.9f) _ui.alertasOk();
    else                          _ui.alertasHigh();

    /* varianza sobre las últimas 10 muestras */
    float sumHist = 0.0f;
    for (int i = 0; i < 10; i++) sumHist += _historial[i];
    float mean = sumHist / 10.0f;

    float varianza = 0.0f;
    for (int i = 0; i < 10; i++) {
        float d = _historial[i] - mean;
        varianza += d * d;
    }
    varianza /= 10.0f;
    float stdDev = sqrtf(varianza);

    const char* estabilidad;
    if      (stdDev < 5.0f)  estabilidad = "alta";
    else if (stdDev < 15.0f) estabilidad = "media";
    else                     estabilidad = "baja";

    if (_estabilidadAnterior == nullptr) {
        LogBuffer::log("INFO", "Estabilidad inicial: %s (stdDev=%.2f)", estabilidad, stdDev);
    } else if (strcmp(estabilidad, _estabilidadAnterior) != 0) {
        LogBuffer::log("INFO", "Estabilidad: %s -> %s (stdDev=%.2f)",
                       _estabilidadAnterior, estabilidad, stdDev);
    }
    _estabilidadAnterior = estabilidad;

    _webserver.setData(porcentaje, vDer, minutos, segundos, estabilidad);
}
    _webserver.setData(porcentaje, vPromedio, minutos, segundos, estabilidad);
}
