#include <zephyr/kernel.h>
#include "external_sensors/external_sensors.h"
#include "analog/analog.h"


int main(void)
{
	struct temperatureAndHumidityValues tempHumid = {0};
	double 								soilMoisturePercentage = 0.0f;
	uint8_t								brokenSensors = 0;

	brokenSensors = checkAllSensors();
	if (brokenSensors) {
		printk("Error: %d sensor(s) not ready\n", brokenSensors);
		return 1;
	}

	if (initilizeAdc()) {
		printk("Error: could not initialize ADC\n");
		return 1;
	}

	if (readTemperatureAndHumidity(&tempHumid)) {
		printk("Error: cannot read temperature and humidity\n");
		return 1;
	} else {
		printk("Temperature: %.1f C\n", tempHumid.temperature);
		printk("Humidity: %.1f RH%%\n", tempHumid.humidity);
	}

	if (getAnalogSensorValue(SENSOR_MOISTURE, &soilMoisturePercentage)) {
		printk("Error: cannot get value from analog sensor\n");
		return 1;
	} else {
		printk("Soil moisture: %.1f RH%%\n", soilMoisturePercentage * 100);
	}

	return 0;
}
