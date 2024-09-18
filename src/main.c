#include <zephyr/kernel.h>
#include "external_sensors/external_sensors.h"


int main(void)
{
	struct temperatureAndHumidityValues tempHumid = {0};
	uint8_t								brokenSensors = 0;

	brokenSensors = checkAllSensors();
	if (brokenSensors) {
		printk("Error: %d sensor(s) not ready\n", brokenSensors);
		return -1;
	}

	if (readTemperatureAndHumidity(&tempHumid)) {
		printk("Error: cannot read temperature and humidity\n");
		return -1;
	} else {
		printk("Temperature: %.1f C\n", tempHumid.temperature);
		printk("Humidity: %.1f RH%%\n", tempHumid.humidity);
	}

	return 0;
}
