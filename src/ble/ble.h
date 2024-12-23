#ifndef BLE_H
#define BLE_H

/**
 * @brief   Enable the BLE module and start advertising with data from advertising.h
 * 
 */
int8_t initBle();

/**
 * @brief                   Update data served via BLE, and notify about value change if any client is
 *                          currently subscribed
 * 
 * @param newTempHumidity   Pair of temperature and humidity to set
 * @param newLight          Struct of light and proximity to set light only
 * @param newSoilMoisture   Percentage value of soil moisture
 * @return int8_t           0 on success, error number otherwise
 */
int8_t bleUpdateData(struct temperatureAndHumidityValues newTempHumidity, struct lightAndProximityValues newLight,
                     double newSoilMoisture);

#endif  // BLE_H
