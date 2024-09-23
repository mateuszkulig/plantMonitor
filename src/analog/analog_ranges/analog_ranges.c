#include "../analog.h"
#include "analog_ranges.h"

/**
 * @brief   Measured range of moisture sensor in water / in air
 * 
 */
const struct analogSensorRange moistureSensorRange = {
    .min = 950,
    .max = 3400,
    .inverted = true
};


struct analogSensorRange getSensorRange(enum sensorChannelIndex sensor) {
    switch (sensor) {
        case SENSOR_MOISTURE:
            return moistureSensorRange;
        
        default:
            const struct analogSensorRange dummy = {0};
            return dummy;
    }
}
