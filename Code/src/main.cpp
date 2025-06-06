#include "main.h"
#include "network.tpp"

void subscribeMQTT()
{
  mqtt.subscribe("/pc_led", [](const String &payload, const size_t size)
                 {
                  if(payload=="on")
                    digitalWrite(LED, LOW);
                  if (payload == "off")
                    digitalWrite(LED, HIGH); });

  mqtt.subscribe("/pc", [](const String &payload, const size_t size)
                 {
                  if(payload=="get")
                  {
                    mqtt.publish("/pc_state", digitalRead(IN_POWER_LED)==HIGH? "off":"on");
                    return;
                  }
                  if(payload!="power"&& payload != "reset") return;
                  uint8 pin=payload=="power"? OUT_PWR: OUT_RST;
                  digitalWrite(pin, HIGH);
                  delay(500);
                  digitalWrite(pin, LOW); });
}

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(OUT_PWR, OUTPUT);
  pinMode(OUT_RST, OUTPUT);
  digitalWrite(LED, LOW);
  digitalWrite(OUT_PWR, LOW);
  digitalWrite(OUT_RST, LOW);

#ifdef DEBUG
  Serial.begin(115200);
#endif

  while (!initWiFi())
    yield();
  DEBUG_PRINTLN(WiFi.localIP());

  ArduinoOTA.begin();

  while (!syncNTPTime(30000))
  {
    for (int i = 0; i < 1000; i++) // Approximate 1-second wait
    {
      ArduinoOTA.handle(); // Allow OTA updates
      yield();             // Keep Wi-Fi alive and prevent watchdog resets
    }
  }
  DEBUG_PRINTLN(F(" ntp synced!"));

  client.setReconnectInterval(15000);     // Increase to 15 sec to avoid rapid reconnects
  client.enableHeartbeat(30000, 5000, 3); // Ping every 30 sec, wait 5 sec for pong

  client.beginSslWithCA(SERVER_URL, 443, SERVER_PATH, ENDPOINT_CA_CERT, "mqtt");

  mqtt.begin(client);
  mqtt.setKeepAliveSendInterval(30000); // Send keep-alive every 30 sec
  mqtt.setKeepAliveTimeout(60);         // Ensure it matches broker timeout

  while (!connectMQTT())
  {
    for (int i = 0; i < 1000; i++) // Approximate 1-second wait
    {
      ArduinoOTA.handle(); // Allow OTA updates
      yield();             // Keep Wi-Fi alive and prevent watchdog resets
    }
  }

  subscribeMQTT();

  digitalWrite(LED, HIGH);
}

void loop()
{

  ArduinoOTA.handle();
  mqtt.update(); // should be called

  if (!mqtt.isConnected())
  {
    connectMQTT();
    yield();
    subscribeMQTT();
  }
}