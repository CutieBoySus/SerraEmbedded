C++
#include <Arduino.h>
#include "Config.h"
#include "SystemState.h"

SystemState sysState;

void setup() {
    Serial.begin(115200);
    delay(2000); // Attesa enumerazione USB-CDC su ESP32-S3

    Serial.println("\n=== SerraEmbedded Boot ===");
    if (psramFound()) {
        Serial.printf("PSRAM OK! Totale: %d bytes\n", ESP.getPsramSize());
    } else {
        Serial.println("ATTENZIONE: PSRAM non rilevata!");
    }
}

void loop() {
    Serial.println("ESP32-S3 operativo...");
    delay(3000);
}