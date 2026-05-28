#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "MANAGER.h"

#define PIN_FSR_IZQ 1
#define PIN_FSR_DER 2
#define PIN_MOTOR 39
#define PIN_LED_VERDE 42
#define PIN_LED_AMARILLO 38
#define PIN_LED_ROJO 40
#define PIN_BUZZER 33

extern "C" void app_main(){
    adc_oneshot_unit_handle_t adc1_handle = NULL; 
    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = ADC_UNIT_1;
    
    esp_err_t err = adc_oneshot_new_unit(&init_config, &adc1_handle); 

    if (err != ESP_OK || adc1_handle == NULL) {
        while(1){
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    Manager device(
        adc1_handle, PIN_FSR_DER, 
        PIN_MOTOR, LEDC_CHANNEL_0, 
        PIN_LED_VERDE, PIN_LED_AMARILLO, PIN_LED_ROJO, 
        PIN_BUZZER
    );

    device.setConfig(400, 3100, 0); 
    device.init(); 

    while(1){
        device.update();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
