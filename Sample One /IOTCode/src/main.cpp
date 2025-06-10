
// #include <Arduino.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>
// #include <ESP8266WiFi.h>
// #include <Firebase_ESP_Client.h>
// #include <addons/RTDBHelper.h>
// #include <addons/TokenHelper.h>

// // WiFi credentials
// const char* ssid = "vivo Y27";
// const char* password = "mkmkmkmk";

// // Firebase config
// #define API_KEY "AIzaSyCDBLZZBNcmSIWmDCxgi0flVkVLPulfAoQ"
// #define DATABASE_URL "https://first-trail-f286d-default-rtdb.asia-southeast1.firebasedatabase.app/"
// #define USER_EMAIL "pieceone2u@gmail.com"
// #define USER_PASSWORD "12345@2310"

// // Posture classification thresholds
// const float GOOD_PITCH_MAX = 10.0;
// const float MODERATE_PITCH_MAX = 20.0;
// const float GOOD_ROLL_MAX = 5.0;
// const float MODERATE_ROLL_MAX = 10.0;

// // Firebase and MPU objects
// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;
// Adafruit_MPU6050 mpu;

// // Session control
// bool sessionActive = false;
// unsigned long sessionStartTime = 0;
// unsigned long lastLogTime = 0;
// const unsigned long SESSION_DURATION = 60UL * 1000UL; // 1 min
// const unsigned long LOG_INTERVAL = 3UL * 1000UL; // 3 sec
// String currentSessionPath = "";
// String userFeedback = "";

// void setup() {
//   Serial.begin(115200);
//   delay(1000);

//   // Connect to WiFi
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500); Serial.print(".");
//   }
//   Serial.println("\nWiFi connected");

//   // Firebase setup
//   config.api_key = API_KEY;
//   config.database_url = DATABASE_URL;
//   auth.user.email = USER_EMAIL;
//   auth.user.password = USER_PASSWORD;
//   config.token_status_callback = tokenStatusCallback;
//   Firebase.begin(&config, &auth);
//   Firebase.reconnectWiFi(true);

//   // MPU6050 setup
//   if (!mpu.begin()) {
//     Serial.println("MPU6050 init failed!");
//     while (1) delay(10);
//   }
//   mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
//   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
//   Serial.println("MPU6050 ready");

//   Serial.println("\nType 'start' to begin session.");
// }

// String classifyPosture(float pitch, float roll) {
//   bool pitchGood = abs(pitch) <= GOOD_PITCH_MAX;
//   bool pitchModerate = abs(pitch) <= MODERATE_PITCH_MAX;
//   bool rollGood = abs(roll) <= GOOD_ROLL_MAX;
//   bool rollModerate = abs(roll) <= MODERATE_ROLL_MAX;

//   if (pitchGood && rollGood) return "good";
//   else if (pitchModerate && rollModerate) return "moderate";
//   else return "bad";
// }

// void startNewSession() {
//   sessionStartTime = millis();
//   sessionActive = true;

//   // Use millis as session ID (or replace with NTP later)
//   unsigned long id = millis();
//   currentSessionPath = "/posture/sessions/session_" + String(id);

//   Serial.println("\n=== SESSION STARTED ===");
//   Serial.println("Logging posture every 3s for 1 min.");
//   Serial.println("Session Path: " + currentSessionPath);
// }

// void endSession() {
//   sessionActive = false;
//   Serial.println("\n=== SESSION ENDED ===");
//   Serial.println("Type your posture experience feedback:");

//   while (!Serial.available()) delay(100);
//   userFeedback = Serial.readStringUntil('\n');
//   userFeedback.trim();

//   // Save feedback to Firebase
//   if (Firebase.RTDB.setString(&fbdo, currentSessionPath + "/feedback", userFeedback)) {
//     Serial.println("✅ Feedback saved!");
//   } else {
//     Serial.println("❌ Feedback error: " + fbdo.errorReason());
//   }

//   Serial.println("Type 'start' to begin another session.");
// }

// void logPostureData() {
//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);

//   float pitch = atan2(a.acceleration.y, sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.z, 2))) * 180.0 / PI;
//   float roll = atan2(-a.acceleration.x, a.acceleration.z) * 180.0 / PI;

//   String status = classifyPosture(pitch, roll);

//   FirebaseJson json;
//   json.set("pitch", pitch);
//   json.set("roll", roll);
//   json.set("status", status);
//   json.set("timestamp/.sv", "timestamp"); // Let Firebase insert real time

