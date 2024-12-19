#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

/**
 * @brief Struct holding pair of light and proximity values returned by sensor
 * 
 */
struct lightAndProximityValues {
    uint16_t light;
    uint16_t proximity;
};

#endif  // AMBIENT_LIGHT_H
