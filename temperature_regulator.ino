#include <Wire.h> //Wire.h library is needed for RTClib to function (RTC uses i2c connection protocols)
#include <RTClib.h>
#include <DHT.h>

RTC_DS3231 rtc;  // Create RTC object

const int relayPin = 2;  // Relay is connected to pin 2
const int dhtPin = 11;   // DHT22 sensor connected to pin 11

DHT dht(dhtPin, DHT22);  // Initialize DHT22 sensor

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);  // Halt execution if RTC is not found
  }

  // Set the RTC time to the computer's current time
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Set RTC to compile time

  Serial.println("Time set to computer's current time");

  // Initialize the DHT22 sensor
  dht.begin();

  // Set relay pin as output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Ensure relay is off initially
}

void loop() {
  // Get the current time from the RTC
  DateTime now = rtc.now();

  // Read temperature and humidity from DHT22
  float temperatureC = dht.readTemperature();  // Read temperature in Celsius
  if (isnan(temperatureC)) {
    Serial.println("Failed to read temperature from DHT sensor.");
    return;  // Exit the loop if temperature reading fails
  }
float temperatureF = (temperatureC * 9.0 / 5.0) + 32;
  // Print current time and temperature to Serial Monitor
  Serial.print("Current time: ");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  if (now.minute() < 10) {
    Serial.print("0");
  }
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.println(now.second(), DEC);

  // Print temperature in Celsius
  Serial.print("Temperature: ");
  Serial.print(temperatureF);
  Serial.println(" °F");

  // Check if the time is past 7:00 pm and before 9 am, and if it is, check if it is above 75 degrees and if it is, turn the relay on
   if ((now.hour() >= 18 || now.hour() < 9) && temperatureF > 77.5) {
    // Turn on the relay if conditions are met
    digitalWrite(relayPin, HIGH);
    Serial.println("Relay ON (Time is between 6 PM and 9 AM and Temperature > 75°F)");
  } else {
    // Otherwise, turn off the relay
    digitalWrite(relayPin, LOW);
    Serial.println("Relay OFF (Conditions not met)");
  }

  // Wait for a second before checking again
  delay(30000);
}
