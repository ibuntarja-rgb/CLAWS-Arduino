
#include <Arduino_RouterBridge.h>
#include <DHT.h>

// ---------------- TURBIDITY ----------------
const int LED_PINS[2] = {9, 10};
const int LDR_PIN = A0;

// ---------------- DHT11 ----------------
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------------- ULTRASONIC ----------------
#define TRIG_PIN 4
#define ECHO_PIN 3

void setup() {
  Monitor.begin();
  Monitor.println("System starting...");

  // ---- LED setup (turbidity light source) ----
  for (int i = 0; i < 2; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], HIGH); // LEDs ON
  }

  // ---- DHT ----
  dht.begin();

  // ---- Ultrasonic ----
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  delay(2000);
  Monitor.println("All sensors ready.");
}

void loop() {

  // ================= TURBIDITY =================
  int lightValue = analogRead(LDR_PIN);

  Monitor.print("Light: ");
  Monitor.print(lightValue);
  Monitor.print(" | ");

  // ================= DHT11 =================
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Monitor.print("DHT: FAIL | ");
  } else {
    Monitor.print("T:");
    Monitor.print(temp);
    Monitor.print("C H:");
    Monitor.print(hum);
    Monitor.print("% | ");
  }

  // ================= ULTRASONIC =================
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout added (important)

  if (duration == 0) {
    Monitor.print("Dist: NO ECHO");
  } else {
    distance = duration * 0.034 / 2;
    Monitor.print("Dist: ");
    Monitor.print(distance);
    Monitor.print("cm");
  }

  Monitor.println();
  Monitor.println("----------------------");

  delay(1500);
}
