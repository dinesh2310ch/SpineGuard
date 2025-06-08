// #include <Wire.h>
// #include <MPU6050_light.h>
// #include <ESP8266WiFi.h>
// #include <Firebase_ESP_Client.h>

// // WiFi credentials
// const char* ssid = "";
// const char* password = "";

// // Firebase project credentials
// #define API_KEY ""
// #define DATABASE_URL ""

// // Firebase objects
// FirebaseData fbdo;
// FirebaseConfig config;
// FirebaseAuth auth;

// MPU6050 mpu(Wire);
// unsigned long timer = 0;

// void setup() {
//   Serial.begin(115200);
//   Wire.begin();

//   // WiFi connect
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("\nWiFi connected.");

//   // Firebase init (no auth)
//   config.api_key = API_KEY;
//   config.database_url = DATABASE_URL;

//   // Skip user credentials (anonymous)
//   Firebase.begin(&config, &auth);
//   Firebase.reconnectWiFi(true);

//   // MPU6050 init
//   byte status = mpu.begin();
//   Serial.print("MPU6050 status: ");
//   Serial.println(status);
//   if (status != 0) {
//     Serial.println("MPU6050 failed. Check wiring.");
//     while (1);
//   }

//   Serial.println("Calibrating sensor...");
//   delay(1000);
//   mpu.calcOffsets();  // Calibrate
//   Serial.println("Calibration complete.");
// }

// void loop() {
//   mpu.update();

//   if (millis() - timer > 2000) {
//     float pitch = mpu.getAngleX();
//     float roll = mpu.getAngleY();

//     Serial.print("Pitch: ");
//     Serial.print(pitch);
//     Serial.print(" | Roll: ");
//     Serial.println(roll);

//     // Send data to Firebase
//     Firebase.RTDB.setFloat(&fbdo, "/posture/current/pitch", pitch);
//     Firebase.RTDB.setFloat(&fbdo, "/posture/current/roll", roll);

//     timer = millis();
//   }
// }



// #include <Wire.h>
// #include <MPU6050_light.h>
// #include <ESP8266WiFi.h>
// #include <Firebase_ESP_Client.h>
// #include <ESP8266Ping.h>
// #include <addons/TokenHelper.h>
// #include <addons/RTDBHelper.h>

// // WiFi credentials
// const char* ssid = "";
// const char* password = "";

// // Firebase project credentials
// #define API_KEY ""
// #define DATABASE_URL ""
// #define USER_EMAIL "" // Replace with your Firebase registered email
// #define USER_PASSWORD ""       // Replace with your Firebase registered password

// // Firebase objects
// FirebaseData fbdo;
// FirebaseConfig config;
// FirebaseAuth auth;

// MPU6050 mpu(Wire);
// unsigned long timer = 0;
// unsigned long sendDataPrevMillis = 0;

// bool initializeMPU6050() {
//   byte status = mpu.begin();
//   Serial.print("MPU6050 status: ");
//   Serial.println(status);
//   if (status != 0) {
//     Serial.println("MPU6050 failed. Retrying...");
//     return false;
//   }
//   Serial.println("Calibrating sensor...");
//   delay(1000);
//   mpu.calcOffsets();
//   Serial.println("Calibration complete.");
//   return true;
// }

// void setup() {
//   Serial.begin(115200);
//   delay(100); // Small delay to ensure Serial is ready
//   Wire.begin();

//   // WiFi connect
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi");
//   unsigned long ms = millis();
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print(".");
//     delay(300);
//     if (millis() - ms > 10000) {
//       Serial.println("\nWiFi connection timeout.");
//       break;
//     }
//   }
//   if (WiFi.status() == WL_CONNECTED) {
//     Serial.println("\nWiFi connected.");
//     Serial.print("IP Address: ");
//     Serial.println(WiFi.localIP());
//   } else {
//     Serial.println("\nFailed to connect to WiFi.");
//   }

//   // Test network reachability
//   Serial.println("Pinging Google DNS (8.8.8.8)...");
//   if (Ping.ping("8.8.8.8", 3)) {
//     Serial.println("Ping successful. Network is reachable.");
//   } else {
//     Serial.println("Ping failed. Network may be unreachable.");
//   }

