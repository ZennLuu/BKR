#include <ESP32Servo.h>
#include <WiFi.h>
#include <esp_now.h>

#define F1 13
#define F2 12
#define F3 14
#define F4 27
#define F5 26

Servo s1;
Servo s2;
Servo s3;
Servo s4;
Servo s5;

typedef struct struct_message {
  int s1 = 0;
  int s2 = 0;
  int s3 = 0;
  int s4 = 0;
  int s5 = 0;
} struct_message;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  struct_message receivedData;
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  int a1 = map(receivedData.s1, 0, 100, 0, 180);
  int a2 = map(receivedData.s2, 0, 100, 0, 180);
  int a3 = map(receivedData.s3, 0, 100, 0, 180);
  int a4 = map(receivedData.s4, 0, 100, 0, 180);
  int a5 = map(receivedData.s5, 0, 100, 0, 180);
  s1.write(a1);
  s2.write(a2);
  s3.write(a3);
  s4.write(a4);
  s5.write(a5);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  s1.attach(F1);
  s2.attach(F2);
  s3.attach(F3);
  s4.attach(F4);
  s5.attach(F5);

  delay(5000);
}

void loop() {
  // s1.write(180);
  // s2.write(180);
  // s3.write(180);
  // s4.write(180);
  // s5.write(180);
  // delay(10000);

  // s1.write(0);
  // s2.write(0);
  // s3.write(0);
  // s4.write(0);
  // s5.write(0);
  delay(10);
}
