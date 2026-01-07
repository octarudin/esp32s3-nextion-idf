/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "nextion/nextion.h"

#define TXD_PIN (CONFIG_EXAMPLE_UART_TXD)
#define RXD_PIN (CONFIG_EXAMPLE_UART_RXD)

void my_nextion_cb(nextion_event_t *event, void *arg) 
{
    switch (event->type) {
        case NEXTION_EVENT_TOUCH:
            printf("Tombol/Slider %d, value=%d\n", event->component_id, event->event_value);
            break;
        case NEXTION_EVENT_PAGE:
            printf("Page berubah ke %d\n", event->page_id);
            break;
        case NEXTION_EVENT_TIMER:
            printf("Timer event dari component %d\n", event->component_id);
            break;
        default:
            break;
    }
}

void app_main(void)
{
    nextion_handle_t nex = {
        .uart_num = NEXTION_UART_NUM,
        .tx_pin = TXD_PIN,
        .rx_pin = RXD_PIN,
        .baud_rate = NEXTION_UART_BAUD
    };

    nextion_init(&nex);
    nextion_set_callback(&nex, my_nextion_cb, NULL);

    // Kirim perintah contoh
    nextion_send(&nex, "t0.txt=\"Hello Callback\"");

}
