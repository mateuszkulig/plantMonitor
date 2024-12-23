#include "../../external_sensors/external_sensors.h"
#include "../uuid.h"
#include "service.h"
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>


LOG_MODULE_REGISTER(bleService, LOG_LEVEL_DBG);

/**
 * @brief   Global variable holding data served via BLE GATT
 * 
 */
static struct servedData bleData = {0};

/**
 * @brief   Counter checking how many characteristics currently are subscribed by clients
 *          This should rise by 4 for each client asking for notify
 * 
 */
static uint8_t notifyEnabled = 0;

/**
 * @brief   Map that matches service attribute index to value that should be passed to notify.
 *          Four attributes are present with one characteristic and config, and from lbs example
 *          we know that 3rd is the one we want to pass with call. Each new characteristic adds
 *          three more attributes, so 3 seems to be correct step when enumerating them
 * 
 */
static const struct attrIndexValuePair attrNotifyMap[] = {
    {.index = 2, .value = &bleData.temperature},
    {.index = 5, .value = &bleData.humidity},
    {.index = 8, .value = &bleData.light},
    {.index = 11, .value = &bleData.soilMoisture}
};


/**
 * @brief           Generic callback launched on read from BLE characteristic. It just logs
 *                  that read has happened, as no action is required when that happens
 * 
 * @param conn      Connection that requests the read
 * @param attr      Attribute that is being read
 * @param buf       Buffer for read result
 * @param len       Length of buffer
 * @param offset    Offset to start reading from
 * @return ssize_t  Either number of bytes read or error 
 */
static ssize_t bleDataReadCallback(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf,
                                   uint16_t len, uint16_t offset) {
    LOG_DBG("Characteristic read via BLE callback called");
    const char *value = attr->user_data;
    return bt_gatt_attr_read(conn, attr, buf, len, offset, value,
					 sizeof(*value));
}


/**
 * @brief       Callback launched on writing to BLE Client Characteristic Configuration 
 * 
 * @param attr  Data of GATT attribute
 * @param value Value that configuration is being set to
 */
static void characteristicConfigCallback(const struct bt_gatt_attr *attr,
				  uint16_t value)
{
	LOG_DBG("Notify option toggled: %d", value);
    if (value) {
        notifyEnabled++;
    } else if (notifyEnabled) {
        notifyEnabled--;
    } else {
        LOG_ERR("Something went wrong, notify count cannot be less than 0");
    }
}


/**
 * @brief   Define a service with 4 characteristics: temperature, humidity, light level and soil moisture
 * 
 */
BT_GATT_SERVICE_DEFINE(SERVICE_NAME,
    BT_GATT_PRIMARY_SERVICE(BLE_UUID_PRIMARY),
	BT_GATT_CHARACTERISTIC(BLE_UUID_TEMP,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, bleDataReadCallback, NULL, &bleData.temperature),
    BT_GATT_CCC(characteristicConfigCallback,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BLE_UUID_AIRHUMID,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, bleDataReadCallback, NULL, &bleData.humidity),
    BT_GATT_CCC(characteristicConfigCallback,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BLE_UUID_LIGHT,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, bleDataReadCallback, NULL, &bleData.light),
    BT_GATT_CCC(characteristicConfigCallback,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BLE_UUID_SOILMOIST,
			       BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
			       BT_GATT_PERM_READ, bleDataReadCallback, NULL, &bleData.soilMoisture),
    BT_GATT_CCC(characteristicConfigCallback,
		    BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)
    
);


/**
 * @brief           Send notification of value change for each changed attribute. Attribute nofify
 *                  map is used as attributes in service are not next to each other index-wise.
 * 
 * @return int8_t   0 on success, negative error number otherwise
 */
static int8_t bleNotifyAll() {
    int8_t  rc = 0;
    uint8_t attrIndex = 0;
    double  *attrValuePointer = NULL;

    for (size_t i=0; i<ARRAY_SIZE(attrNotifyMap); ++i) {
        attrIndex = attrNotifyMap[i].index;
        attrValuePointer = attrNotifyMap[i].value;
        rc = bt_gatt_notify(NULL, &SERVICE_NAME.attrs[attrIndex], attrValuePointer, sizeof(*attrValuePointer));
        if (rc) {
            LOG_ERR("Failed to notify attribute with index: %d", attrIndex);
            return rc;
        }
    }

    return rc;
}


int8_t bleUpdateData(struct temperatureAndHumidityValues newTempHumidity, struct lightAndProximityValues newLight,
                     double newSoilMoisture) {
    bleData.temperature = newTempHumidity.temperature;
    bleData.humidity = newTempHumidity.humidity;
    bleData.light = newLight.light;
    bleData.soilMoisture = newSoilMoisture;

    if (notifyEnabled) {
        LOG_DBG("Notifying about data change");
        return bleNotifyAll();
    }

    return 0;
}
