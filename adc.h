#ifndef ADC_H
#define ADC_H

#include <Arduino.h>
#include <driver/i2s.h>
#include <driver/adc.h>
#include <soc/syscon_reg.h>
#include "freertos/queue.h"
#include "WAVFileReader.h"

#define FRAMES_TO_SEND 128
#define CANALES 2
#define SAMPLE_SIZE_SIZE FRAMES_TO_SEND*sizeof(uint16_t)*CANALES
#define ADC_CHANNEL ADC1_CHANNEL_0 // GPIO36

void i2s_configure();

#endif
