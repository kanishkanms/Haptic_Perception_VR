//Authors:
//Kanishkan M S (ME19B192)
//Ashish Kumar Shroti (ME19B083)


#include "I2Cdev.h"                          // Library for I2C communication
#include "MPU6050_6Axis_MotionApps20.h"       // Library for MPU6050 sensor data acquisition
#include "MPU6050.h"                         // Library for MPU6050 sensor data acquisition
#include "Wire.h"                            // Library for I2C communication
#include <WiFi.h>                            // Library for WiFi communication
#include <WebSocketsServer.h>                // Library for WebSockets communication

// Create a WebSocket server object listening on port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Set the WiFi SSID and password
char * ssid = "Fruitseye";
char * password = "kanimyd514";

// Create an MPU6050 object
MPU6050 mpu;

// Define some constants
#define INTERRUPT_PIN 2                       // Pin for interrupt signal from MPU6050
#define OUTPUT_READABLE_QUATERNION

// MPU control/status vars
bool dmpReady = false;                       // Set true if DMP init was successful
uint8_t mpuIntStatus;                        // Holds actual interrupt status byte from MPU
uint8_t devStatus;                           // Return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;                         // Expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64];                      // FIFO storage buffer

// Orientation/motion vars
Quaternion q;                                 // Quaternion container

// ***********INTERRUPT*********
// This function is called when the interrupt pin goes high (MPU data is ready)
volatile bool mpuInterrupt = false;           // Indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}

void setup() {
  Wire.begin();                               // Initialize the I2C bus
  Serial.begin(115200);                        // Initialize the Serial port

  WiFi.begin(ssid, password);                  // Connect to WiFi network
  while(WiFi.status()!=WL_CONNECTED) {         // Wait for connection
   Serial.print(".");
   delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();                           // Start the WebSocket server

  mpu.initialize();                            // Initialize the MPU6050
  pinMode(INTERRUPT_PIN, INPUT);               // Set the interrupt pin as input

  //****DMP****
  devStatus = mpu.dmpInitialize();             // Initialize the DMP

  // Set gyro and accelerometer offsets
  mpu.setXGyroOffset(108);
  mpu.setYGyroOffset(36);
  mpu.setZGyroOffset(48);
  mpu.setXAccelOffset(-1585);
  mpu.setYAccelOffset(3611);
  mpu.setZAccelOffset(765);

  // Check if DMP initialization was successful
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    // Turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // Enable Arduino interrupt detection
    Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
    Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
    Serial.println(F(")..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
}

void loop() {
  // Continuously run this code block

  // Keep the WebSocket connection alive
  webSocket.loop();

  // Check if there is a new data packet available in the MPU FIFO buffer
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {

    // Get the quaternion data from the data packet
    mpu.dmpGetQuaternion(&q, fifoBuffer);

    // Convert quaternion data to a string
    String dt = "";
    dt += q.w;
    dt += ',';
    dt += q.x;
    dt += ',';
    dt += q.y;
    dt += ',';
    dt += q.z;

    // Send the quaternion data to all connected WebSocket clients
    webSocket.broadcastTXT(dt.c_str(), dt.length());

    // Print the quaternion data to the serial monitor
    Serial.println(dt.c_str());
  }

  // Wait for 50 milliseconds before checking for the next data packet
  delay(50);
}
