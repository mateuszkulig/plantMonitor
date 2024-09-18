#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include "../external_sensors.h"
#include "temp_humid.h"

LOG_MODULE_REGISTER(extSensTempHumid, LOG_LEVEL_DBG);


int8_t readTemperatureAndHumidity(struct temperatureAndHumidityValues *buffer) {
    const struct device *dht = getDeviceByName("dht11");
    int8_t              ret = 0;
    struct sensor_value temperature = {0};
    struct sensor_value humidity = {0};
    
    ret = sensor_sample_fetch(dht);
    if (ret) {
        LOG_ERR("Sensor fetch failed: %d", ret);
        return ret;
    }
    
    // FIXME: Weird looking code, maybe some better error handling would help
    ret = sensor_channel_get(dht, SENSOR_CHAN_AMBIENT_TEMP, &temperature);
    if (!ret) {
        ret = sensor_channel_get(dht, SENSOR_CHAN_HUMIDITY, &humidity);
    }
    if (ret) {
        LOG_ERR("Sensor channel get failed: %d", ret);
        return ret;
    }
    
    buffer->temperature = sensor_value_to_double(&temperature);
    buffer->humidity = sensor_value_to_double(&humidity);

    LOG_DBG("Read temperature and humidity: %.1f C, %.1f RH%%",
            buffer->temperature, buffer->humidity);

    return ret;
}
