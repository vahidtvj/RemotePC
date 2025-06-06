#include "main.h"

bool initWiFi(uint32_t timeout = 20000)
{
    // WiFi.disconnect();
    // delay(1000);
    WiFi.setAutoReconnect(true);
    // WiFi.persistent(true);
    ulong t = millis();
    DEBUG_PRINT(F("Connecting to WiFi"));
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - t >= timeout)
            return false;
        delay(500);
        DEBUG_PRINT(F("."));
        yield();
    }
    return true;
}

bool syncNTPTime(uint32_t timeout = 10000)
{
    DEBUG_PRINTLN(F("Trying NTP"));
    if (WiFi.status() != WL_CONNECTED)
        return false;
    // if (!Ping.ping(googleDNS, 3))
    //   return;

    DEBUG_PRINTLN(F("Getting time from NTP"));
    configTime(12600, 0, "pool.ntp.org", "time.windows.com", "ntp1.ripe.net");

    uint32_t startMillis = millis(); // Store the start time
    while (time(nullptr) <= 100000)
    {
        if (millis() - startMillis >= timeout)
        {
            DEBUG_PRINTLN(F("NTP sync timed out."));
            return false; // Timeout reached
        }

        DEBUG_PRINT(F("."));
        for (int i = 0; i < 1000; i++) // Replace delay(1000)
        {
            ArduinoOTA.handle(); // Allow OTA updates
            yield();
        }
    }

    time_t tnow = time(nullptr);
    DEBUG_PRINTLN(String(ctime(&tnow)));

    isNTPSet = true; // Set the global flag to true
    return true;     // NTP sync successful
}

bool connectMQTT()
{
    if (WiFi.status() != WL_CONNECTED)
        return false;

    if (mqtt.isConnected())
        return true;

    DEBUG_PRINT(F("Heap before mqtt: "));
    DEBUG_PRINTLN(ESP.getFreeHeap());

    uint start = millis();

    DEBUG_PRINT(F("connecting to mqtt broker..."));
    while (!mqtt.connect(CLIENT_ID, CLIENT_USERNAME, CLIENT_PASSWORD))
    {
        DEBUG_PRINT(F("."));
        for (int i = 0; i < 1000; i++) // Replace delay(1000)
        {
            ArduinoOTA.handle(); // Allow OTA updates
            yield();
        }
        if (millis() - start > 10000 && !client.isConnected())
        {
            DEBUG_PRINTLN(F("WebSocketsClient disconnected"));
            return false;
        }
    }
    yield();

    DEBUG_PRINTLN(F(" connected!"));
    // return true;
    DEBUG_PRINT(F("Heap after mqtt: "));
    DEBUG_PRINTLN(ESP.getFreeHeap());

    return true;
}