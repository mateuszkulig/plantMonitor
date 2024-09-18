#include <zephyr/kernel.h>
#include "external_sensors/external_sensors.h"


int main(void)
{
	struct temperatureAndHumidityValues tempHumid = {0};
	checkAllSensors();
	readTemperatureAndHumidity(&tempHumid);
	printk("Temperature: %.1f C\n", tempHumid.temperature);
	printk("Humidity: %.1f RH%%\n", tempHumid.humidity);
	return 0;
}
