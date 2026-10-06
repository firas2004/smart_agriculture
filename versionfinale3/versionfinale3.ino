#define BLYNK_TEMPLATE_ID "TMPL22621bfPf"
#define BLYNK_TEMPLATE_NAME "smartagri"
#define BLYNK_AUTH_TOKEN "v7mLB4KrAwBubhLahfrNys_807FWEfeR"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <time.h>

// WiFi
const char* ssid = "Iphone de Mohamed";
const char* password = "hhhhhh666..";

// ThingSpeak
const char* server = "http://api.thingspeak.com";
String channelID = "2906874";
String fieldNumber = "3";
String writeAPIKey = "2OJMR1D255GTCJ24";
String readAPIKey = "I0XZ87IDI1STJ9M0";

// Broches
const int soilPin = A0;
const int pumpLedPin = D1;
const int pumpRelayPin = D2;
const int rainLedPin = D6;
const int statusLed = D7;

int dryThreshold = 600;

// Override manuel
bool manualOverride = false;
unsigned long manualStartTime = 0;
const unsigned long manualDuration = 10000;

// Timing
unsigned long lastCheck = 0;
const unsigned long checkInterval = 30000;

void logToTerminal(String msg) {
  Serial.println(msg);
  Blynk.virtualWrite(V10, msg);
}

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(pumpLedPin, OUTPUT);
  pinMode(pumpRelayPin, OUTPUT);
  pinMode(rainLedPin, OUTPUT);
  pinMode(statusLed, OUTPUT);
  digitalWrite(pumpLedPin, LOW);
  digitalWrite(pumpRelayPin, LOW);
  digitalWrite(rainLedPin, LOW);
  digitalWrite(statusLed, LOW);

  WiFi.begin(ssid, password);
  logToTerminal("Connexion WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    logToTerminal(".");
  }
  logToTerminal("✅ WiFi connecté !");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  logToTerminal("⏳ Synchronisation NTP...");
  while (time(nullptr) < 100000) {
    delay(500);
    logToTerminal(".");
  }
  logToTerminal("🕒 Heure synchronisée !");
}

void loop() {
  Blynk.run();

  if (manualOverride && millis() - manualStartTime >= manualDuration) {
    digitalWrite(pumpLedPin, LOW);
    digitalWrite(pumpRelayPin, LOW);
    manualOverride = false;
    logToTerminal("⏹️ Override terminé, pompe OFF");
  }

  if (!manualOverride && millis() - lastCheck >= checkInterval) {
    lastCheck = millis();

    int soilValue = analogRead(soilPin);
    logToTerminal("🌱 Humidité du sol : " + String(soilValue));
    sendSoilToThingSpeak(soilValue);

    time_t now = time(nullptr);
    struct tm* timeInfo = localtime(&now);
    int hour = timeInfo->tm_hour;
    logToTerminal("🕒 Heure actuelle : " + String(hour) + "h");

    if (soilValue > dryThreshold) {
      logToTerminal("❗ Sol sec. Vérif pluie...");
      int rainExpected = getRainForecast();

      if (rainExpected == 1) {
        logToTerminal("☔ Pluie attendue. Aucune irrigation.");
        digitalWrite(pumpLedPin, LOW);
        digitalWrite(pumpRelayPin, LOW);
        digitalWrite(rainLedPin, HIGH);   // Affiche pluie
        digitalWrite(statusLed, LOW);
      } else {
        logToTerminal("☀️ Pas de pluie prévue.");
        if ((hour >= 5 && hour <= 9) || (hour >= 18 && hour <= 20)) {
          logToTerminal("✅ Heure favorable. Activation pompe.");
          digitalWrite(pumpLedPin, HIGH);   // Seule cette LED ON
          digitalWrite(pumpRelayPin, HIGH);
          digitalWrite(rainLedPin, LOW);
          digitalWrite(statusLed, LOW);    // Status désactivée
        } else {
          logToTerminal("⛔ Pas le bon moment. Attente...");
          digitalWrite(pumpLedPin, LOW);
          digitalWrite(pumpRelayPin, LOW);
          digitalWrite(rainLedPin, HIGH);  // Affiche "pas le moment"
          digitalWrite(statusLed, LOW);
        }
      }
    } else {
      logToTerminal("✅ Sol humide. Pas d'arrosage.");
      digitalWrite(pumpLedPin, LOW);
      digitalWrite(pumpRelayPin, LOW);
      digitalWrite(rainLedPin, LOW);
      digitalWrite(statusLed, LOW);
    }
  }
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  if (value == 1) {
    logToTerminal("🟢 Override manuel Blynk : Pompe ON");
    digitalWrite(pumpLedPin, HIGH);      // Seulement LED pompe ON
    digitalWrite(pumpRelayPin, HIGH);
    digitalWrite(statusLed, LOW);
    digitalWrite(rainLedPin, LOW);
    manualOverride = true;
    manualStartTime = millis();
  } else {
    logToTerminal("🔴 Override OFF");
    digitalWrite(pumpLedPin, LOW);
    digitalWrite(pumpRelayPin, LOW);
    digitalWrite(statusLed, LOW);
    digitalWrite(rainLedPin, LOW);
    manualOverride = false;
  }
}

void sendSoilToThingSpeak(int value) {
  WiFiClient client;
  HTTPClient http;
  String url = "http://api.thingspeak.com/update?api_key=" + writeAPIKey + "&field1=" + String(value);
  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    logToTerminal("✅ Humidité envoyée à ThingSpeak.");
  } else {
    logToTerminal("❌ Échec envoi humidité.");
  }
  http.end();
}

int getRainForecast() {
  if (WiFi.status() != WL_CONNECTED) {
    logToTerminal("🚫 WiFi déconnecté !");
    return 0;
  }

  WiFiClient client;
  HTTPClient http;
  String url = String(server) + "/channels/" + channelID + "/fields/" + fieldNumber + "/last.txt?api_key=" + readAPIKey;

  logToTerminal("📡 Requête : " + url);
  http.begin(client, url);
  int httpCode = http.GET();

  if (httpCode == 200) {
    String payload = http.getString();
    logToTerminal("📨 Réponse : " + payload);
    http.end();
    return payload.toInt();
  } else {
    logToTerminal("❌ Erreur HTTP : " + String(httpCode));
    http.end();
    return 0;
  }
}
