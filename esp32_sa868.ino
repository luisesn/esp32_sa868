#include "WAVFileReader.h"
#include "adc.h"

#define PTT 18
#define LED 5

void setup() {
  pinMode(PTT, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(PTT, HIGH);
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  i2s_configure();
  //AT+DMOSETGROUP=GBW，TFV，RFV，Tx_CXCSS，SQ，Rx_CXCSS
  Serial2.println("AT+DMOSETGROUP=0,433.1250,433.1250,0000,0,0000");
}

void loop() {
  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
  }
  if (Serial.available()) {
    switch (Serial.read()) {
      case 'q':
        digitalWrite(PTT, LOW);
        digitalWrite(LED, LOW);
        break;
      case 'w':
        digitalWrite(PTT, HIGH);
        digitalWrite(LED, HIGH);
        break;
    }
  }
}
