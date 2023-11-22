#include "esp_log.h"
#include "nvs_flash.h"

/* BLE */
#include "ble_spp_server.h"
#include "console/console.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_check.h"
#include "esp_sleep.h"
#include "host/ble_hs.h"
#include "host/ble_hs_adv.h"
#include "host/util/util.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

#define INPUT_PIN 23

RTC_DATA_ATTR int rf_calib_skipped = 0;
int count = 1;

struct ble_hs_adv_fields adv_fields;
struct ble_gap_adv_params adv_params;

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
    adv_fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

    adv_fields.uuids16 = (ble_uuid16_t[]){BLE_UUID16_INIT(BLE_SVC_SPP_UUID16)};
    adv_fields.num_uuids16 = 1;
    adv_fields.uuids16_is_complete = 1;

    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
}

static int ble_spp_server_gap_event(struct ble_gap_event *event, void *arg);

// we do not care events resulted from a round of advertising
static int ble_spp_server_gap_event(struct ble_gap_event *event, void *arg) {
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

static void main_poll() {
    esp_light_sleep_start();

    char str[40];
    if (count > 9)
        count = 0;
    snprintf(str, sizeof(str), "nimbleServer_%d1", count);

    init_adv_structs();

    ble_svc_gap_device_name_set(str);
    const char *name = ble_svc_gap_device_name();
    adv_fields.name = (uint8_t *)name;
    adv_fields.name_len = strlen(name);
    adv_fields.name_is_complete = 1;
    ble_gap_adv_set_fields(&adv_fields);

    ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER, &adv_params,
                      ble_spp_server_gap_event, NULL);
    vTaskDelay(20 / portTICK_PERIOD_MS);
    ble_gap_adv_stop();

    count++;

    while (gpio_get_level(INPUT_PIN)) {
        gpio_wakeup_enable(INPUT_PIN, GPIO_INTR_LOW_LEVEL);
        esp_light_sleep_start();
    }
    gpio_wakeup_enable(INPUT_PIN, GPIO_INTR_HIGH_LEVEL);
}

void app_main(void) {
    if (rf_calib_skipped++ == 0) {
        esp_deep_sleep(50000);
    }

    gpio_reset_pin(INPUT_PIN);
    gpio_set_direction(INPUT_PIN, GPIO_MODE_INPUT);
    gpio_wakeup_enable(INPUT_PIN, GPIO_INTR_HIGH_LEVEL);
    esp_sleep_enable_gpio_wakeup();

    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Initialize the NimBLE host configuration. */
    ble_hs_cfg.reset_cb = ble_spp_server_on_reset;
    ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;
    ble_hs_cfg.sm_io_cap = CONFIG_EXAMPLE_IO_TYPE;

    nimble_port_init();
    nimble_port_freertos_init(ble_spp_server_host_task);

    while (1) {
        main_poll();
    }
}