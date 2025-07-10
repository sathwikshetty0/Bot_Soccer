// =================================================================
// Project: Bot Soccer - ESP-NOW Receiver (Attacker Bot)
// =================================================================

#include <esp_now.h>
#include <WiFi.h>

// Define pins for the BTS7960 Motor Driver
// Left Motor
#define M1_L_PWM 26 // Connect to L_PWM on BTS7960 for Motor 1
#define M1_R_PWM 25 // Connect to R_PWM on BTS7960 for Motor 1
// Right Motor
#define M2_L_PWM 33 // Connect to L_PWM on BTS7960 for Motor 2
#define M2_R_PWM 32 // Connect to R_PWM on BTS7960 for Motor 2

// PWM Properties
const int PWM_FREQ = 1000; // PWM frequency in Hz
const int PWM_RESOLUTION = 8; // 8-bit resolution (0-255)

// PWM Channels
const int M1_L_PWM_CH = 0;
const int M1_R_PWM_CH = 1;
const int M2_L_PWM_CH = 2;
const int M2_R_PWM_CH = 3;

// Define a data structure to receive the joystick values
// This structure must be identical on the transmitter side
typedef struct struct_message {
  int x;
  int y;
} struct_message;

// Create a variable of that structure type
struct_message joystickData;

// Safety timeout - if no data is received for this duration (ms), stop motors
unsigned long lastRecvTime = 0;
const unsigned int timeoutDuration = 500; // 0.5 seconds

// Callback function that is called when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&joystickData, incomingData, sizeof(joystickData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("X: ");
  Serial.print(joystickData.x);
  Serial.print("  Y: ");
  Serial.println(joystickData.y);

  lastRecvTime = millis(); // Update the time of last reception
}

// Function to control the motors based on speed values (-255 to 255)
void driveMotors(int leftSpeed, int rightSpeed) {
  // Control Left Motor
  if (leftSpeed > 0) {
    ledcWrite(M1_R_PWM_CH, leftSpeed);
    ledcWrite(M1_L_PWM_CH, 0);
  } else {
    ledcWrite(M1_R_PWM_CH, 0);
    ledcWrite(M1_L_PWM_CH, -leftSpeed);
  }

  // Control Right Motor
  if (rightSpeed > 0) {
    ledcWrite(M2_R_PWM_CH, rightSpeed);
    ledcWrite(M2_L_PWM_CH, 0);
  } else {
    ledcWrite(M2_R_PWM_CH, 0);
    ledcWrite(M2_L_PWM_CH, -rightSpeed);
  }
}


void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // IMPORTANT: Print the MAC address of this receiver board
  Serial.println("Bot Receiver Ready. My MAC Address is:");
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback function
  esp_now_register_recv_cb(OnDataRecv);

  // --- Configure PWM for Motors ---
  // Left Motor
  ledcSetup(M1_L_PWM_CH, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(M1_L_PWM, M1_L_PWM_CH);
  ledcSetup(M1_R_PWM_CH, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(M1_R_PWM, M1_R_PWM_CH);
  // Right Motor
  ledcSetup(M2_L_PWM_CH, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(M2_L_PWM, M2_L_PWM_CH);
  ledcSetup(M2_R_PWM_CH, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(M2_R_PWM, M2_R_PWM_CH);

  // Set initial motor speed to 0
  driveMotors(0, 0);
}

void loop() {
  // --- Mixing Logic ---
  // Convert X and Y joystick values to left and right motor speeds
  int y = joystickData.y;
  int x = joystickData.x;

  int leftSpeed = y - x;
  int rightSpeed = y + x;

  // Constrain the speeds to the valid PWM range (-255 to 255)
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  
  // --- Safety Timeout ---
  // If no data received for a while, stop the motors
  if (millis() - lastRecvTime > timeoutDuration) {
    leftSpeed = 0;
    rightSpeed = 0;
    joystickData.x = 0; // Reset data to prevent sudden movement on reconnect
    joystickData.y = 0;
  }

  // Update the motor speeds
  driveMotors(leftSpeed, rightSpeed);

  delay(20);
}
