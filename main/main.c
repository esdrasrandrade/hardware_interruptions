#include <stdio.h>
#include "driver/gpio.h" 
#include "esp_rom_sys.h" 
#include "esp_system.h" 
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h" 
#include "rom/ets_sys.h" 

#define BLINK_GPIO GPIO_NUM_35 /* Definição do pino de saída para o LED */
#define GPIO_INPUT_IO_0 0 /* Definição do pino de entrada, GPIO INPUT */
#define GPIO_INPUT_PIN_SEL (1ULL << GPIO_INPUT_IO_0) /* Definição do pino de entrada, GPIO INPUT */
#define ESP_INTR_FLAG_DEFAULT 0 /*"Canal" de interrupção padrão*/

static void IRAM_ATTR gpio_isr_handler(void *arg) { 
  ets_printf("GPIO interrupt fired!\n"); 
} 

void app_main(void){
    gpio_reset_pin(BLINK_GPIO); 
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT); 
    gpio_config_t io_conf = {}; 
    io_conf.intr_type = GPIO_INTR_ANYEDGE; 
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL; 
    io_conf.mode = GPIO_MODE_INPUT; 
    io_conf.pull_up_en = 1; 
    gpio_config(&io_conf); 
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void *)GPIO_INPUT_IO_0);
    
    int cnt = 0; 
    while (1) { 
        gpio_set_level(BLINK_GPIO, cnt % 2); 
        vTaskDelay(500 / portTICK_PERIOD_MS); 
        cnt++; 
    }
}
