#ifndef __FSR_H__
#define __FSR_H__

#include <stdint.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

class Fsr
{
private:
    uint8_t _pin;
    adc_channel_t _channel;
    adc_oneshot_unit_handle_t _adc_handle;
    adc_cali_handle_t _cali_handle;
    bool _cali_enabled;
    bool _channelValid;
    float _lastPressure;
public:
    Fsr(uint8_t gpio, adc_oneshot_unit_handle_t shared_handle);
    ~Fsr();
    void init();
    int read();
};

#endif 
