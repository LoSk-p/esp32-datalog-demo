#include <vector>
#include <string>

#include <Arduino.h>
#include <EEPROM.h>
#include <Defines.h>
#include <RpcRobonomics.h>

#ifdef ESP32_MODEL
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif

// WiFi credentials and private keys are hidden in Private.h file
// pub keys are derived on ctor of RobonomicsRpc
// need to have derived or generated PUB_OWNER_KEY, SS58_ADR, SS58_DEVICE_ADR
// i.e. by subkey inspect "some mnemonics ..." --network robonomics --scheme ed25519

//#define DUMMY_KEYS
#include "Private.h"

uint64_t id_counter = 0; 
uint64_t coins_count = 100000; 

void setup() {
  Serial.begin(115200);
 
  WiFi.begin(STASSID, STAPSK);
  Serial.printf ("Trying to connected to SSID: %s \n", STASSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected and get IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
  Serial.print("My Robonomics address is ");
  Serial.println(SS58_ADR);
  Serial.println("Type the message for datalog record");

}

void loop () {
    if ((WiFi.status() == WL_CONNECTED)) { 
        WiFiClient client;
        if(Serial.available()) {
            String record = Serial.readStringUntil('\n');
            Serial.print("Got message for datalog record: ");
            Serial.println(record);
            Serial.println("RPC task run");
            RobonomicsRpc rpcProvider(client, URLRPC, PRIV_KEY, SS58_ADR, id_counter);
            RpcResult r = rpcProvider.DatalogRecord(record.c_str()); // id_counter as payload just for example
            id_counter = id_counter + 2;
            Serial.printf("[RPC] %ld %s\n\n", r.code, r.body.c_str());  
            delay(12000);
            Serial.println("Type the message for datalog record");
        }
    }
}
