#include "nextion.h"


static void nextion_task(void *arg) {
    nextion_handle_t *handle = (nextion_handle_t *)arg;
    uint8_t buf[NEXTION_BUF_SIZE];
    int idx = 0;

    while (1) {
        int len = uart_read_bytes(handle->uart_num, &buf[idx], 1, 10 / portTICK_PERIOD_MS);
        if (len <= 0) continue;

        idx += len;

        // Cek event minimal 4 bytes (tombol, slider, page)
        if (idx >= 4) {
            nextion_event_t event;
            event.type = NEXTION_EVENT_OTHER;
            event.component_id = 0;
            event.event_value = 0;
            event.page_id = 0;

            if (buf[0] == 0x65 && idx >= 4) { // Touch event
                event.type = NEXTION_EVENT_TOUCH;
                event.component_id = buf[1];
                event.event_value = buf[2];
                if (handle->callback) handle->callback(&event, handle->callback_arg);
                memmove(buf, buf + 4, idx - 4);
                idx -= 4;
            } else if (buf[0] == 0x66 && idx >= 4) { // Page event
                event.type = NEXTION_EVENT_PAGE;
                event.page_id = buf[1];
                if (handle->callback) handle->callback(&event, handle->callback_arg);
                memmove(buf, buf + 4, idx - 4);
                idx -= 4;
            } else if (buf[0] == 0x88 && idx >= 4) { // Timer event
                event.type = NEXTION_EVENT_TIMER;
                event.component_id = buf[1];
                if (handle->callback) handle->callback(&event, handle->callback_arg);
                memmove(buf, buf + 4, idx - 4);
                idx -= 4;
            }
            // Event lain akan ditangani di iterasi berikutnya
        }
    }
}

esp_err_t nextion_init(nextion_handle_t *handle) {
    if (!handle) return ESP_ERR_INVALID_ARG;

    uart_config_t uart_config = {
        .baud_rate = handle->baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    ESP_ERROR_CHECK(uart_driver_install(handle->uart_num, NEXTION_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(handle->uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(handle->uart_num, handle->tx_pin, handle->rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Start listener task
    xTaskCreate(nextion_task, "nextion_task", 4096, handle, 10, NULL);

    return ESP_OK;
}

esp_err_t nextion_send(nextion_handle_t *handle, const char *cmd) {
    if (!handle || !cmd) return ESP_ERR_INVALID_ARG;

    size_t cmd_len = strlen(cmd);
    uart_write_bytes(handle->uart_num, cmd, cmd_len);
    uint8_t end[3] = {0xFF, 0xFF, 0xFF};
    uart_write_bytes(handle->uart_num, (const char *)end, 3);
    return ESP_OK;
}

void nextion_set_callback(nextion_handle_t *handle, nextion_event_cb_t cb, void *arg) {
    if (!handle) return;
    handle->callback = cb;
    handle->callback_arg = arg;
}
