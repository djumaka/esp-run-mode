#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
// #include <ConfigFile.h>

const int setupBtnPin = 0;
int setupBtnVal = HIGH;
int setupMode = 0; // 0 for regular, 1 for setup, 2 for OTA

const char* ssid = "Matrix Global N";
const char* password = "thereisnocowlevel";
//ConfigFile config;

void setup() {
  // Setup serial and button definiton of mode
  Serial.begin(115200);
  pinMode(setupBtnPin, INPUT);
  Serial.println("Booting...");

  // Determin working mode
  delay(1500);
  setupBtnVal = digitalRead(setupBtnPin);

  if(setupBtnVal == LOW){
    delay(2000);
    setupBtnVal = digitalRead(setupBtnPin);
    if(setupBtnVal == LOW) {
      setupMode = 2; // OTA mode
      Serial.println("Start OTA mode");
    } else {
      setupMode = 1; // AP mode
      Serial.println("Start SETUP mode");
    }
  } else {
    Serial.println("Start regular mode");
  }

  /*
   * execute mode setup
   */

  // Setup mode OTA
  if(setupMode == 2) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }

    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("myesp8266");

    // No authentication by default
    ArduinoOTA.setPassword((const char *)"123");

    ArduinoOTA.onStart([]() {
      Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();

    Serial.print("Ready at ");
    Serial.println(WiFi.localIP());

  } // END mode OTA
  else if (setupMode == 1) { // mode SETUP
    WiFi.softAP("node", "node123");
  } // END mode SETUP

}

void loop() {

  ArduinoOTA.handle();
}
