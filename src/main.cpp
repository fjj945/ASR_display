#include <Arduino.h>
#include <OneButton.h>
#include <driver/i2s.h>

#define key     26
#define I2S_SD  32
#define I2S_WS  23
#define I2S_SCK 25

#define SAMPLE_RATE 16000

// 音频缓冲区大小
const size_t bufferSize = 1024;
uint8_t buffer[bufferSize];

void I2S_init();
size_t i2s_read(uint8_t* data, size_t size);
void setup(){
  Serial.begin(9600);
  I2S_init();
  i2s_start(I2S_NUM_0);
}

void loop(){
  i2s_read(buffer, bufferSize);
  
  delay(100);
  

}
void I2S_init(){
  i2s_config_t i2s_config_rx = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = bufferSize,
    .use_apll = false,
  };

  i2s_pin_config_t pin_config_rx = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config_rx, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config_rx);
  i2s_set_clk(I2S_NUM_0,SAMPLE_RATE,I2S_BITS_PER_SAMPLE_16BIT,I2S_CHANNEL_MONO);
}

size_t i2s_read(uint8_t* data, size_t size) {
  size_t bytesRead;
  esp_err_t result = i2s_read(I2S_NUM_0, data, size, &bytesRead, portMAX_DELAY);
  Serial.println(bytesRead);
  if (result == ESP_OK)
  {
    // Lectura del buffer de datos I2S
    int16_t muestras = bytesRead / 2;
    if (muestras > 0) {
      float promedio = 0;
      for (int16_t i = 0; i < muestras; ++i) {
        promedio += (buffer[i]);
      }

      // Promedio de los datos leídos
      promedio /= muestras;

      // Gráfica de los datos obtenidos
      Serial.println(promedio);
  }
  }else{
    Serial.println("read data error");
  }
    return bytesRead;
}