//   // Check free heap memory
//   Serial.print("Free Heap: ");
//   Serial.print(ESP.getFreeHeap());
//   Serial.println(" bytes");

//   // Firebase init
//   Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

//   // Assign the API key (required)
//   config.api_key = API_KEY;

//   // Assign the user sign-in credentials
//   auth.user.email = USER_EMAIL;
//   auth.user.password = USER_PASSWORD;

//   // Assign the RTDB URL (required)
//   config.database_url = DATABASE_URL;

//   // Set the callback for token generation
//   config.token_status_callback = tokenStatusCallback; // From TokenHelper.h

//   // Set buffer sizes to prevent SSL connection issues
//   fbdo.setBSSLBufferSize(4096 /* Rx buffer size */, 1024 /* Tx buffer size */);
//   fbdo.setResponseSize(2048);

//   // Set timeout options
//   config.timeout.serverResponse = 10 * 1000;      // 10 seconds
//   config.timeout.networkReconnect = 10 * 1000;    // 10 seconds
//   config.timeout.socketConnection = 10 * 1000;    // 10 seconds

//   // Enable WiFi reconnection
//   Firebase.reconnectNetwork(true);

//   // Initialize Firebase
//   Serial.println("Initializing Firebase...");
//   Firebase.begin(&config, &auth);

//   // Wait for token generation (up to 10 seconds)
//   ms = millis();
//   while (!Firebase.ready() && (millis() - ms < 10000)) {
//     Serial.print(".");
//     delay(500);
//   }
//   if (Firebase.ready()) {
//     Serial.println("\nFirebase is ready.");
//   } else {
//     Serial.println("\nFirebase not ready after initialization. Check credentials, network, or Firebase rules.");
//   }

//   // MPU6050 init with retry
//   int retries = 3;
//   bool mpuInitialized = false;
//   for (int i = 0; i < retries; i++) {
//     if (initializeMPU6050()) {
//       mpuInitialized = true;
//       break;
//     }
//     delay(1000);
//   }
//   if (!mpuInitialized) {
//     Serial.println("Failed to initialize MPU6050 after retries. Check wiring.");
//   }
// }

// void loop() {
//   mpu.update();

//   if (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0) {
//     sendDataPrevMillis = millis();

//     float pitch = mpu.getAngleX();
//     float roll = mpu.getAngleY();

//     Serial.print("Pitch: ");
//     Serial.print(pitch);
//     Serial.print(" | Roll: ");
//     Serial.println(roll);

//     // Classify posture
//     String posture;
//     if (pitch >= -10 && pitch <= 10 && roll >= -10 && roll <= 10) {
//       posture = "good";
//     } else if ((pitch >= -20 && pitch <= -10) || (pitch >= 10 && pitch <= 20) ||
//                (roll >= -20 && roll <= -10) || (roll >= 10 && roll <= 20)) {
//       posture = "moderate";
//     } else {
//       posture = "bad";
//     }

//     Serial.print("Posture: ");
//     Serial.println(posture);

//     // Check WiFi status before Firebase operations
//     if (WiFi.status() != WL_CONNECTED) {
//       Serial.println("WiFi disconnected. Attempting to reconnect...");
//       WiFi.reconnect();
//       unsigned long startTime = millis();
//       while (WiFi.status() != WL_CONNECTED && millis() - startTime < 5000) {
//         delay(500);
//         Serial.print(".");
//       }
//       if (WiFi.status() == WL_CONNECTED) {
//         Serial.println("\nWiFi reconnected.");
//       } else {
//         Serial.println("\nWiFi reconnection failed.");
//       }
//     }

//     // Check free heap memory
//     Serial.print("Free Heap: ");
//     Serial.print(ESP.getFreeHeap());
//     Serial.println(" bytes");

