#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SDA_PIN D6
#define SCL_PIN D5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* SSID = "Hand client";
const char* Password = "12345678";

uint8_t receiverMAC[] = { 0xD0, 0xEF, 0x76, 0xEC, 0x0F, 0x80 };

ESP8266WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />

  <title>ESP32 Web Control</title>
  <script>
    function sendPost(param, value) {
      var xhr = new XMLHttpRequest();
      xhr.open("POST", "/post", true);
      xhr.setRequestHeader(
        "Content-Type",
        "application/x-www-form-urlencoded"
      );
      xhr.send(param + "=" + value);
    }
  </script>
</head>
<body>
  <h1 style="text-align: center;">ESP32 Web Control</h1>
    <button  style="width:90%; height:70px; margin-left: 5%;" onclick="sendPost('button', '1')">Click Me</button><br><br>
    <input style="width:90%; height: 70px; margin-left: 5%;" type="range" min="0" max="100" oninput="sendPost('slider1', this.value)">
    <input style="width:90%; height:70px; margin-left: 5%;" type="range" min="0" max="100" oninput="sendPost('slider2', this.value)">
    <input style="width:90%; height:70px; margin-left: 5%;" type="range" min="0" max="100" oninput="sendPost('slider3', this.value)">
    <input style="width:90%; height:70px; margin-left: 5%;" type="range" min="0" max="100" oninput="sendPost('slider4', this.value)">
    <input style="width:90%; height:70px; margin-left: 5%;" type="range" min="0" max="100" oninput="sendPost('slider5', this.value)">
</body>
</html>
  )rawliteral";


typedef struct struct_message {
  int s1 = 0;
  int s2 = 0;
  int s3 = 0;
  int s4 = 0;
  int s5 = 0;
} struct_message;

struct_message dataToSend;

void OnSent(uint8_t* mac_addr, uint8_t sendStatus) {
  Serial.print("Send Status: ");
  Serial.println(sendStatus == 0 ? "Success" : "Fail");
  delay(10);
}

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handlePost() {
  if (server.hasArg("button")) {
    Serial.println("Button clicked!");
  }
  if (server.hasArg("slider1")) {
    int sliderValue = server.arg("slider1").toInt();
    Serial.printf("Slider value: %d\n", sliderValue);
    dataToSend.s1 = sliderValue;
    esp_now_send(receiverMAC, (uint8_t*)&dataToSend, sizeof(dataToSend));
  }
  if (server.hasArg("slider2")) {
    int sliderValue = server.arg("slider2").toInt();
    Serial.printf("Slider value: %d\n", sliderValue);
    dataToSend.s2 = sliderValue;
    esp_now_send(receiverMAC, (uint8_t*)&dataToSend, sizeof(dataToSend));
  }
  if (server.hasArg("slider3")) {
    int sliderValue = server.arg("slider3").toInt();
    Serial.printf("Slider value: %d\n", sliderValue);
    dataToSend.s3 = sliderValue;
    esp_now_send(receiverMAC, (uint8_t*)&dataToSend, sizeof(dataToSend));
  }
  if (server.hasArg("slider4")) {
    int sliderValue = server.arg("slider4").toInt();
    Serial.printf("Slider value: %d\n", sliderValue);
    dataToSend.s4 = sliderValue;
    esp_now_send(receiverMAC, (uint8_t*)&dataToSend, sizeof(dataToSend));
  }
  if (server.hasArg("slider5")) {
    int sliderValue = server.arg("slider5").toInt();
    Serial.printf("Slider value: %d\n", sliderValue);
    dataToSend.s5 = sliderValue;
    esp_now_send(receiverMAC, (uint8_t*)&dataToSend, sizeof(dataToSend));
  }

  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SCL_PIN, SDA_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnSent);

  esp_now_add_peer(receiverMAC, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  WiFi.mode(WIFI_STA);
  WiFi.softAP(SSID, Password);
  IPAddress myIP = WiFi.softAPIP();

  Serial.print("IP Address: ");
  Serial.println(myIP);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("SSID: ");
  display.println(SSID);
  display.print("IP: ");
  display.println(myIP);
  display.display();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/post", HTTP_POST, handlePost);
  server.begin();
  Serial.println("Server started!");
}

void loop() {
  server.handleClient();
}