//   String path = currentSessionPath + "/readings";
//   if (Firebase.RTDB.pushJSON(&fbdo, path, &json)) {
//     Serial.print("Logged: ");
//     Serial.print(pitch, 1); Serial.print("°, ");
//     Serial.print(roll, 1); Serial.print("° => ");
//     Serial.println(status);
//   } else {
//     Serial.println("Log failed: " + fbdo.errorReason());
//   }
// }

// void loop() {
//   // Handle "start" command
//   if (Serial.available()) {
//     String command = Serial.readStringUntil('\n');
//     command.trim();
//     if (command.equalsIgnoreCase("start") && !sessionActive) {
//       startNewSession();
//     }
//   }

//   // Active session
//   if (sessionActive) {
//     unsigned long now = millis();

//     if (now - lastLogTime >= LOG_INTERVAL) {
//       lastLogTime = now;
//       logPostureData();
//     }

//     if (now - sessionStartTime >= SESSION_DURATION) {
//       endSession();
//     }
//   }

//   if (Firebase.isTokenExpired()) {
//     Firebase.refreshToken(&config);
//   }

//   delay(100);
// }



// #include <Arduino.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>
// #include <ESP8266WiFi.h>
// #include <Firebase_ESP_Client.h>
// #include <addons/RTDBHelper.h>
// #include <addons/TokenHelper.h>

// // WiFi credentials
// const char* ssid = "vivo Y27";
// const char* password = "mkmkmkmk";

// // Firebase config
// #define API_KEY "AIzaSyCDBLZZBNcmSIWmDCxgi0flVkVLPulfAoQ"
// #define DATABASE_URL "https://first-trail-f286d-default-rtdb.asia-southeast1.firebasedatabase.app/"
// #define USER_EMAIL "pieceone2u@gmail.com"
// #define USER_PASSWORD "12345@2310"

// // Posture classification thresholds
// const float GOOD_PITCH_MAX = 10.0;
// const float MODERATE_PITCH_MAX = 20.0;
// const float GOOD_ROLL_MAX = 5.0;
// const float MODERATE_ROLL_MAX = 10.0;

// // Firebase and MPU objects
// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;
// Adafruit_MPU6050 mpu;

// // Session control
// bool sessionActive = false;
// unsigned long sessionStartTime = 0;
// unsigned long lastLogTime = 0;
// unsigned long SESSION_DURATION = 30UL * 60UL * 1000UL; // Default 30 minutes
// const unsigned long LOG_INTERVAL = 3UL * 1000UL; // 3 sec
// String currentSessionPath = "";
// String userFeedback = "";
// String userName = "";
// int userAge = 0;

// void setup() {
//   Serial.begin(115200);
//   delay(1000);

//   // Connect to WiFi
//   WiFi.begin(ssid, password);
//   Serial.print("Connecting to WiFi");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500); Serial.print(".");
//   }
//   Serial.println("\nWiFi connected");

//   // Firebase setup
//   config.api_key = API_KEY;
//   config.database_url = DATABASE_URL;
//   auth.user.email = USER_EMAIL;
//   auth.user.password = USER_PASSWORD;
//   config.token_status_callback = tokenStatusCallback;
//   Firebase.begin(&config, &auth);
//   Firebase.reconnectWiFi(true);

//   // MPU6050 setup
//   if (!mpu.begin()) {
//     Serial.println("MPU6050 init failed!");
//     while (1) delay(10);
//   }
//   mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
//   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
//   Serial.println("MPU6050 ready");

//   Serial.println("\nType 'start' to begin session.");
// }

// String classifyPosture(float pitch, float roll) {
//   bool pitchGood = abs(pitch) <= GOOD_PITCH_MAX;
//   bool pitchModerate = abs(pitch) <= MODERATE_PITCH_MAX;
//   bool rollGood = abs(roll) <= GOOD_ROLL_MAX;
//   bool rollModerate = abs(roll) <= MODERATE_ROLL_MAX;

//   if (pitchGood && rollGood) return "good";
//   else if (pitchModerate && rollModerate) return "moderate";
//   else return "bad";
// }

// void getUserInfo() {
//   Serial.println("\nPlease enter your name:");
//   while (!Serial.available()) delay(100);
//   userName = Serial.readStringUntil('\n');
//   userName.trim();

