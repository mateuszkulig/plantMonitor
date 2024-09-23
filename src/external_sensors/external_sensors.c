#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(extSensors, LOG_LEVEL_DBG);

/**
 * @brief List of all externally connected sensors from devicetree
 * 
 */
static const struct device *deviceList[] = {
    DEVICE_DT_GET(DT_ALIAS(dht11)),
};

/**
 * @brief Count of externally connected sensors
 * 
 */
static const uint8_t deviceCount = sizeof(deviceList) / sizeof(deviceList[0]);


uint8_t checkAllSensors() {
    uint8_t errorCount = 0;

    LOG_DBG("Number of connected external devices: %d", deviceCount);
    for (int i=0; i<deviceCount; ++i) {
        if (!device_is_ready(deviceList[i])) {
            LOG_ERR("Device is not ready: %s", deviceList[i]->name);
            errorCount++;
        }
    }

    return errorCount;
}


struct device *getDeviceByName(char *name) {
    for (size_t i=0; i<deviceCount; ++i) {
        if (name == deviceList[i]->name) {
            return deviceList[i];
        }
    }

    return NULL;
}
