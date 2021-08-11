#include "adc.h"

QueueHandle_t dac_i2sQueue; // i2s writer queue
WAVFileReader wav;

void i2sWriterTask(void *param) {
  uint16_t availableBytes = 0;
  uint16_t buffer_position = 0;
  uint16_t frames[FRAMES_TO_SEND * CANALES]; // Por dos canales
  i2s_event_t evt;
  // wait for some data to be requested
  while (true) {
    if (xQueueReceive(dac_i2sQueue, &evt, portMAX_DELAY) == pdPASS) {
      if (evt.type == I2S_EVENT_TX_DONE) {
        size_t bytesWritten = 0;
        do {
          if (availableBytes == 0) {
            // get some frames from the wave file - a frame consists of a 16 bit left and right sample
            wav.getFrames(frames, FRAMES_TO_SEND);
            // how many bytes do we now have to send
            availableBytes = SAMPLE_SIZE_SIZE;
            // reset the buffer position back to the start
            buffer_position = 0;
          }
          // do we have something to write?
          if (availableBytes > 0) {
            // write data to the i2s peripheral
            i2s_write(I2S_NUM_0, buffer_position + (uint8_t *)frames, availableBytes, &bytesWritten, portMAX_DELAY);
            availableBytes -= bytesWritten;
            buffer_position += bytesWritten;
          }
        } while (bytesWritten > 0);
      }
    }
  }
}

void i2s_configure() {
  wav.init("/sample.wav");
  i2s_config_t i2sConfig = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = 8000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 64
  };

  if (i2s_driver_install(I2S_NUM_0, &i2sConfig, 4, &dac_i2sQueue) != ESP_OK) {
    Serial.println("Fail DAC");
    while (1) {}
  }

  // enable the DAC channels
  i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
  // clear the DMA buffers
  i2s_zero_dma_buffer(I2S_NUM_0);
  // start a task to write samples to the i2s peripheral
  TaskHandle_t writerTaskHandle;
  xTaskCreate(i2sWriterTask, "i2s Writer Task", 4096, NULL, 1, &writerTaskHandle);
}
