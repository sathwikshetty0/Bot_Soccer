// =================================================================
// Project: Bot Soccer - ESP-NOW Transmitter
// =================================================================

#include <esp_now.h>
#include <WiFi.h>

// IMPORTANT: REPLACE THIS WITH YOUR BOT'S MAC ADDRESS
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Define the pins for the joystick
const int JOYSTICK_X_PIN = 34; // VRx
const int JOYSTICK_Y_PIN = 35; // VRy

// Define a data structure to send the joystick values
// This structure must be identical on the receiver side
typedef struct struct_message {
  int x;
  int y;
} struct_message;

// Create a variable of that structure type
struct_message joystickData;

// ESP-NOW peer info
esp_now_peer_info_t peerInfo;

// Callback function that is called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback function
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read the raw analog values from the joystick (0-4095)
  int rawX = analogRead(JOYSTICK_X_PIN);
  int rawY = analogRead(JOYSTICK_Y_PIN);

  // Map the raw values from 0-4095 to -255 to 255
  // Note: Your joystick's resting value might not be exactly 2048.
  // You might need to calibrate these values for perfect centering.
  int mappedX = map(rawX, 0, 4095, -255, 255);
  int mappedY = map(rawY, 0, 4095, 255, -255); // Y is inverted on my joystick

  // Implement a "deadzone" to prevent drift when joystick is at rest
  if (abs(mappedX) < 20) {
    mappedX = 0;
  }
  if (abs(mappedY) < 20) {
    mappedY = 0;
  }

  // Load the mapped values into our data structure
  joystickData.x = mappedX;
  joystickData.y = mappedY;

  // Send the message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &joystickData, sizeof(joystickData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
    delay(50);
}
