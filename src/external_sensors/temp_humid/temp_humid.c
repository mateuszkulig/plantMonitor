#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include "../external_sensors.h"
#include "temp_humid.h"

LOG_MODULE_REGISTER(extSensTempHumid, LOG_LEVEL_DBG);


int8_t readTemperatureAndHumidity(struct temperatureAndHumidityValues *buffer) {
    const struct device *dht = getDeviceByName("dht22");
    int8_t              rc = 0;
    struct sensor_value temperature = {0};
    struct sensor_value humidity = {0};
    
    if (dht == NULL) {
        LOG_ERR("Failed to get the device");
        return -ENXIO;
    }

    rc = sensor_sample_fetch(dht);
    if (rc) {
        LOG_ERR("Sensor fetch failed: %d", rc);
        return rc;
    }
    
    // FIXME: Weird looking code, maybe some better error handling would help
    rc = sensor_channel_get(dht, SENSOR_CHAN_AMBIENT_TEMP, &temperature);
    if (!rc) {
        rc = sensor_channel_get(dht, SENSOR_CHAN_HUMIDITY, &humidity);
    }
    if (rc) {
        LOG_ERR("Sensor channel get failed: %d", rc);
        return rc;
    }
    
    buffer->temperature = sensor_value_to_double(&temperature);
    buffer->humidity = sensor_value_to_double(&humidity);

    LOG_DBG("Read temperature and humidity: %.1f C, %.1f RH%%",
            buffer->temperature, buffer->humidity);

    return rc;
}