//   Serial.println("Please enter your age:");
//   while (!Serial.available()) delay(100);
//   String ageStr = Serial.readStringUntil('\n');
//   userAge = ageStr.toInt();

//   Serial.println("\nSelect session duration:");
//   Serial.println("1 - 30 minutes");
//   Serial.println("2 - 2 hours");
//   Serial.println("3 - 4 hours");
//   Serial.println("Enter choice (1-3):");
  
//   while (!Serial.available()) delay(100);
//   String choice = Serial.readStringUntil('\n');
//   choice.trim();

//   if (choice == "1") {
//     SESSION_DURATION = 30UL * 60UL * 1000UL;
//     Serial.println("30 minutes session selected");
//   } else if (choice == "2") {
//     SESSION_DURATION = 2UL * 60UL * 60UL * 1000UL;
//     Serial.println("2 hours session selected");
//   } else if (choice == "3") {
//     SESSION_DURATION = 4UL * 60UL * 60UL * 1000UL;
//     Serial.println("4 hours session selected");
//   } else {
//     Serial.println("Invalid choice, defaulting to 30 minutes");
//     SESSION_DURATION = 30UL * 60UL * 1000UL;
//   }
// }

// void startNewSession() {
//   getUserInfo();
  
//   sessionStartTime = millis();
//   sessionActive = true;

//   // Use millis as session ID (or replace with NTP later)
//   unsigned long id = millis();
//   currentSessionPath = "/posture/sessions/session_" + String(id);

//   // Save user info to Firebase
//   Firebase.RTDB.setString(&fbdo, currentSessionPath + "/user/name", userName);
//   Firebase.RTDB.setInt(&fbdo, currentSessionPath + "/user/age", userAge);
//   Firebase.RTDB.setInt(&fbdo, currentSessionPath + "/duration_minutes", SESSION_DURATION / 60000);

//   Serial.println("\n=== SESSION STARTED ===");
//   Serial.print("User: "); Serial.print(userName);
//   Serial.print(", Age: "); Serial.println(userAge);
//   Serial.print("Duration: "); Serial.print(SESSION_DURATION / 60000); Serial.println(" minutes");
//   Serial.println("Logging posture every 3s");
//   Serial.println("Session Path: " + currentSessionPath);
// }

// void endSession() {
//   sessionActive = false;
//   Serial.println("\n=== SESSION ENDED ===");
//   Serial.println("Type your posture experience feedback:");

//   while (!Serial.available()) delay(100);
//   userFeedback = Serial.readStringUntil('\n');
//   userFeedback.trim();

//   // Save feedback to Firebase
//   if (Firebase.RTDB.setString(&fbdo, currentSessionPath + "/feedback", userFeedback)) {
//     Serial.println("✅ Feedback saved!");
//   } else {
//     Serial.println("❌ Feedback error: " + fbdo.errorReason());
//   }

//   Serial.println("Type 'start' to begin another session.");
// }

// void logPostureData() {
//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);

//   float pitch = atan2(a.acceleration.y, sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.z, 2))) * 180.0 / PI;
//   float roll = atan2(-a.acceleration.x, a.acceleration.z) * 180.0 / PI;

//   String status = classifyPosture(pitch, roll);

//   FirebaseJson json;
//   json.set("pitch", pitch);
//   json.set("roll", roll);
//   json.set("status", status);
//   json.set("timestamp/.sv", "timestamp"); // Let Firebase insert real time

//   String path = currentSessionPath + "/readings";
//   if (Firebase.RTDB.pushJSON(&fbdo, path, &json)) {
//     Serial.print("Logged: ");
//     Serial.print(pitch, 1); Serial.print("°, ");
//     Serial.print(roll, 1); Serial.print("° => ");
//     Serial.println(status);
//   } else {
//     Serial.println("Log failed: " + fbdo.errorReason());
//   }
// }

// void loop() {
//   // Handle "start" command
//   if (Serial.available()) {
//     String command = Serial.readStringUntil('\n');
//     command.trim();
//     if (command.equalsIgnoreCase("start") && !sessionActive) {
//       startNewSession();
//     }
//   }

//   // Active session
//   if (sessionActive) {
//     unsigned long now = millis();

//     if (now - lastLogTime >= LOG_INTERVAL) {
//       lastLogTime = now;
//       logPostureData();
//     }

//     if (now - sessionStartTime >= SESSION_DURATION) {
//       endSession();
//     }
//   }

//   if (Firebase.isTokenExpired()) {
//     Firebase.refreshToken(&config);
//   }

