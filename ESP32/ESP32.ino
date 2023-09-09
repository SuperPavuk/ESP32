#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <TFT_eSPI.h>

const char* ssid = "freewifi";
const char* password = NULL;

AsyncWebServer server(80);
TFT_eSPI tft = TFT_eSPI();

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  if(!SPIFFS.begin(true)){
    tft.println("An error occurred while mounting SPIFFS");
    return;
  }

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String screenshot = "/screenshot" + String(millis()) + ".bmp";
    tft.saveScreenBMP(screenshot.c_str());
    request->send(SPIFFS, screenshot, "image/bmp");
  });

  server.begin();

  tft.println("Access Point started");
  tft.println("SSID: " + String(ssid));
  tft.println("IP address: " + IP.toString());

  String screenshot = "/screenshot" + String(millis()) + ".bmp";
  tft.saveScreenBMP(screenshot.c_str());
}

void loop() {
}