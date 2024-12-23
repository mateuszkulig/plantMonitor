#ifndef UUID_H
#define UUID_H

#include <zephyr/bluetooth/uuid.h>

/**
 * @brief   Raw and encoded generated BLE UUID values
 *          Uses defines as these macros output array elements
 * 
 */
#define BLE_UUID_PRIMARY_VALUE      BT_UUID_128_ENCODE(0x4f12114b, 0x0c5d, 0x4e95, 0x8c3b, 0xe001e598c304)
#define BLE_UUID_TEMP_VALUE         BT_UUID_128_ENCODE(0x96061f1a, 0x2902, 0x41e9, 0xba53, 0x41d7af84d40a)
#define BLE_UUID_AIRHUMID_VALUE     BT_UUID_128_ENCODE(0x5a365339, 0x8b28, 0x488f, 0xaaaf, 0xb74b9c5d3590)
#define BLE_UUID_LIGHT_VALUE        BT_UUID_128_ENCODE(0x6e1175ab, 0x30d0, 0x48ee, 0x915a, 0xb02e898dafa4)
#define BLE_UUID_SOILMOIST_VALUE    BT_UUID_128_ENCODE(0x44f297e9, 0x87d8, 0x4fac, 0xbdc6, 0x2f6a2f9b4733)
#define BLE_UUID_PRIMARY            BT_UUID_DECLARE_128(BLE_UUID_PRIMARY_VALUE)
#define BLE_UUID_TEMP               BT_UUID_DECLARE_128(BLE_UUID_TEMP_VALUE)
#define BLE_UUID_AIRHUMID           BT_UUID_DECLARE_128(BLE_UUID_AIRHUMID_VALUE)
#define BLE_UUID_LIGHT              BT_UUID_DECLARE_128(BLE_UUID_LIGHT_VALUE)
#define BLE_UUID_SOILMOIST          BT_UUID_DECLARE_128(BLE_UUID_SOILMOIST_VALUE)

#endif  // UUID_H