//   delay(100);
// }


#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/RTDBHelper.h>
#include <addons/TokenHelper.h>

// WiFi credentials
const char* ssid = "vivo Y27";
const char* password = "mkmkmkmk";

// Firebase config
#define API_KEY "AIzaSyCDBLZZBNcmSIWmDCxgi0flVkVLPulfAoQ"
#define DATABASE_URL "https://first-trail-f286d-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "pieceone2u@gmail.com"
#define USER_PASSWORD "12345@2310"

// Posture classification thresholds
const float GOOD_PITCH_MAX = 10.0;
const float MODERATE_PITCH_MAX = 20.0;
const float GOOD_ROLL_MAX = 5.0;
const float MODERATE_ROLL_MAX = 10.0;

// Firebase and MPU objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
Adafruit_MPU6050 mpu;

// Session control
bool sessionActive = false;
unsigned long sessionStartTime = 0;
unsigned long lastLogTime = 0;
unsigned long SESSION_DURATION = 30UL * 60UL * 1000UL; // Default 30 minutes
const unsigned long LOG_INTERVAL = 3UL * 1000UL; // 3 sec
String currentSessionPath = "";
String userFeedback = "";
String userName = "";
int userAge = 0;

// Feedback variables
String discomfortLevel = "";
String painArea = "";
String actualDuration = "";
String notes = "";

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // MPU6050 setup
  if (!mpu.begin()) {
    Serial.println("MPU6050 init failed!");
    while (1) delay(10);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("MPU6050 ready");

  Serial.println("\nType 'start' to begin session.");
}

String classifyPosture(float pitch, float roll) {
  bool pitchGood = abs(pitch) <= GOOD_PITCH_MAX;
  bool pitchModerate = abs(pitch) <= MODERATE_PITCH_MAX;
  bool rollGood = abs(roll) <= GOOD_ROLL_MAX;
  bool rollModerate = abs(roll) <= MODERATE_ROLL_MAX;

  if (pitchGood && rollGood) return "good";
  else if (pitchModerate && rollModerate) return "moderate";
  else return "bad";
}

void getUserInfo() {
  Serial.println("\nPlease enter your name:");
  while (!Serial.available()) delay(100);
  userName = Serial.readStringUntil('\n');
  userName.trim();

  Serial.println("Please enter your age:");
  while (!Serial.available()) delay(100);
  String ageStr = Serial.readStringUntil('\n');
  userAge = ageStr.toInt();

  Serial.println("\nSelect session duration:");
  Serial.println("1 - 30 minutes");
  Serial.println("2 - 2 hours");
  Serial.println("3 - 4 hours");
  Serial.println("Enter choice (1-3):");
  
  while (!Serial.available()) delay(100);
  String choice = Serial.readStringUntil('\n');
  choice.trim();

  if (choice == "1") {
    SESSION_DURATION = 30UL * 10UL * 1000UL;
    Serial.println("30 minutes session selected");
  } else if (choice == "2") {
    SESSION_DURATION = 2UL * 60UL * 60UL * 1000UL;
    Serial.println("2 hours session selected");
  } else if (choice == "3") {
    SESSION_DURATION = 4UL * 60UL * 60UL * 1000UL;
    Serial.println("4 hours session selected");
  } else {
    Serial.println("Invalid choice, defaulting to 30 minutes");
    SESSION_DURATION = 30UL * 60UL * 1000UL;
  }
}