//     // Send data to Firebase
//     if (Firebase.ready()) {
//       Serial.printf("Set pitch... %s\n", Firebase.RTDB.setFloat(&fbdo, "/posture/current/pitch", pitch) ? "ok" : fbdo.errorReason().c_str());
//       Serial.printf("Set roll... %s\n", Firebase.RTDB.setFloat(&fbdo, "/posture/current/roll", roll) ? "ok" : fbdo.errorReason().c_str());
//       Serial.printf("Set posture... %s\n", Firebase.RTDB.setString(&fbdo, "/posture/current/status", posture) ? "ok" : fbdo.errorReason().c_str());
//     } else {
//       Serial.println("Firebase not ready. Check credentials, network, or Firebase rules.");
//       // Test network reachability again
//       if (Ping.ping("8.8.8.8", 1)) {
//         Serial.println("Ping successful. Network is reachable.");
//       } else {
//         Serial.println("Ping failed. Network may be unreachable.");
//       }
//     }
//   }
// }




// #include <Wire.h>
// #include <MPU6050_light.h>
// #include <ESP8266WiFi.h>
// #include <Firebase_ESP_Client.h>
// #include <addons/RTDBHelper.h>
// #include <addons/TokenHelper.h>

// // WiFi credentials
// const char* ssid = "";
// const char* password = "";

// // Firebase project credentials
// #define API_KEY ""
// #define DATABASE_URL ""
// #define USER_EMAIL ""
// #define USER_PASSWORD ""

// // Firebase objects
// FirebaseData fbdo;
// FirebaseConfig config;
// FirebaseAuth auth;

// MPU6050 mpu(Wire);
// unsigned long sendDataPrevMillis = 0;

// bool initializeMPU6050() {
//   byte status = mpu.begin();
//   if (status != 0) {
//     Serial.println("MPU6050 failed.");
//     return false;
//   }
//   Serial.println("Calibrating MPU6050...");
//   delay(1000);
//   mpu.calcOffsets();
//   Serial.println("Calibration complete.");
//   return true;
// }

// void setup() {
//   Serial.begin(115200);
//   delay(100);
//   Wire.begin();

