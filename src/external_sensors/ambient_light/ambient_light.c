#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include "../external_sensors.h"
#include "ambient_light.h"

LOG_MODULE_REGISTER(extSensAmbientLight, LOG_LEVEL_DBG);


int8_t readAmbientLightValue(struct lightAndProximityValues *buffer)
{
	const struct device *vcnl = getDeviceByName("vcnl4040@60");
	int8_t              rc = 0;
	struct sensor_value light = {0};
	struct sensor_value	proximity = {0};

	if (vcnl == NULL) {
        LOG_ERR("Failed to get the device");
        return -ENXIO;
    }

	rc = sensor_sample_fetch(vcnl);
    if (rc) {
        LOG_ERR("Sensor fetch failed: %d", rc);
        return rc;
    }

	rc = sensor_channel_get(vcnl, SENSOR_CHAN_PROX, &proximity);
	if (rc) {
        LOG_ERR("Sensor channel get failed: %d", rc);
        return rc;
    }
	rc = sensor_channel_get(vcnl, SENSOR_CHAN_LIGHT, &light);
	if (rc) {
        LOG_ERR("Sensor channel get failed: %d", rc);
        return rc;
    }

	buffer->proximity = (uint16_t)proximity.val1;
	buffer->light = (uint16_t)light.val1;

	LOG_DBG("Read ambient light and proximity: %d, %d",
			buffer->light, buffer->proximity);

	return rc;
}
