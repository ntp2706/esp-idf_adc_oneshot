#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_types.h"
#include "sdkconfig.h"
#include "esp_adc/adc_oneshot.h"

static const char *TAG = "ADC";

void app_main(void)
{
    esp_err_t ret;
    adc_oneshot_unit_handle_t adc_handle = NULL;

    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
        .clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &adc_handle));

    adc_channel_t channel = ADC_CHANNEL_0;
    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12,          
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, channel, &chan_cfg));

    while (1) {
        int raw = 0;
        ret = adc_oneshot_read(adc_handle, channel, &raw);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "ADC raw: %d", raw);
        } else {
            ESP_LOGW(TAG, "adc_oneshot_read failed: %s", esp_err_to_name(ret));
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    //clean up
    // adc_oneshot_del_unit(adc_handle);
}
