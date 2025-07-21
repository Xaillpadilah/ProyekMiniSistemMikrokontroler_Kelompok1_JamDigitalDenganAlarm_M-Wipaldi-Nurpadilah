
#define BLYNK_AUTH_TOKEN "kmc46Bmd1tGq7Kv7qKUrymCCM7dKfov_"        // Ganti dengan token perangkat
#define BLYNK_TEMPLATE_ID "TMPL6t4H3hGE9"
#define BLYNK_TEMPLATE_NAME "Gas sensor"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>

/*********** Konfigurasi WiFi ***********/
// Blynk
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST";
char pass[] = "";
/*********** Inisialisasi LCD ***********/
LiquidCrystal_I2C lcd(0x27, 16, 2);

/*********** Pin dan Variabel ***********/
const int mqPin = 34;       // Sensor gas di pin analog ESP32 (GPIO34)
const int buzzerPin = 5;    // Buzzer di GPIO5
const int led = 19;         // LED fisik di GPIO19
const int threshold = 500;  // Ambang batas gas

BlynkTimer timer;

void sendToBlynk() {
  int gasValue = analogRead(mqPin);

  // Kirim data ke widget Blynk
  Blynk.virtualWrite(V0, gasValue);                       // Gauge Widget
  Blynk.virtualWrite(V2, String("Gas: ") + gasValue);     // LCD Widget

  if (gasValue > threshold) {
    Blynk.virtualWrite(V1, 255); // LED virtual ON (bahaya)
  } else {
    Blynk.virtualWrite(V1, 0);   // LED virtual OFF
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);
  pinMode(led, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("DETEKSI GAS!");
  delay(3000);
  lcd.clear();

  Blynk.begin(auth, ssid, pass);             // Koneksi ke Blynk
  timer.setInterval(2000L, sendToBlynk);     // Kirim data tiap 2 detik
}

void loop() {
  Blynk.run();
  timer.run();

  int gasValue = analogRead(mqPin);
  Serial.print("Nilai Gas: ");
  Serial.println(gasValue);

  if (gasValue > threshold) {
    tone(buzzerPin, 1000);        // Nyalakan buzzer
    digitalWrite(led, HIGH);      // Nyalakan LED fisik

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas: ");
    lcd.print(gasValue);
    lcd.setCursor(0, 1);
    lcd.print("BAHAYA GAS!!");
  } else {
    noTone(buzzerPin);            // Matikan buzzer
    digitalWrite(led, LOW);       // Matikan LED fisik

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas: ");
    lcd.print(gasValue);
  }

  delay(1000);
}
