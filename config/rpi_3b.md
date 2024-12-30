# Description
Le Raspberry Pi 3 Model B placé sur le bateau est le maître responsable de toutes les opérations de la solution robotique. Il établit la communication avec la manette permettant de contrôler les robots, puis envoie les commandes du pilote aux robots via UART et Bluetooth. Il est également responsable de la communication avec la boîte noire, permettant la gestion et le transfert automatique des passagers entre les stations. En outre, il assure le contrôle des moteurs sans balais imperméables utilisés pour les hélices du bateau.

# Choix du matériel
- **Raspberry Pi 3 Model B** : choisi pour son port COM permettant l'utilisation d'USB-UART pour communiquer avec la boîte noire.
- **Arduino Nano** : utilisé en raison du nombre limité de broches PWM sur le Raspberry Pi, incapable de contrôler directement les moteurs des roues.
- **Moteurs sans balais** : gérés par le Raspberry Pi, car un PWM d'une fréquence de 50 à 60 Hz est requis, ce que l'Arduino exécute avec difficulté.
- **Serveur Bluetooth** : choisi pour une initialisation plus rapide que celle du Raspberry Pi Zero 2 avec lequel il communique.
- **Broches GPIO** : utilisées pour communiquer entre les différents exécutables pour des raisons de simplicité.

# Broches utilisées
- **Interfaçage des exécutables** :
  - GPIO 0 / Pin #11 -> GPIO 2 / Pin #13
  - GPIO 3 / Pin #15 -> GPIO 4 / Pin #16
  - GPIO 5 / Pin #18 -> GPIO 25 / Pin #37
  - GPIO 6 / Pin #22 -> GPIO 27 / Pin #36
- **PWM et alimentation pour les moteurs sans balais** :
  - GPIO 23 / Pin #33 (PWM1) + GPIO 24 / Pin #35
  - GPIO 26 / Pin #32 (PWM0) + GPIO 22 / Pin #31
- **Servomoteurs** :
  - GPIO 7 / Pin #7
- **LEDs d'états du système** :
  - GPIO 12 / Pin #19
  - GPIO 13 / Pin #21
  - GPIO 14 / Pin #23
  - GPIO 28 / Pin #38
  - GPIO 29 / Pin #40
- **Communication UART** :
  - GPIO 15 / Pin #8 : Tx pour la communication UART avec les Arduino.
- **Ports USB** :
  - Récepteur de la manette sans fil : ports USB du haut.
  - Fil USB-A vers USB-C : pour la communication avec la boîte noire (ports USB du bas).

# Exécutables
- **blackbox** :
  - Gère les passagers et la communication avec la boîte noire.
- **controller** :
  - Gère la communication avec la manette, la conversion des commandes en un format prédéfini compréhensible par les Arduino, la communication Bluetooth et le contrôle des moteurs sans balais.

# Configuration logicielle
- **OS** : Raspberry Pi OS Lite (64-bit)
- **Services activés** : SSH, Bluetooth
- **Utilisateur par défaut** :
  - Nom d'utilisateur : `admin`
  - Mot de passe : `password`
- **Optimisation future** : Si le temps le permet, un système Linux embarqué simplifié sera construit avec Buildroot pour optimiser les performances.
