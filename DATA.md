# Dies ist keine offizielle Dokumentation

Die hier aufgeführten Daten sind aus dem Code entnommen
*Alle Daten werden als String übertragen*

## Original Daten bis 7.5.2026

- **version**
    Firmware Version
- **ip**
    aktuelle IP Adresse
- **state**
    Betriebsstatus
    ```
    enum State {
        // Flags (8 bits)
        Suspended,
        Suspending,
        ShortCircuit,

        // Outputs
        DCCOperations,
        DCCService,
        DECUPZpp,
        DECUPZsu,
        MDUZpp,
        MDUZsu,
        ZUSI,

        // USB protocols
        ULF_DCC_EIN,
        ULF_MDU_EIN,
        ULF_SUSIV2,

        // System
        OTA,
    }
    ```
- **ssid**
    WiFi SSID (bei LAN leer)
- **mdns**
    selbst definierter Host Name
- **rssi**
    WiFi Empfangsstärke
- **voltage**
    Versorgungsspannung
- **current**
    Stromstärke

## Original Daten ab 7.5.2026

- **version**
    Firmware Version
- **ip**
    aktuelle IP Adresse
- **state**
    Betriebsstatus
    - suspended
    - DCCOperations
    - OTA
    - ... (weitere sind nicht bekannt) 
- **ssid**
    WiFi SSID (bei LAN leer)
- **mdns**
    selbst definierter Host Name
- **rssi**
    WiFi Empfangsstärke
- **vcc_voltage**
    ??? Spannung
- **supply_voltage**
    Versorgungsspannung
- **current**
    Stromstärke


## Eigene - erweiterte - Daten

- **is_restarting**
    true / false: Versuch einen Reboot anzuzeigen, zB nach OTA Update
- **eth_connected**
    LAN ist in Benutzung
- **wifi_status**
    - DISCONNECTED - keine Verbindung
    - STA_CONNECTED - Verbunden
    - AP_ACTIVE - AccessPoint ist aktiv
    - AP_CONNECTED - Ein Client ist mit dem AccessPoint verbunden
- **temperature**
    CPU Temperatur
