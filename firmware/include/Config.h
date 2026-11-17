
// =========================================================================
// DIZIONARIO HARDWARE E PARAMETRI GLOBALI (Riferimento unico per tutto il team)
// Non modificare i pin GPIO senza avvisare gli altri membri!
// allo stesso tempo è provvisorio come parametri
// =========================================================================

#pragma once

// --- SENSORI TERRENO ---
#define PIN_SOIL_NATIVE_1   1   // ADC1_CH0 (immune al Wi-Fi)
#define PIN_SOIL_NATIVE_2   2   // ADC1_CH1 (immune al Wi-Fi)
#define PIN_I2C_SDA         48  // Bus I2C per ADS1115 (4 canali)
#define PIN_I2C_SCL         47
#define ADS1115_I2C_ADDR    0x48

// --- SENSORI CLIMA & SERBATOIO ---
#define PIN_DHT22           21
#define PIN_HCSR04_TRIG     45
#define PIN_HCSR04_ECHO     38  // OBBLIGATORIO partitore 5V -> 3.3V

// --- ATTUATORI MOSFET (12V) ---
#define PIN_PUMP_1          41
#define PIN_PUMP_2          42
#define PIN_PUMP_3          40
#define PIN_FAN_PWM         39

// --- SOGLIE E SICUREZZA ---
// --- Sono d'esempio quindi siete liberi di modificare questi valori ---

#define SOIL_DRY_THRESHOLD      35.0f   // Avvia irrigazione sotto il 35%
#define SOIL_WET_THRESHOLD      70.0f   // Ferma irrigazione al 70%
#define MAX_PUMP_DURATION_MS    8000    // Timeout sicurezza pompa: max 8 secondi
#define MIN_TANK_LEVEL_PERCENT  15.0f   // Blocca pompe se acqua < 15%
#define AIR_HUMIDITY_HIGH       85.0f   // Accende ventola anticondensa sopra 85%