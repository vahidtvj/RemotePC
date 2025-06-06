#pragma once
// #define DEBUG

#include <Arduino.h>
#include <ESP8266WiFi.h>
// #include <WiFi.h>
#include <WebSocketsClient.h> // include before MQTTPubSubClient.h
#include <MQTTPubSubClient.h>
#include <ArduinoOTA.h>

#include "secrets.h"

WiFiClientSecure webClient;
WebSocketsClient client;
MQTTPubSubClient mqtt;

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x) x
#define DEBUG_PRINTLN(x) x
#endif

bool isNTPSet = false;

#define LED LED_BUILTIN
#define OUT_PWR 12
#define OUT_RST 14
#define IN_POWER_LED 13

// ISRG Root X1
const char ENDPOINT_CA_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
**************************
-----END CERTIFICATE-----
)EOF";
