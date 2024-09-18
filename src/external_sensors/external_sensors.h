#ifndef EXTERNAL_SENSORS_H
#define EXTERNAL_SENSORS_H

#include <zephyr/kernel.h>
#include "temp_humid/temp_humid.h"


/**
 * @brief   Check if all sensors signal that they are ready to use
 * 
 * @return  int8_t Count of not ready devices
 */
uint8_t checkAllSensors();


/**
 * @brief                   Get the pointer to device struct by its name property
 * 
 * @param name              Name of the device
 * @return struct device*   Pointer to device or NULL if it does not exist
 */
struct device *getDeviceByName(char *name);


int8_t readTemperatureAndHumidity(struct temperatureAndHumidityValues *buffer);

#endif  // EXTERNAL_SENSORS_H
