#include "advertising.h"
#include "uuid.h"
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/logging/log.h>


LOG_MODULE_REGISTER(ble, LOG_LEVEL_DBG);


/**
 * @brief		Callback launched on opening BLE connection
 * 
 * @param conn 	Struct holding data of opened connection
 * @param err 	0 for success, error number otherwise
 */
static void bleConnectedCallback(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		LOG_DBG("Connection failed: %d", err);
		return;
	}
	LOG_DBG("Connected");
}


/**
 * @brief			Callback launched on closing BLE connection
 * 
 * @param conn 		Struct holding data of opened connection
 * @param reason	Disconnect reason
 */
static void bleDisconnectedCallback(struct bt_conn *conn, uint8_t reason)
{
	LOG_DBG("Disconnected: %d", reason);
}


/**
 * @brief	Define callbacks to BLE events, must be global as it is a define wrapper
 * 
 */
BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected        = bleConnectedCallback,
	.disconnected     = bleDisconnectedCallback,
};


/**
 * @brief 			Initialize BLE and start advertising
 * 
 * @return int8_t 	0 on success, negative error number on fail
 */
int8_t initBle() {
	int8_t rc = 0;

	LOG_DBG("Enabling BLE");
	rc = bt_enable(NULL);
	if (rc) {
		LOG_ERR("Bluetooth init failed: %d", rc);
		return rc;
	}
	LOG_DBG("Bluetooth initialized");

	rc = bt_le_adv_start(BT_LE_ADV_CONN, advertisingData, ARRAY_SIZE(advertisingData),
			      scanResponseData, ARRAY_SIZE(scanResponseData));
	if (rc) {
		LOG_ERR("Failed to start advertising %d", rc);
		return rc;
	}
	LOG_DBG("Advertising started");

    return rc;
}

