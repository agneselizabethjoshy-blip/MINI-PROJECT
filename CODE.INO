#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensors
const int gasPin = A0;
const int tempPin = A1;
const int ldrPin = A2;

const int pirPin = 2;

// Outputs
const int buzzerPin = 8;
const int greenLED = 12;
const int redLED = 13;


void setup()
{
  pinMode(pirPin, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(buzzerPin, LOW);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Home Automation");
  lcd.setCursor(0,1);
  lcd.print("System Ready");

  delay(2000);
}


void loop()
{
  // Read sensors
  int gasValue = analogRead(gasPin);
  int ldrValue = analogRead(ldrPin);
  int motion = digitalRead(pirPin);

  int tempADC = analogRead(tempPin);


  // TMP36 temperature calculation
  float voltage = tempADC * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100.0;


  // ---------------------------
  // GREEN LED - LDR CONTROL
  // ---------------------------

  if (ldrValue < 500)       // Dark
  {
    digitalWrite(greenLED, HIGH);
  }
  else                      // Bright
  {
    digitalWrite(greenLED, LOW);
  }


  // ---------------------------
  // RED LED - PIR CONTROL
  // ---------------------------

  if (motion == HIGH)
  {
    digitalWrite(redLED, HIGH);
    tone(buzzerPin, 1000);
  }
  else
  {
    digitalWrite(redLED, LOW);
    noTone(buzzerPin);
  }


  // ---------------------------
  // LCD DISPLAY
  // ---------------------------

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temperature,1);
  lcd.print("C");

  lcd.setCursor(9,0);
  lcd.print("G:");
  lcd.print(gasValue);


  lcd.setCursor(0,1);

  if (ldrValue < 500)
  {
    lcd.print("Dark ");
  }
  else
  {
    lcd.print("Bright");
  }


  lcd.setCursor(8,1);

  if (motion == HIGH)
  {
    lcd.print("Motion");
  }
  else
  {
    lcd.print("NoMotion");
  }


  // Serial monitor
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C | ");

  Serial.print("Gas: ");
  Serial.print(gasValue);
  Serial.print(" | ");

  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print(" | ");

  Serial.print("Motion: ");
  Serial.println(motion);


  delay(500);
}
