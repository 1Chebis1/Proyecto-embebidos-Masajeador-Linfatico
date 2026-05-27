#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Fsr.h"
#include "LOGBUFFER.h"

Fsr::Fsr(uint8_t gpio, adc_oneshot_unit_handle_t shared_handle){
    _pin          = gpio;
    _adc_handle   = shared_handle;
    _cali_enabled = false;
    _channelValid = false;
    _channel      = ADC_CHANNEL_0; /* safe default; overwritten below if pin known */

    switch (_pin){
        case 32: _channel = ADC_CHANNEL_4; _channelValid = true; break;
        case 33: _channel = ADC_CHANNEL_5; _channelValid = true; break;
        case 34: _channel = ADC_CHANNEL_6; _channelValid = true; break;
        case 35: _channel = ADC_CHANNEL_7; _channelValid = true; break;
        case 36: _channel = ADC_CHANNEL_0; _channelValid = true; break;
        case 39: _channel = ADC_CHANNEL_3; _channelValid = true; break;
        default:
            LogBuffer::log("ERROR",
                "Pin %d no es un canal ADC1 valido (usar 32/33/34/35/36/39)",
                (int)_pin);
            break;
    }
}

Fsr::~Fsr(){
    if(_cali_enabled){
        adc_cali_delete_scheme_line_fitting(_cali_handle);
    }
}

void Fsr::init(){
    if (!_channelValid) {
        LogBuffer::log("ERROR", "FSR pin %d: init omitido, canal invalido", (int)_pin);
        return;
    }

    adc_oneshot_chan_cfg_t chan_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_oneshot_config_channel(_adc_handle, _channel, &chan_config);

    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    esp_err_t ret = adc_cali_create_scheme_line_fitting(&cali_config, &_cali_handle);

    _cali_enabled = (ret == ESP_OK);
    if (!_cali_enabled) {
        LogBuffer::log("WARN", "ADC calibracion no disponible en pin %d, usando valores crudos", (int)_pin);
    }
}

int Fsr::read() {
    if (!_channelValid) return -1;

    int raw_val = 0;
    int voltage = 0;

    esp_err_t ret = adc_oneshot_read(_adc_handle, _channel, &raw_val);

    if (ret != ESP_OK){
        return -1;
    }

    if (_cali_enabled) {
        adc_cali_raw_to_voltage(_cali_handle    , raw_val, &voltage); 
        return voltage; 
    }
    return raw_val;
}