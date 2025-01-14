#include <zephyr/kernel.h>
#include "external_sensors/external_sensors.h"
#include "analog/analog.h"
#include "ble/ble.h"
#include "ble/service/service.h"

int main(void)
{
	struct temperatureAndHumidityValues tempHumid = {0};
	struct lightAndProximityValues		lightProxi = {0};
	double 								soilMoisturePercentage = 0.0f;
	uint8_t								brokenSensors = 0;
	const uint16_t						waitTime = 5;

	brokenSensors = checkAllSensors();
	if (brokenSensors) {
		printk("Error: %d sensor(s) not ready\n", brokenSensors);
		return 1;
	}

	if (initilizeAdc()) {
		printk("Error: could not initialize ADC\n");
		return 1;
	}

	if (initBle()) {
		printk("Error: cannot initialize BLE\n");
		return 1;
	}

	for (;;) {
		if (readTemperatureAndHumidity(&tempHumid)) {
			printk("Error: cannot read temperature and humidity\n");
			continue;
		} else {
			printk("Temperature: %.1f C\n", tempHumid.temperature);
			printk("Humidity: %.1f RH%%\n", tempHumid.humidity);
		}

		if (getAnalogSensorValue(SENSOR_MOISTURE, &soilMoisturePercentage)) {
			printk("Error: cannot get value from analog sensor\n");
			continue;
		} else {
			printk("Soil moisture: %.1f RH%%\n", soilMoisturePercentage * 100);
		}

		if (readAmbientLightValue(&lightProxi)) {
			printk("Error: cannot read light and proximity\n");
			continue;
		} else {
			printk("Ambient light: %d lux\n", lightProxi.light);
			printk("Proximity value: %d \n", lightProxi.proximity);
		}

		bleUpdateData(tempHumid, lightProxi, soilMoisturePercentage);

		k_sleep(K_SECONDS(waitTime));
	}

	return 0;
}
