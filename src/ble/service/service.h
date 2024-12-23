#ifndef SERVICE_H
#define SERVICE_H

/**
 * @brief   Human readable service name, required to be a define as it is passed to macro
 * 
 */
#define SERVICE_NAME Plant

/**
 * @brief   Structure that holds all variables that BLE presents to client
 * 
 */
struct servedData {
    double temperature;
    double humidity;
    double light;
    double soilMoisture;
};

/**
 * @brief   Pair of index of attribute in SERVICE_NAME.attrs array and value that notify should pass
 *          to that attr
 * 
 */
struct attrIndexValuePair {
    uint8_t index;
    double *value;
};

#endif  // SERVICE_H
