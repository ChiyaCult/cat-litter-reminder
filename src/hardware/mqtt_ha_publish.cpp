#include "mqtt_ha_publish.h"
#include "../domain/connect_act_disconnect.h"
#include "../domain/mqtt_publish.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <PubSubClient.h>
#include "secrets.h" // WIFI_SSID, WIFI_PASSWORD, MQTT_HOST, MQTT_PORT

namespace hardware {

namespace {

constexpr unsigned long kWifiConnectTimeoutMs = 15000;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

bool connectWifiAndMqtt() {
  Serial.print("[mqtt] connecting WiFi to \"");
  Serial.print(WIFI_SSID);
  Serial.println("\"...");  

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - start > kWifiConnectTimeoutMs) {
      Serial.println("[mqtt] WiFi connect timed out");
      return false;
    }
    delay(100);
  }
  Serial.print("[mqtt] WiFi connected, IP ");
  Serial.println(WiFi.localIP());

  // Start mDNS so that .local hostnames resolve before the broker connect.
  MDNS.begin("cat-litter-reminder");

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  if (!mqttClient.connect("cat-litter-reminder")) {
    Serial.print("[mqtt] broker connect failed, state=");
    Serial.println(mqttClient.state());
    return false;
  }
  Serial.println("[mqtt] broker connected");
  return true;
}

void disconnectMqttAndWifi() {
  mqttClient.disconnect();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("[mqtt] disconnected");
}

} // namespace

bool performMqttHaPublish(time_t lastCleanedUtc) {
  return domain::connectActDisconnect(
      connectWifiAndMqtt,
      domain::makeMqttPublishAction(
          lastCleanedUtc,
          [](const char *topic, const char *payload) -> bool {
            // retain=true so HA still has the value after a broker restart.
            bool ok = mqttClient.publish(topic, payload, /*retain=*/true);
            if (ok) {
              Serial.print("[mqtt] published to ");
              Serial.println(topic);
            } else {
              Serial.println("[mqtt] publish failed");
            }
            return ok;
          }),
      disconnectMqttAndWifi);
}

} // namespace hardware
