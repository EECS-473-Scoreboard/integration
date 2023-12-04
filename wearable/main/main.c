#include "esp_log.h"
#include "nvs_flash.h"

/* BLE */
#include "ble_spp_server.h"
//#include "host/ble_uuid.h"
#include "console/console.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_bt.h"
#include "esp_check.h"
#include "esp_sleep.h"
#include "host/ble_hs.h"
#include "host/ble_hs_adv.h"
#include "host/util/util.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

/* FreeRTOS */
#include "freertos/portmacro.h"
#include "freertos/projdefs.h"
#include "sdkconfig.h"

#define INPUT_PIN_B4 23
#define INPUT_PIN_B3 22
#define INPUT_PIN_B2 21
#define INPUT_PIN_B1 20
#define DEVICE_ID 1

uint8_t low4 = 1;
uint8_t low3 = 1;
uint8_t low2 = 1;
uint8_t low1 = 1;
RTC_DATA_ATTR int rf_calib_skipped = 0;

#define ADV_TIME_MS 4
#define ADV_POWER ESP_PWR_LVL_P15

struct ble_hs_adv_fields adv_fields;
struct ble_gap_adv_params adv_params;

TaskHandle_t main_thread;

static void init_adv_structs() {
    memset(&adv_fields, 0, sizeof(adv_fields));
    memset(&adv_params, 0, sizeof(adv_params));

    /* Advertise two flags:
     *     o Discoverability in forthcoming advertisement (general)
     *     o BLE-only (BR/EDR unsupported).
     */
    adv_fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;

    /* Indicate that the TX power level field should be included; have the
     * stack fill this value automatically.  This is done by assigning the
     * special value BLE_HS_ADV_TX_PWR_LVL_AUTO.
     */
    adv_fields.tx_pwr_lvl_is_present = 1;
    adv_fields.tx_pwr_lvl = esp_ble_tx_power_get(ESP_BLE_PWR_TYPE_ADV);

    adv_fields.uuids16 = (ble_uuid16_t[]){BLE_UUID16_INIT(BLE_SVC_SPP_UUID16)};
    adv_fields.num_uuids16 = 1;
    adv_fields.uuids16_is_complete = 1;

    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
}

static int adv_event_cb(struct ble_gap_event *event, void *arg) {
    if (event->type != BLE_GAP_EVENT_ADV_COMPLETE)
        return 0;

    xTaskNotifyGive(main_thread);
    return 0;
}

static void ble_spp_server_on_reset(int reason) { return; }

void ble_spp_server_host_task(void *param) {
    MODLOG_DFLT(INFO, "BLE Host Task Started");
    /* This function will return only when nimble_port_stop() is executed */
    nimble_port_run();

    nimble_port_freertos_deinit();
}

static void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt,
                                 void *arg) {
    return;
}
static void PinSetHigh(){
    if(low4 == 1){
        gpio_wakeup_enable(INPUT_PIN_B4, GPIO_INTR_HIGH_LEVEL);
        low4 = 0;
    }
    if(low3 == 1){
        gpio_wakeup_enable(INPUT_PIN_B3, GPIO_INTR_HIGH_LEVEL);
        low3 = 0;
    }
    if(low2 == 1){ 
        gpio_wakeup_enable(INPUT_PIN_B2, GPIO_INTR_HIGH_LEVEL);
        low2 = 0;
    }
    if(low1 == 1) {
        gpio_wakeup_enable(INPUT_PIN_B1, GPIO_INTR_HIGH_LEVEL);
        low1 = 0;
    }
}
static void PinSetLow(){
    if(gpio_get_level(INPUT_PIN_B4)==1){
        low4 = 1;
        gpio_wakeup_enable(INPUT_PIN_B4, GPIO_INTR_LOW_LEVEL);
    }
    if(gpio_get_level(INPUT_PIN_B3)==1){
        low3 = 1;
        gpio_wakeup_enable(INPUT_PIN_B3, GPIO_INTR_LOW_LEVEL);
    }
    if(gpio_get_level(INPUT_PIN_B2)==1){
        low2 = 1;
        gpio_wakeup_enable(INPUT_PIN_B2, GPIO_INTR_LOW_LEVEL);
    }
    if(gpio_get_level(INPUT_PIN_B1)==1){
        low1 = 1;
        gpio_wakeup_enable(INPUT_PIN_B1, GPIO_INTR_LOW_LEVEL);
    } 

}

static void main_poll() {
    PinSetHigh();
    nimble_port_stop();
    nimble_port_deinit();

    esp_light_sleep_start();

    nimble_port_init();
    nimble_port_freertos_init(ble_spp_server_host_task);
    PinSetLow();

    char str[40];
    uint32_t com = 1; //For now com is just a number 1 offset from the button nums
    if(low1) com = 2;
    else if(low2) com = 3;
    else if(low3) com = 4;
    else if(low4) com = 5;
    com = com | (DEVICE_ID << 16);
    char IdHC = (char)(((com>>24) & 0xFF) + '0');
    char IdLC = (char)(((com>>16) & 0xFF) + '0');
    char DataHC = (char)(((com >> 8) & 0xFF)+'0');
    char DataLC = (char)(((com) & 0xFF)+'0');
    snprintf(str, sizeof(str), "nimbleServer_%c%c%c%c#",IdHC,IdLC,DataHC,DataLC);
    init_adv_structs();
    //printf(str);

    ble_svc_gap_device_name_set(str);
    const char *name = ble_svc_gap_device_name();
    adv_fields.name = (uint8_t *)name;
    adv_fields.name_len = strlen(name);
    adv_fields.name_is_complete = 1;
    ble_gap_adv_set_fields(&adv_fields);

    ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, ADV_TIME_MS, &adv_params,
                      adv_event_cb, NULL);

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    esp_light_sleep_start();
}

void app_main(void) {
    if (rf_calib_skipped++ == 0) {
        esp_deep_sleep(50000);
    }

    gpio_reset_pin(INPUT_PIN_B4);
    gpio_set_direction(INPUT_PIN_B4, GPIO_MODE_INPUT);
    gpio_reset_pin(INPUT_PIN_B3);
    gpio_set_direction(INPUT_PIN_B3, GPIO_MODE_INPUT);
    gpio_reset_pin(INPUT_PIN_B2);
    gpio_set_direction(INPUT_PIN_B2, GPIO_MODE_INPUT);
    gpio_reset_pin(INPUT_PIN_B1);
    gpio_set_direction(INPUT_PIN_B1, GPIO_MODE_INPUT);
    esp_sleep_enable_gpio_wakeup();

    // enable low power light sleep
    // https://github.com/espressif/esp-idf/issues/11858
    // by the time of coding, a modification in esp lib must be made
    esp_sleep_cpu_retention_init();

    /* Initialize NVS — it is used to store PHY calibration data */
    nvs_flash_init();

    /* Initialize the NimBLE host configuration. */
    ble_hs_cfg.reset_cb = ble_spp_server_on_reset;
    ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;
    ble_hs_cfg.sm_io_cap = CONFIG_EXAMPLE_IO_TYPE;

    nimble_port_init();
    nimble_port_freertos_init(ble_spp_server_host_task);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ADV_POWER);

    main_thread = xTaskGetCurrentTaskHandle();
    while (1) {
        main_poll();
    }
}