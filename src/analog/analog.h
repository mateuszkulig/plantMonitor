#ifndef ANALOG_H
#define ANALOG_H

#include <zephyr/kernel.h>

/**
 * @brief   Map each sensor connected to ADC to index of that it's using
 * 
 */
enum sensorChannelIndex {
    SENSOR_MOISTURE = 0
};


/**
 * @brief           Read the value from analog sensor with predefined calibration and configuration, and convert it to
 *                  percentage relative to maximum output
 * 
 * @param sensor    Sensor to read from
 * @param buffer    Addres of buffer to write to
 * @return int8_t   0 on succes, error code on error
 */
int8_t getAnalogSensorValue(enum sensorChannelIndex sensor, double *buffer);


/**
 * @brief           Initialize Analog-to-Digital converter along with all of its channels
 * 
 * @return int8_t   0 on succes, error code on error
 */
int8_t initilizeAdc();

#endif  // ANALOG_H
