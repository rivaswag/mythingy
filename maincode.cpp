#include <ESPWiFi.h>
#include <WiFiClient.h>
#include "AerisObservations.h"
//done
AerisObservations::AerisObservations() {

}

void AerisObservations::updateObservations(AerisObservationsData *observations, String clientId, String clientSecret, String location) {
  doUpdate(observations, "/observations/closest?p=" + location + "&client_id=" + clientId + "&client_secret=" + clientSecret);
}

void AerisObservations::doUpdate(AerisObservationsData *observations, String path) {
  unsigned long lostTest = 10000UL;
  unsigned long lost_do = millis();

  this->observations = observations;
  JsonStreamingParser parser;
  parser.setListener(this);
  Serial.printf("[HTTP] Requesting resource at http://%s:%u%s\n", host.c_str(), port, path.c_str());


  while (client.connected() || client.available()) {
      if (client.available()) {
        if ((millis() - lost_do) > lostTest) {
          Serial.println("[HTTP] lost in client with a timeout");
          client.stop();
          ESP.restart();
        }
        c = client.read();
        if (c == '{' || c == '[') {
          isBody = true;
        }
    if (isBody) {
          parser.parse(c);
        }
      }//yes
