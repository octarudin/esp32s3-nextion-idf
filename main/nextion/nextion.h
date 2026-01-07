#pragma once
#include "driver/uart.h"
#include "esp_err.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Default UART configuration
#define NEXTION_UART_NUM UART_NUM_1
#define NEXTION_UART_BAUD 9600
#define NEXTION_BUF_SIZE 1024

// Event types dari Nextion
typedef enum {
    NEXTION_EVENT_TOUCH = 0x65,     // Tombol / slider / component touch
    NEXTION_EVENT_PAGE = 0x66,      // Page change
    NEXTION_EVENT_TIMER = 0x88,     // Timer event
    NEXTION_EVENT_OTHER              // Event lain / unknown
} nextion_event_type_t;

typedef struct {
    nextion_event_type_t type;
    uint8_t component_id;
    uint8_t event_value;
    uint16_t page_id;
} nextion_event_t;

// Callback type
typedef void (*nextion_event_cb_t)(nextion_event_t *event, void *arg);

typedef struct {
    uart_port_t uart_num;
    int tx_pin;
    int rx_pin;
    int baud_rate;
    nextion_event_cb_t callback;
    void *callback_arg;
} nextion_handle_t;

// Inisialisasi UART dan start listener task
esp_err_t nextion_init(nextion_handle_t *handle);

// Kirim perintah ke Nextion (otomatis menambahkan 0xFF 0xFF 0xFF)
esp_err_t nextion_send(nextion_handle_t *handle, const char *cmd);

// Daftarkan callback event
void nextion_set_callback(nextion_handle_t *handle, nextion_event_cb_t cb, void *arg);
