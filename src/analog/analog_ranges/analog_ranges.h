#ifndef ANALOG_RANGES_H
#define ANALOG_RANGES_H

#include <zephyr/kernel.h>

/**
 * @brief   Minimum and maximum raw analog values recorded on sensor along with inverted flag, as some sensors give
 *          lower values for higher unit of measure
 * 
 */
struct analogSensorRange {
    uint16_t    min;
    uint16_t    max;
    bool        inverted;
};


/**
 * @brief                                   Get the structure containing minimum and maximum values of given sensor
 * 
 * @param sensor                            Analog sensor for which to get values
 * @return const struct analogSensorRange   Struct filled with appropriate range or zeroes if range is not specified
 */
struct analogSensorRange getSensorRange(enum sensorChannelIndex sensor);

#endif  // ANALOG_RANGES_H
