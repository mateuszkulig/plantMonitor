import asyncio
import logging
import struct
from dataclasses import dataclass
from bleak import BleakScanner, BleakClient
from bleak.backends.characteristic import BleakGATTCharacteristic

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# MAC adress of nrf chip
DEVICE_MAC = "E2:97:D5:93:F7:F3"

class UUIDs:
    UUID_TEMP = "96061f1a-2902-41e9-ba53-41d7af84d40a"
    UUID_AIRHUMID = "5a365339-8b28-488f-aaaf-b74b9c5d3590"
    UUID_LIGHT = "6e1175ab-30d0-48ee-915a-b02e898dafa4"
    UUID_SOILMOIST = "44f297e9-87d8-4fac-bdc6-2f6a2f9b4733"
    UUIDS = (UUID_TEMP, UUID_AIRHUMID, UUID_LIGHT, UUID_SOILMOIST)

@dataclass(init=False)
class SensorMeasurements:
    temperature: float
    airHumidity: float
    lightLevel: float
    soilMoisture: float

sm = SensorMeasurements()
callCounter = 0

def printMeasurements():
    logger.info("")
    logger.info(f"\t{sm.temperature=} C")
    logger.info(f"\t{sm.airHumidity=}% RH")
    logger.info(f"\t{sm.lightLevel=} lux")
    logger.info(f"\t{sm.soilMoisture=}% RH")
    logger.info("")

def notificationHandler(characteristic: BleakGATTCharacteristic, data: bytearray):
    global callCounter
    global sm

    [decodedDouble] = struct.unpack("d", data)
    
    logger.info(f"Recieved data from UUID {characteristic.uuid}: {decodedDouble}")

    match characteristic.uuid:
        case UUIDs.UUID_TEMP:
            sm.temperature = decodedDouble
        case UUIDs.UUID_AIRHUMID:
            sm.airHumidity = decodedDouble
        case UUIDs.UUID_LIGHT:
            sm.lightLevel = decodedDouble
        case UUIDs.UUID_SOILMOIST:
            sm.soilMoisture = round(decodedDouble * 100, 2)
    
    callCounter += 1
    if not (callCounter % 4):
        printMeasurements()

def disconnectHandler(client: BleakClient):
    logger.info("Disconnected")

async def startNotifications(client: BleakClient):
    for uuid in UUIDs.UUIDS:
        await client.start_notify(uuid, notificationHandler)

async def stopNotifications(client: BleakClient):
    for uuid in UUIDs.UUIDS:
        await client.stop_notify(uuid)

async def main():
    logger.info("Searching for device...")
    dev = await BleakScanner.find_device_by_address(DEVICE_MAC)
    if (dev is None):
        logger.error("Cannot find the device")
        return
    logger.info(f"Found device: {dev.name}")
    
    logger.info("Press Ctrl+C to stop")
    async with BleakClient(DEVICE_MAC, disconnectHandler) as client:
        await startNotifications(client)
        try:
            await asyncio.Future()
        except (asyncio.exceptions.CancelledError):
            await stopNotifications(client)

if __name__ == "__main__":
    asyncio.run(main())