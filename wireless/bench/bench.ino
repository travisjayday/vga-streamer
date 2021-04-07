#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFiClientSecure.h>

TFT_eSPI tft = TFT_eSPI();

const char* bssid     = "xfinityk2";     
const char* wifi_pass = "3593572466"; 

void setup() {
    Serial.begin(115200);               
    tft.init();  
    tft.setRotation(2); 
    tft.setTextSize(1); 
    tft.fillScreen(TFT_BLACK); 
    tft.setTextColor(TFT_GREEN, TFT_BLACK); 

    delay(100); 
    pinMode(PIN_1, INPUT_PULLUP);
    pinMode(PIN_2, INPUT_PULLUP);
    pinMode(25, OUTPUT); 
    digitalWrite(25,0);

    WiFi.begin(bssid, wifi_pass); 
    Serial.printf("Attempting to connect to %s\n", bssid);
    while (WiFi.status() != WL_CONNECTED && count < 12) {
        delay(500);
        Serial.print(".");
        count++;
    }
    delay(1000);

    if (WiFi.isConnected()) { 
        Serial.printf("%d:%d:%d:%d (%s) (%s)\n", WiFi.localIP()[3], WiFi.localIP()[2],
                      WiFi.localIP()[1], WiFi.localIP()[0],
                      WiFi.macAddress().c_str() , WiFi.SSID().c_str());
        delay(500);
    } else { 
        Serial.println("Failed to Connect :/  Going to restart");
        Serial.println(WiFi.status());
        ESP.restart(); 
    }
}

void loop() {

}
