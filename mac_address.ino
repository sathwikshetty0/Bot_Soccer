#include <WiFi.h>

void setup() {
  Serial.begin(115200);

  // Set the device to Wi-Fi station mode to initialize the hardware
  WiFi.mode(WIFI_STA);
  
  // Print the MAC address to the Serial Monitor
  Serial.println("Reading MAC address...");
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // The loop is empty because we only need to get the address once.
}
