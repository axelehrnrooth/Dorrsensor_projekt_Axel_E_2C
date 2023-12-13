// Defenierar pin-nummer för, grönt LED-ljus, högtalaren, flexsensorn och tröskelvärde för flexsensor
const int greenLedPin = 7;
const int speakerPin = 8;
const int flexSensorPin = A0;
const int flexThreshold = 770;

// Definierar alarmets varaktighet och variablernma för att spåra inmatad kod och om larmet är utlöst
const int correctCode = 1234;
const int alarmDuration = 5000;
int enteredCode = 0;
bool alarmTriggered = false;

void setup() {
  Serial.begin(9600);
// Ändrar dem olika pinsen till rätt mode
  pinMode(greenLedPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(flexSensorPin, INPUT);
  digitalWrite(greenLedPin, LOW); // Slår av grönt LED-ljus
}

void loop() {
  // Läser av värdet från flexsensor
  int flexValue = analogRead(flexSensorPin);

  Serial.println("Flexvärde: " + String(flexValue));

  // Läser inmatad kod från Serial Monitor OM ingen kod har angetts ännu
  if (enteredCode == 0 && Serial.available() >= 4) {
    for (int i = 0; i < 4; i++) {
      char digit = Serial.read();
      enteredCode = enteredCode * 10 + (digit - '0');
    }
    Serial.println("Kod mottagen. Testa att öppna dörren");
  }

  // Hanterar felaktig kod eller avstängt larm
  if ((enteredCode != correctCode && alarmTriggered) || (enteredCode == correctCode && alarmTriggered)) {
    enteredCode = 0;
    alarmTriggered = false;
    digitalWrite(greenLedPin, LOW);
    Serial.println("Felaktig kod eller larm avstängt.");
  }

  // Utlöser larm om flexvärdet är under tröskelvärdet och ingen kod har angetts och larmet inte redan är utlöst så körs denna if satsen
  if (flexValue < flexThreshold && enteredCode == 0 && !alarmTriggered) {
    playAlarm();
    alarmTriggered = true;
    Serial.println("Larm utlöst!");
  }

  // Hanterar rätt inmatad kod
  if (enteredCode == correctCode) {
    digitalWrite(greenLedPin, HIGH);
    delay(alarmDuration);
    digitalWrite(greenLedPin, LOW);
    Serial.println("Rätt kod! Grön lampa tänds i 5 sekunder.");
    enteredCode = 0;
  }

  // Lite fördröjning för att undvika en överbelastning eller lagg
  delay(100);
}

// Funktion för att spela ett larm genom högtalaren
void playAlarm() {
  Serial.println("Larm spelas i 5 sekunder.");
  for (int i = 0; i < 5; i++) {
    tone(speakerPin, 1000);
    delay(500);
    noTone(speakerPin);
    delay(500);
  }
  alarmTriggered = false;
}

// I detta projekt varken tänkte jag på eller lyckades hålla loopen så "clean" som möjligt som du sa att man skulle försöka med. 
//Men jag ska tänka på det tills nästa projekt och se till så all logik bakom programmet inte ligger i loopen utan istället görs av olika funktioner.
