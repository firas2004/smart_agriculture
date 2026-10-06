# 🌱 Smart Agriculture — Système d'Irrigation Intelligent IoT

[![Platform: ESP8266](https://img.shields.io/badge/Platform-NodeMCU%20ESP8266-green.svg)](https://en.wikipedia.org/wiki/NodeMCU)
[![Cloud: ThingSpeak](https://img.shields.io/badge/Cloud-ThingSpeak-blue.svg)](https://thingspeak.com/)
[![Mobile: Blynk IoT](https://img.shields.io/badge/Mobile-Blynk%20IoT-00d084.svg)](https://blynk.io/)
[![Language: C++ / Arduino](https://img.shields.io/badge/Language-Arduino%20C%2B%2B-00979C.svg)](https://www.arduino.cc/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Un système d'irrigation connecté et automatisé de nouvelle génération conçu pour optimiser l'arrosage agricole, préserver les ressources en eau et surveiller la santé des cultures en temps réel grâce à l'Internet des Objets (**IoT**).

---

## 📌 Présentation du Projet

Le projet **Smart Agriculture** répond aux défis actuels de l'agriculture moderne face au changement climatique et au stress hydrique. En combinant des capteurs environnementaux, une unité de contrôle NodeMCU ESP8266 et des plateformes cloud (ThingSpeak et Blynk), le système assure une prise de décision autonome pour l'arrosage tout en offrant un suivi détaillé et un contrôle manuel à distance.

### ✨ Fonctionnalités Principales

- 💧 **Irrigation Autonome & Ciblée** : Déclenchement automatique de la pompe à eau en fonction du seuil d'humidité du sol.
- 🌡️ **Surveillance Microclimatique** : Suivi en continu de la température et de l'humidité de l'air via le capteur DHT11.
- ☀️ **Détection de Luminosité (LDR)** : Évite l'irrigation aux heures d'ensoleillement maximal pour limiter l'évaporation excessive.
- 🌧️ **Prévention Météorologique & Capteur de Pluie** : Suspension de l'arrosage en cas de pluie détectée ou prévisions défavorables.
- 📊 **Télémétrie Cloud ThingSpeak** : Historique graphique des mesures avec analyse statistique à long terme.
- 📱 **Tableau de Bord Mobile Blynk** : Supervision en direct, alertes instantanées et activation manuelle de secours.
- 🌐 **Interface Web Interactive** : Page de présentation et documentation complète incluse dans le dépôt (`index.html`).

---

## 🏗️ Architecture du Système

```
                           +----------------------+
                           |   Capteurs de Terrain|
                           |  - DHT11 (Temp/Hum)  |
                           |  - Humidité du sol   |
                           |  - Capteur de pluie  |
                           |  - Photorésistance   |
                           +----------+-----------+
                                      |
                                      v
+------------------+       +----------+-----------+       +------------------+
| Relais & Pompe   | <---- |   NodeMCU ESP8266    | ----> | Écran LCD / LEDs |
| Électrovanne     |       |   (Unité Centrale)   |       | Indicateurs      |
+------------------+       +----------+-----------+       +------------------+
                                      |
                                  Wi-Fi (IP)
                                      |
               +----------------------+----------------------+
               |                                             |
               v                                             v
     +-------------------+                         +-------------------+
     |    ThingSpeak     |                         |     Blynk IoT     |
     | Analyse & Cloud   |                         | Contrôle Mobile   |
     +-------------------+                         +-------------------+
```

---

## 🔌 Composants Matériels & Câblage

| Composant | Rôle | Broche / Interface |
|---|---|---|
| **NodeMCU ESP8266** | Microcontrôleur Wi-Fi central | — |
| **Capteur d'humidité du sol** | Mesure du taux d'humidité de la terre | Entrée Analogique `A0` (multiplexée / dédiée) |
| **DHT11** | Mesure de la température et humidité ambiante | Entrée Numérique `D4` (GPIO2) |
| **Capteur de pluie** | Détection des précipitations en direct | Entrée Numérique `D5` (GPIO14) |
| **Photorésistance (LDR)** | Évaluation du niveau de lumière | Entrée Numérique / Analogique |
| **Module Relais 5V** | Commande tout-ou-rien de la pompe | Sortie Numérique `D1` (GPIO5) |
| **Mini Pompe à eau 5V/12V** | Actionneur d'irrigation | Commandée par le relais |
| **LEDs d'état & Buzzer** | Alertes visuelles et sonores de niveau bas | Sorties Numériques |

---

## ☁️ Écosystème Logiciel & Protocoles

### 1. ThingSpeak
- **Canal de métriques environnementales :**
  - **Champ 1 :** Température ambiante (°C)
  - **Champ 2 :** Humidité de l'air (%)
  - **Champ 3 :** Humidité du sol (%)
  - **Champ 4 :** État du relais / Pompe (0 = Arrêt, 1 = Marche)
  - **Champ 5 :** Statut pluie & luminosité

### 2. Blynk IoT
- Bouton poussoir virtuel pour basculer entre **Mode Auto** et **Mode Manuel**.
- Interrupteur virtuel d'urgence pour forcer l'arrosage.
- Jauges circulaires pour l'humidité du sol et la température.
- Notifications push en cas de seuil critique ou de dysfonctionnement.

### 3. Synchronisation NTP & Météo
- Horloge synchronisée via **NTP** pour programmer l'arrosage aux plages horaires optimales (tôt le matin ou en soirée).
- Requêtes HTTP REST pour intégration prévisionnelle avec l'API OpenWeatherMap.

---

## 📁 Structure du Dépôt

```plaintext
smart_agriculture/
├── images/                                 # Captures et schémas extraits des documentations
│   ├── code_page01.png ... code_page04.png
│   ├── comm_page01.png ... comm_page04.png
│   ├── rapport_page01.png ... rapport_page04.png
│   └── thingspeak_page01.png ... thingspeak_page04.png
├── versionfinale3/
│   └── versionfinale3.ino                  # Code source Arduino / ESP8266 complet
├── index.html                              # Présentation web interactive moderne du projet
├── smart-agriculture-pitch (2).html        # Support de présentation / Pitch deck
├── Rapport Technique.pdf                   # Rapport technique complet du projet
├── Smart Agriculture Code Explanation.pdf  # Guide explicatif détaillé du code firmware
├── Smart Agriculture Communication Explanation.pdf # Analyse des protocoles et flux réseaux
├── ThingSpeak.pdf                          # Documentation de configuration ThingSpeak
├── .gitignore
└── README.md
```

---

## 🚀 Démarrage Rapide

### Prérequis
- [Arduino IDE](https://www.arduino.cc/en/software) (version 1.8.19 ou 2.x)
- Support des cartes ESP8266 installé via le gestionnaire de cartes (`http://arduino.esp8266.com/stable/package_esp8266com_index.json`)
- Bibliothèques Arduino requises :
  - `ESP8266WiFi`
  - `BlynkSimpleEsp8266`
  - `DHT sensor library`
  - `ThingSpeak`
  - `NTPClient`

### Configuration du Firmware

1. Ouvrez le fichier [`versionfinale3/versionfinale3.ino`](versionfinale3/versionfinale3.ino) dans l'Arduino IDE.
2. Renseignez vos identifiants réseau et clés API :

```cpp
// Configuration Wi-Fi
char ssid[] = "VOTRE_SSID_WIFI";
char pass[] = "VOTRE_MOT_DE_PASSE";

// Configuration Blynk
char auth[] = "VOTRE_TOKEN_BLYNK";

// Configuration ThingSpeak
unsigned long myChannelNumber = VOTRE_NUMERO_DE_CANAL;
const char * myWriteAPIKey = "VOTRE_CLE_API_ECRITURE";
```

3. Sélectionnez la carte **NodeMCU 1.0 (ESP-12E Module)** et le port série adéquat.
4. Téléversez le code sur la carte.

---

## 🖥️ Visualisation de la Page Web

Le projet inclut une page web moderne et responsive [`index.html`](index.html) synthétisant l'ensemble des aspects techniques, hardware, réseau et code :
- Double-cliquez simplement sur `index.html` pour l'ouvrir dans n'importe quel navigateur moderne.
- Aucune dépendance externe ni serveur n'est nécessaire.

---

## 🔮 Perspectives d'Évolution

- 📡 **Passage à LoRa / LoRaWAN** : Augmentation de la portée de transmission jusqu'à 10 km pour les grandes parcelles agricoles isolées.
- ☀️ **Alimentation Autonome Solaire** : Ajout d'un panneau photovoltaïque avec module de charge TP4056 et batterie Li-ion 18650.
- 🤖 **Modèle Prédictif d'Évapotranspiration** : Algorithme prédictif ajustant le volume d'eau selon les données météo à 24h.
- 🧪 **Capteurs NPK** : Analyse de la fertilité chimique des sols (Azote, Phosphore, Potassium).

---

## 👥 Auteur

- **Firas Adel** — Étudiant à la Faculté des Sciences de Tunis (FST)
- Contact : [firas.adel@etudiant-fst.utm.tn](mailto:firas.adel@etudiant-fst.utm.tn)
