# Description
Chaque robot est équipé d'un Arduino Nano responsable du contrôle des moteurs DC et des servomoteurs. Les instructions sont reçues simultanément par tous les robots via une commande UART de format prédéfini. Les LEDs sur les robots permettent d'indiquer l'état du bras de connexion aux stations (s'il y a lieu) et lequel est présentement contrôlé par la manette.

# Choix du matériel
- **Arduino Nano** : choisi pour son grand nombre de broches PWM, sa simplicité et sa petite taille.
- **Communication UART** : choisie pour limiter le nombre de fils à utiliser. Trois fils étaient déjà requis pour connecter les robots à la boîte noire et aux stations (incluant la mise à la terre). Comme les robots ne font que recevoir des commandes, un seul fil supplémentaire était nécessaire.

# Broches utilisées
- **3, 5, 6, 11** : moteurs
- **9, 10** : servomoteurs
- **12, 13** : LEDs
- **1 (Rx)** : réception du message par UART
- **Sortie 5V** : alimentation du module de contrôle des moteurs
- **Vin** : alimentation du MCU par la batterie

# Configuration logicielle
Pour configurer un Arduino, veuillez suivre les étapes suivantes :
1. Ouvrir le fichier `mini_robots.ino` dans l'IDE Arduino.
2. Inclure le fichier d'en-tête correspondant au robot à configurer.
3. Téléverser le code sur le MCU.
