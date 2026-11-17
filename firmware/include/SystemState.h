
// MEMORIA CONDIVISA: Contiene lo stato attuale di tutti i sensori e attuatori in tempo reale
#pragma once
#include <Arduino.h>

enum class SystemMode {
    AUTO,
    MANUAL,
    SAFE_LOCKOUT // Attivo se serbatoio vuoto o errore critico sensori
};

struct SystemState {
    SystemMode mode = SystemMode::AUTO;

    // Dati Sensori (aggiornati da Degia e Membro 3)
    float soilMoisture[6] = {0};  // Percentuale 0-100%
    int soilRaw[6] = {0};         // Valore grezzo ADC
    float airTemp = 0.0f;         // °C
    float airHumidity = 0.0f;     // %
    float waterLevel = 0.0f;      // % serbatoio
    bool isTankEmpty = false;     // True se waterLevel < MIN_TANK_LEVEL_PERCENT

    // Stato Attuatori (aggiornato da Alex)
    bool pumpActive[3] = {false, false, false};
    uint8_t fanPwm = 0;           // 0-255
};