void collectFeedback() {
  Serial.println("\n=== POSTURE EXPERIENCE FEEDBACK ===");
  
  // Discomfort Level
  Serial.println("Rate your discomfort level:");
  Serial.println("1 - None");
  Serial.println("2 - Mild");
  Serial.println("3 - Moderate");
  Serial.println("4 - Severe");
  Serial.println("Enter choice (1-4):");
  while (!Serial.available()) delay(100);
  String discomfortChoice = Serial.readStringUntil('\n');
  discomfortChoice.trim();
  
  if (discomfortChoice == "1") discomfortLevel = "none";
  else if (discomfortChoice == "2") discomfortLevel = "mild";
  else if (discomfortChoice == "3") discomfortLevel = "moderate";
  else if (discomfortChoice == "4") discomfortLevel = "severe";
  else discomfortLevel = "unknown";

  // Pain Area
  Serial.println("\nWhere did you feel discomfort?");
  Serial.println("1 - Lower back");
  Serial.println("2 - Neck");
  Serial.println("3 - Shoulders");
  Serial.println("4 - None");
  Serial.println("Enter choice (1-4):");
  while (!Serial.available()) delay(100);
  String painChoice = Serial.readStringUntil('\n');
  painChoice.trim();
  
  if (painChoice == "1") painArea = "lower back";
  else if (painChoice == "2") painArea = "neck";
  else if (painChoice == "3") painArea = "shoulders";
  else if (painChoice == "4") painArea = "none";
  else painArea = "unknown";

  // Actual Duration
  Serial.println("\nHow long did you actually sit? (in minutes)");
  Serial.println("Enter number (e.g., 30, 45, 60):");
  while (!Serial.available()) delay(100);
  actualDuration = Serial.readStringUntil('\n');
  actualDuration.trim();

  // Notes
  Serial.println("\nAny additional notes? (optional)");
  Serial.println("Examples: 'Felt good', 'Back pain at 20 min'");
  while (!Serial.available()) delay(100);
  notes = Serial.readStringUntil('\n');
  notes.trim();
}

void startNewSession() {
  getUserInfo();
  
  sessionStartTime = millis();
  sessionActive = true;

  // Use millis as session ID (or replace with NTP later)
  unsigned long id = millis();
  currentSessionPath = "/posture/sessions/session_" + String(id);

  // Save user info to Firebase
  Firebase.RTDB.setString(&fbdo, currentSessionPath + "/user/name", userName);
  Firebase.RTDB.setInt(&fbdo, currentSessionPath + "/user/age", userAge);
  Firebase.RTDB.setInt(&fbdo, currentSessionPath + "/duration_minutes", SESSION_DURATION / 60000);

  Serial.println("\n=== SESSION STARTED ===");
  Serial.print("User: "); Serial.print(userName);
  Serial.print(", Age: "); Serial.println(userAge);
  Serial.print("Duration: "); Serial.print(SESSION_DURATION / 60000); Serial.println(" minutes");
  Serial.println("Logging posture every 3s");
  Serial.println("Session Path: " + currentSessionPath);
}

void endSession() {
  sessionActive = false;
  
  // Collect detailed feedback
  collectFeedback();

  // Save all feedback to Firebase
  FirebaseJson feedbackJson;
  feedbackJson.set("discomfort_level", discomfortLevel);
  feedbackJson.set("pain_area", painArea);
  feedbackJson.set("actual_duration_minutes", actualDuration);
  feedbackJson.set("notes", notes);

  if (Firebase.RTDB.setJSON(&fbdo, currentSessionPath + "/feedback", &feedbackJson)) {
    Serial.println("\n✅ Feedback saved!");
    Serial.println("Discomfort Level: " + discomfortLevel);
    Serial.println("Pain Area: " + painArea);
    Serial.println("Actual Duration: " + actualDuration + " minutes");
    Serial.println("Notes: " + notes);
  } else {
    Serial.println("❌ Feedback error: " + fbdo.errorReason());
  }

  Serial.println("\nType 'start' to begin another session.");
}

void logPostureData() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float pitch = atan2(a.acceleration.y, sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.z, 2))) * 180.0 / PI;
  float roll = atan2(-a.acceleration.x, a.acceleration.z) * 180.0 / PI;

  String status = classifyPosture(pitch, roll);

  FirebaseJson json;
  json.set("pitch", pitch);
  json.set("roll", roll);
  json.set("status", status);
  json.set("timestamp/.sv", "timestamp"); // Let Firebase insert real time

  String path = currentSessionPath + "/readings";
  if (Firebase.RTDB.pushJSON(&fbdo, path, &json)) {
    Serial.print("Logged: ");
    Serial.print(pitch, 1); Serial.print("°, ");
    Serial.print(roll, 1); Serial.print("° => ");
    Serial.println(status);
  } else {
    Serial.println("Log failed: " + fbdo.errorReason());
  }
}

void loop() {
  // Handle "start" command
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command.equalsIgnoreCase("start") && !sessionActive) {
      startNewSession();
    }
  }

  // Active session
  if (sessionActive) {
    unsigned long now = millis();

    if (now - lastLogTime >= LOG_INTERVAL) {
      lastLogTime = now;
      logPostureData();
    }

    if (now - sessionStartTime >= SESSION_DURATION) {
      endSession();
    }
  }

  if (Firebase.isTokenExpired()) {
    Firebase.refreshToken(&config);
  }

  delay(100);
}