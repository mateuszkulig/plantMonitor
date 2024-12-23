#ifndef ADVERTISING_H
#define ADVERTISING_H

#include <zephyr/bluetooth/bluetooth.h>
#include "uuid.h"

/**
 * @brief Name displayed on advertising and its size
 * 
 */
const char	btName[] = "Plant";
const int	btNameLength = sizeof(btName) / sizeof(char);

/**
 * @brief Data sent in advertising packets, includes name of device and flags
 * 
 */
const struct bt_data advertisingData[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, btName, btNameLength),
};

/**
 * @brief Data sent in scan response packets, includes UUID of service
 * 
 */
const struct bt_data scanResponseData[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BLE_UUID_PRIMARY_VALUE),
};

#endif	// ADVERTISING_H