//   // WiFi connect
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED && millis() < 10000) {
//     Serial.print(".");
//     delay(300);
//   }
//   Serial.println(WiFi.status() == WL_CONNECTED ? "\nWiFi connected." : "\nWiFi connection failed.");

//   // Firebase init
//   config.api_key = API_KEY;
//   auth.user.email = USER_EMAIL;
//   auth.user.password = USER_PASSWORD;
//   config.database_url = DATABASE_URL;
//   config.token_status_callback = tokenStatusCallback;
//   fbdo.setBSSLBufferSize(2048, 512);
//   fbdo.setResponseSize(1024);
//   config.timeout.serverResponse = 10 * 1000;
//   Firebase.reconnectNetwork(true);

//   Serial.println("Initializing Firebase...");
//   Firebase.begin(&config, &auth);

//   while (!Firebase.ready() && millis() < 5000) {
//     delay(500);
//   }
//   Serial.println(Firebase.ready() ? "Firebase ready." : "Firebase not ready. Check credentials.");

//   // MPU6050 init with retry
//   bool mpuInitialized = false;
//   for (int i = 0; i < 3; i++) {
//     if (initializeMPU6050()) {
//       mpuInitialized = true;
//       break;
//     }
//     delay(500);
//   }
//   if (!mpuInitialized) {
//     Serial.println("MPU6050 initialization failed.");
//   }
// }

// void loop() {
//   mpu.update();

//   if (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0) {
//     sendDataPrevMillis = millis();

//     float pitch = mpu.getAngleX();
//     float roll = mpu.getAngleY();

//     Serial.print("Pitch: ");
//     Serial.print(pitch);
//     Serial.print(" | Roll: ");
//     Serial.println(roll);

//     // Classify posture
//     String posture;
//     if (pitch >= -10 && pitch <= 10 && roll >= -10 && roll <= 10) {
//       posture = "good";
//     } else if ((pitch >= -20 && pitch <= -10) || (pitch >= 10 && pitch <= 20) ||
//                (roll >= -20 && roll <= -10) || (roll >= 10 && roll <= 20)) {
//       posture = "moderate";
//     } else {
//       posture = "bad";
//     }

//     Serial.print("Posture: ");
//     Serial.println(posture);

//     // Send data to Firebase
//     if (Firebase.ready()) {
//       // Update real-time values
//       Firebase.RTDB.setFloat(&fbdo, "/posture/current/pitch", pitch);
//       Firebase.RTDB.setFloat(&fbdo, "/posture/current/roll", roll);
//       Firebase.RTDB.setString(&fbdo, "/posture/current/status", posture);

//       // Store historical data
//       FirebaseJson json;
//       json.set("pitch", pitch);
//       json.set("roll", roll);
//       json.set("status", posture);
//       json.set("timestamp/.sv", "timestamp"); // Firebase server timestamp
//       Firebase.RTDB.pushJSON(&fbdo, "/posture/history", &json);
//     } else {
//       Serial.println("Firebase not ready.");
//     }
//   }
// }



// Perfect code ok
#include <Wire.h>
#include <MPU6050_light.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/RTDBHelper.h>
#include <addons/TokenHelper.h>

// WiFi credentials
const char* ssid = "";
const char* password = "";

// Firebase project credentials
#define API_KEY ""
#define DATABASE_URL ""
#define USER_EMAIL ""
#define USER_PASSWORD ""

// Firebase objects
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

MPU6050 mpu(Wire);
unsigned long sendDataPrevMillis = 0;

bool initializeMPU6050() {
  byte status = mpu.begin();
  if (status != 0) {
    Serial.println("MPU6050 failed.");
    return false;
  }
  Serial.println("Calibrating MPU6050...");
  delay(1000);
  mpu.calcOffsets();
  Serial.println("Calibration complete.");
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Wire.begin();

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED && millis() < 10000) {
    Serial.print(".");
    delay(300);
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? "\nWiFi connected." : "\nWiFi connection failed.");

  // Firebase init
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  fbdo.setBSSLBufferSize(2048, 512);
  fbdo.setResponseSize(1024);
  config.timeout.serverResponse = 10 * 1000;
  Firebase.reconnectNetwork(true);

  Serial.println("Initializing Firebase...");
  Firebase.begin(&config, &auth);

  while (!Firebase.ready() && millis() < 5000) {
    delay(500);
  }
  Serial.println(Firebase.ready() ? "Firebase ready." : "Firebase not ready. Check credentials.");

  // MPU6050 init with retry
  bool mpuInitialized = false;
  for (int i = 0; i < 3; i++) {
    if (initializeMPU6050()) {
      mpuInitialized = true;
      break;
    }
    delay(500);
  }
  if (!mpuInitialized) {
    Serial.println("MPU6050 initialization failed.");
  }
}

void loop() {
  mpu.update();

  if (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0) {
    sendDataPrevMillis = millis();

    float pitch = mpu.getAngleX();
    float roll = mpu.getAngleY();

    Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print(" | Roll: ");
    Serial.println(roll);

    // Classify posture
    String posture;
    if (pitch >= -10 && pitch <= 10 && roll >= -10 && roll <= 10) {
      posture = "good";
    } else if ((pitch >= -20 && pitch <= -10) || (pitch >= 10 && pitch <= 20) ||
               (roll >= -20 && roll <= -10) || (roll >= 10 && roll <= 20)) {
      posture = "moderate";
    } else {
      posture = "bad";
    }

    Serial.print("Posture: ");
    Serial.println(posture);

    // Send data to Firebase
    if (Firebase.ready()) {
      // Update real-time values
      Firebase.RTDB.setFloat(&fbdo, "/posture/current/pitch", pitch);
      Firebase.RTDB.setFloat(&fbdo, "/posture/current/roll", roll);
      Firebase.RTDB.setString(&fbdo, "/posture/current/status", posture);

      // Store historical data
      FirebaseJson json;
      json.set("pitch", pitch);
      json.set("roll", roll);
      json.set("status", posture);
      json.set("timestamp/.sv", "timestamp"); // Firebase server timestamp
      if (Firebase.RTDB.pushJSON(&fbdo, "/posture/history", &json)) {
        Serial.println("Historical data stored.");
      } else {
        Serial.print("Failed to store historical data: ");
        Serial.println(fbdo.errorReason());
      }
    } else {
      Serial.println("Firebase not ready.");
    }
  }
}

