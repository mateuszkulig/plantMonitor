#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/adc.h>
#include "analog.h"
#include "analog_ranges/analog_ranges.h"

LOG_MODULE_REGISTER(analog, LOG_LEVEL_DBG);


static const struct device *adc = DEVICE_DT_GET(DT_ALIAS(adc0));
static const struct adc_channel_cfg channel_cfgs[] = {
    DT_FOREACH_CHILD_SEP(DT_ALIAS(adc0), ADC_CHANNEL_CFG_DT, (,))
};
static const uint8_t adcChannelCount = ARRAY_SIZE(channel_cfgs);


int8_t readAnalogSensor(enum sensorChannelIndex channel, uint16_t *buffer) {
    const uint8_t   adcSamples = 10;
    const uint8_t   adcResolution = 12;
    const uint8_t   oversamplingValue = 8;
    uint16_t        adcReadBuffer[adcSamples][adcChannelCount];
    int8_t          rc = 0;
    int32_t         totalValue = 0;

    const struct adc_sequence_options options = {
		.extra_samplings = adcSamples - 1,
		.interval_us = 0,
	};

	struct adc_sequence sequence = {
		.buffer = adcReadBuffer,
		.buffer_size = sizeof(adcReadBuffer),
		.resolution = adcResolution,
		.options = &options,
        .oversampling = oversamplingValue,
	};

    sequence.channels |= BIT(channel_cfgs[channel].channel_id);

    rc = adc_read(adc, &sequence);
    if (rc) {
        LOG_ERR("Could not read from ADC");
        return rc;
    }

    for (size_t channelIdx=0; channelIdx<adcChannelCount; ++channelIdx) {
        for (size_t sampleIdx = 0; sampleIdx<adcSamples; ++sampleIdx) {
            totalValue += adcReadBuffer[sampleIdx][channelIdx];
        }
    }
    *buffer = totalValue / adcSamples;

    return rc;
}


int8_t getAnalogSensorValue(enum sensorChannelIndex sensor, double *buffer) {
    const struct analogSensorRange  range = getSensorRange(sensor);
    uint16_t                        readBuffer;
    int8_t                          rc = 0;
    double                          percentage;

    rc = readAnalogSensor(sensor, &readBuffer);
    if (rc) {
        LOG_ERR("Failed to read value from analog sensor");
        return rc;
    }

    LOG_DBG("Read raw adc value: %d", readBuffer);
    
    if (readBuffer < range.min) {
        *buffer = range.inverted ? 1 : 0;
    } else if (readBuffer > range.max) {
        *buffer = range.inverted ? 0 : 1;
    } else {
        percentage = ((double)readBuffer - range.min) / (range.max - range.min);
        *buffer = range.inverted ? 1 - percentage : percentage;
    }

    return rc;
}


int8_t initilizeAdc() {
    int8_t  rc = 0;

    if (!device_is_ready(adc)) {
		LOG_ERR("ADC controller device is not ready");
		return -ENXIO;
	}

    for (size_t i=0; i<adcChannelCount; ++i) {
        rc = adc_channel_setup(adc, &channel_cfgs[i]);
		if (rc) {
			LOG_ERR("Could not setup channel #%d (%d)", i, rc);
			return -rc;
        }
    }

    LOG_DBG("ADC initialized with %d channel(s)", adcChannelCount);

    return 0;
}
