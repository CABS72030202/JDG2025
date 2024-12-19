# Description
Le robot utilisé pour récupérer et transporter les cônes placés sur le parcours est équipé d'un Raspberry Pi Zero 2 pour permettre un contrôle sans fil via Bluetooth. Ce microprocesseur retransmet ensuite la commande reçue via UART à l'Arduino responsable du contrôle des moteurs et des servomoteurs de la pince.

# Choix du matériel
- **Raspberry Pi Zero 2** : choisi pour sa petite taille, son module sans fil, et pour s'associer aux connaissances et compétences déjà acquises par l'équipe de programmation.
- **Arduino Nano** : choisi en raison du nombre limité de broches PWM sur le Raspberry Pi, incapable de contrôler les moteurs directement.
- **Client Bluetooth** : utilisé car son initialisation est plus lente que celle du Raspberry Pi 3 Model B avec lequel il communique.

# Broches utilisées
- **GPIO 16 / Pin #10** : Rx pour la communication UART avec l'Arduino.

# Exécutables
- **mini_rpi** :
  - Initialise la communication Bluetooth avec le Raspberry Pi du bateau.
  - Exécute une boucle infinie pour recevoir les messages transmis et les retransmettre par UART.

# Configuration logicielle
- **OS** : Raspberry Pi OS Lite (32-bit)
- **Services activés** : SSH, Bluetooth
- **Utilisateur par défaut** :
  - Nom d'utilisateur : `admin`
  - Mot de passe : `ubuntu`
- **Optimisation future** : Si le temps le permet, un système Linux embarqué simplifié sera construit avec Buildroot pour optimiser les performances.
