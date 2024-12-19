#ifndef EXTERNAL_SENSORS_H
#define EXTERNAL_SENSORS_H

#include <zephyr/kernel.h>
#include "temp_humid/temp_humid.h"
#include "ambient_light/ambient_light.h"


/**
 * @brief           Check if all sensors signal that they are ready to use
 * 
 * @return int8_t   Count of not ready devices
 */
uint8_t checkAllSensors();


/**
 * @brief                   Get the pointer to device struct by its name property
 * 
 * @param name              Name of the device
 * @return struct device*   Pointer to device or NULL if it does not exist
 */
struct device *getDeviceByName(char *name);


/**
 * @brief           Read the data from DHT sensor and store its values in buffer
 * 
 * @param buffer    Buffer to fill with readings
 * @return int8_t   0 for success, negative error number for failure
 */
int8_t readTemperatureAndHumidity(struct temperatureAndHumidityValues *buffer);


/**
 * @brief           Read the data from VCNL4040 sensor and store its values in buffer
 * 
 * @param buffer    Buffer to fill with values
 * @return int8_t   0 for success, negative error number for failure
 */
int8_t readAmbientLightValue(struct lightAndProximityValues *buffer);


#endif  // EXTERNAL_SENSORS_H
