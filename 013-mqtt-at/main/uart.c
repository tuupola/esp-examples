
#include "uart.h"

static const char* TAG = "uart";

void uart_init()
{
    ESP_LOGI(TAG, "Init UART.");

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    ESP_ERROR_CHECK(
        uart_param_config(UART_NUM_2, &uart_config)
    );
    ESP_ERROR_CHECK(
        uart_driver_install(
            UART_NUM_2,
            UART_RX_BUF_SIZE,
            UART_TX_BUF_SIZE,
            UART_QUEUE_SIZE,
            UART_QUEUE_HANDLE,
            UART_INTR_FLAGS
        )
    );

    ESP_ERROR_CHECK(
        uart_set_pin(
            UART_NUM_2,
            GPIO_NUM_17, // M5Stack TX SIM800l module pins
            GPIO_NUM_16, // M5Stack RX SIM800l module pins
            UART_PIN_NO_CHANGE,
            UART_PIN_NO_CHANGE
        )
    );
}

char *uart_read_line(uart_port_t uart_port) {
    static char line[255];
    char *ptr = line;

    while (true) {

        int16_t bytes_read = uart_read_bytes(
            uart_port,
            (unsigned char *)ptr,
            1,
            portMAX_DELAY
        );

        if (1 == bytes_read) {
            if ('\n' == *ptr) {
                //ptr++;
                *ptr = '\0';
                return line;
            }
            ptr++;
        }
    }
}

void uart_task(void *params)
{
    ESP_LOGI(TAG, "Start UART task.");
    while(1) {
        char *line = uart_read_line(UART_NUM_2);
        ESP_LOGD(TAG, "%s", line);
        vTaskDelay(100 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}
