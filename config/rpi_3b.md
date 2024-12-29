# Description
<div style="text-align: justify"> Le Raspberry Pi 3 Model B placé sur le bateau est le maître responsable de toutes les opérations de la solution robotique. Il établit la communication avec la manette permettant de contrôler les robots, puis envoie les commandes du pilote aux robots via UART et Bluetooth. Il est également responsable de la communication avec la boîte noire, permettant la gestion et le transfert automatique des passagers entre les stations. En outre, il assure le contrôle des moteurs sans balais imperméables utilisés pour les hélices du bateau. </div>

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

# Développement d'un système Linux embarqué simplifié avec Buildroot
<div style="text-align: justify"> Afin d'optimiser les perfomances de la solution robotique, un système d'exploitation allégé est utilisé. Voici les étapes détaillées du développement de ce système avec Buildroot. </div>

### 1. Préparation
- **1.1 Installation des outils requis** 

  <div style="text-align: justify"> Les outils nécessaires au projet sont installés dans le répertoire $HOME pour garantir un accès facile et centralisé. Cette approche permet de maintenir un environnement de travail propre et bien organisé. </div>

  ```bash
  sudo apt-get update 
  sudo apt-get upgrade
  cd
  git clone https://git.buildroot.net/buildroot 
  ```

- **1.2 Préparation de l'environnement**

  <div style="text-align: justify"> Cette étape consiste à s’assurer que l’environnement de travail est adéquatement configuré pour le projet. </div>

  ```bash
  ln -s "/mnt/c/Users/sebas/OneDrive - Université du Québec à Trois-Rivières/Implication/JDG/Machine 2025/JDG2025/" ~/buildroot/JDG2025
  ```

- **1.3 Choix de la configuration par défaut**

  <div style="text-align: justify"> La première étape de la configuration consiste à spécifier l’architecture cible du système embarqué. Avant de procéder, il est crucial de supprimer tous les artefacts résiduels provenant d’anciennes configurations, afin d’éviter d’éventuels conflits ou erreurs. Ce choix est fondamental pour garantir la compatibilité du code avec le matériel cible et assurer le bon fonctionnement de l’ensemble du système. </div> 

  ```bash
  cd ~/buildroot 
  make distclean 
  make list-defconfigs 
  make raspberrypi3_64_defconfig
  ```

### 2. Configuration du noyau Linux

<div style="text-align: justify"> L’un des objectifs principaux du projet étant de concevoir un système embarqué dédié à l’exécution d’une application spécifique, il est conseillé de minimiser la charge du système. Cette approche permet non seulement d’améliorer les performances globales, mais également d’accroître la sécurité. </div>

```bash
make linux-menuconfig 
```

- **2.1 Détermination des dépendences du noyau**

<div style="text-align: justify"> Pour garantir que seules les ressources strictement nécessaires à nos applications soient installées sur le système, il est primordial d’identifier au préalable l’ensemble des dépendances spécifiques requises par ces applications. Cette démarche permet d’optimiser l’utilisation des ressources et de minimiser les risques d’incompatibilités ou de conflits logiciels. </div>

```bash
ldd JDG2025/build/blackbox
ldd JDG2025/build/controller
```

- **2.2 Activation d'options supplémentaires**

<div style="text-align: justify"> Dans le cas où certaines dépendances essentielles ne seraient pas incluses dans la configuration par défaut, il est impératif de les intégrer manuellement. Cette étape garantit le bon fonctionnement des applications en assurant que tous les éléments requis soient disponibles dans l’environnement cible. </div>

```bash
Device Drivers --->       
    Input device support --->
        <*> Joystick interface
        [*] Joysticks/Gamepads --->
            <M> Logitech ADI digital joysticks and gamepads     
        [*] Pulse-Width Modulation (PWM) Support --->
            <M> Clock based PWM support   
```

- **2.3 Désactivation d'options inutilisées**

<div style="text-align: justify"> À l’inverse, si certaines options non nécessaires sont activées par défaut, il est recommandé de les désactiver. Cette démarche contribue à alléger le système, à optimiser ses performances et à réduire les risques potentiels liés à l’encombrement de fonctionnalités superflues. </div>

```bash
Networking support --->
    [ ] Amateur Radio 
    
Device Drivers --->
    [*] Network device support --->
        [ ] Ethernet driver support
    Input device support --->
        [ ] Touchscreens
    {M} I2C support
    [ ] SPI supprt
    < > Multimedia support
    Graphics support --->
        [ ] Bootup logo
    < > Sound card support
    [*] USB support
        [ ] USB announce new devices
    [ ] Real Time Clock
    [ ] Microsoft Surface Platform-Specific Device Drivers
    Android --->
        [ ] Android Binder IPC Driver

Kernel hacking --->
    [ ] Tracers
```

### 3. Configuration du système avec Buildroot

<div style="text-align: justify"> La configuration de Buildroot consiste à définir l’ensemble des options pour le système embarqué, en sélectionnant les paramètres appropriés pour répondre aux besoins spécifiques du projet. Cette étape inclut également l'intégration de tous les paquets nécessaires au bon fonctionnement du système, garantissant ainsi que toutes les dépendances et fonctionnalités requises soient présentes et correctement configurées. </div>

```bash
make menuconfig 
```

- **3.1 Configuration du système**

<div style="text-align: justify"> Tous les ajustements apportés à la configuration par défaut sont détaillés dans cette section. Ces modifications visent à adapter précisément le système aux besoins spécifiques identifiés, tout en documentant les étapes pour assurer la traçabilité et faciliter la maintenance. </div>

```bash
System configuration ---> 
    System hostname > JDG2025
    System banner > https://github.com/CABS72030202/JDG2025.git
    [*] Run a getty (login prompt) after boot ---> 
        other options to pass to getty > -n -l /bin/sh
    Root filesystem overlay directories > board/raspberrypi3-64/rootfs_overlay

Target packages ---> 
  Development tools --->
      [*] binutils
      [*] git
      [*] grep
      [*] make
      [*] tree
  Hardware handling --->
      Firmware --->
          [*] brcmfmac-sdio-firmware-rpi
              [*] brcmfmac-sdio-firmware-rpi-bt
              [*] brcmfmac-sdio-firmware-rpi-wifi
      [*] minicom
  Networking applications ---> 
      [*] bluez_utils
      [ ] hostapd
      [ ] iw
      [*] net-tools
      [*] openssh
      [*] wpa_supplicant
  Shell and utilities --->
      [*] sudo
  Text editors and viewers --->
      [*] nano
```

- **3.2 Modification des fichiers de configuration**

<div style="text-align: justify"> Pour apporter des modifications aux fichiers de configuration du Raspberry Pi tout en préservant les valeurs par défaut, il est recommandé de créer des fichiers de configuration personnalisés sans altérer les fichiers originaux. Afin de simplifier les étapes suivantes, ces fichiers personnalisés sont placés directement dans le répertoire de Buildroot. De plus, un lien symbolique est établi dans le répertoire du projet, permettant un accès rapide et simplifié à ces fichiers tout en maintenant une organisation claire et cohérente des ressources. </div>

```bash
sudo cp ~/buildroot/board/raspberrypi3-64/config_3_64bit.txt board/raspberrypi3-64/config-jdg2025.txt
sudo cp ~/buildroot/board/raspberrypi3-64/post-build.sh board/raspberrypi3-64/post-build-jdg2025.sh
```

<div style="text-align: justify"> Afin d'intégrer les nouveaux fichiers de configuration créés, en remplacement des valeurs par défaut, il est nécessaire de modifier les chemins d'accès dans les options suivantes. Cette étape permet d'assurer que le système utilise les fichiers personnalisés, garantissant ainsi que les configurations spécifiques sont 
correctement appliquées. </div>

```bash
System configuration --->
    Custom scripts to run before creating filesystem images --->
        board/raspberrypi3-64/post-build-jdg2025.sh

Target packages ---> 
    Hardware handling ---> 
        Firmware ---> 
            Path to a file stored as boot/config.txt --->
                board/raspberrypi3-64/config-jdg2025.txt
```

- **3.3 Implémentation des applications**

<div style="text-align: justify"> . </div> 

```bash

```

- **3.4 Configuration du lancement des applications au démarrage du système**

<div style="text-align: justify"> Buildroot prend en charge plusieurs systèmes d'initialisation. Le plus courant pour les systèmes embarqués est BusyBox init, car il est léger et simple à configurer. Les scripts de démarrage sont définis dans /etc/init.d/. Vous devrez créer un script qui démarre votre application.</div>

```bash
mkdir -p ~/buildroot/board/raspberrypi3-64/rootfs_overlay/etc/init.d/
touch ~/buildroot/board/raspberrypi3-64/rootfs_overlay/etc/init.d/S99jdg2025
chmod +x ~/buildroot/board/raspberrypi3-64/rootfs_overlay/etc/init.d/S99jdg2025
```

Le script utilisé est le suivant :
```bash
#!/bin/sh
case "$1" in
start)
    echo "JDG2025: Démarrage de l'application..."
    /usr/bin/blackbox &
    /usr/bin/controller &
    ;;
stop)
    echo "JDG2025: Arrêt de l'application..."
    killall blackbox
    killall controller
    ;;
restart)
    $0 stop
    $0 start
    ;;
*)
    echo "Usage: $0 {start|stop|restart}"
    exit 1
    ;;
esac
exit 0
```

<div style="text-align: justify"> Il peut être avantageux de désactiver le menu de connexion au démarrage du système afin d'établir une connexion automatique avec les privilèges administrateur. Cette configuration simplifie l’accès et accélère le processus de mise en route, particulièrement dans des environnements contrôlés. Les modifications nécessaires ont déjà été apportées dans la section 3.1. </div> 

*Référence : https://unix.stackexchange.com/questions/299408/how-to-login-automatically-without-typing-the-root-username-or-password-in-build?newreg=06001a18d3f048d9a288885fd9b9da63*

- **3.5 Configuration du Wi-Fi**

<div style="text-align: justify"> Pour ajouter une configuration réseau Wi-Fi permettant au Raspberry Pi de se connecter à Internet </div>

```bash
sudo mkdir -p ~/buildroot/board/raspberrypi3-64/etc/wpa_supplicant/
sudo touch ~/buildroot/board/raspberrypi3-64/etc/wpa_supplicant/wpa_supplicant.conf
```

<div style="text-align: justify"> Exemple de fichier de configuration Wi-Fi : </div>

```bash
network={
    ssid="your_network_name"
    psk="your_network_password"
}
```

<div style="text-align: justify"> Pour que la connexion au Wi-Fi se fasse automatiquement, il faut inclure un script au démarrage. </div>

```bash
mkdir -p ~/buildroot/board/raspberrypi3-64/rootfs_overlay/etc/init.d/
touch ~/buildroot/board/raspberrypi3-64/rootfs_overlay/etc/init.d/S99wifi
chmod +x ~/buildroot/board/raspberrypi3-64/rootfs_overlay/etc/init.d/S99wifi
```

Le script utilisé est le suivant :
```bash
#!/bin/sh
ifconfig wlan0 up
wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf
dhclient wlan0
```

- **3.6 Configuration du service SSH**

<div style="text-align: justify"> Pour faciliter l'accès sans fil au Raspberry Pi via SSH, il est recommandé d'ajouter immédiatement la clé publique de l'hôte. Cette démarche permet d'établir une connexion sécurisée et d'éviter toute demande d'authentification manuelle lors des futures connexions, assurant ainsi une gestion simplifiée et plus fluide des accès distants. </div>

```bash
sudo mkdir -p ~/buildroot/board/raspberrypi3-64/etc/ssh/
sudo cp ~/.ssh/id_rsa.pub ~/buildroot/board/raspberrypi3-64/etc/ssh/authorized_keys
```

- **3.7 Construction de l'image du système**

<div style="text-align: justify"> Une fois toutes les options et paquets correctement configurés, la prochaine étape consiste à construire l'image du système embarqué personnalisé. Cette image, une fois générée, sera entièrement opérationnelle et prête à être transférée sur une carte SD. Cependant, il est important de noter que cette opération peut prendre un temps considérable, en fonction de la complexité du système et des ressources nécessaires pour compiler et assembler l'image. </div>

```bash
make
```

### 4. Préparation de la carte SD

<div style="text-align: justify"> Une fois la compilation terminée, l'image peut être transférée sur une carte SD en utilisant l'application Raspberry Pi Imager. Pour ce faire, il suffit d'accéder directement au répertoire de Buildroot sur WSL, puis de sélectionner l'image générée pour la flasher sur la carte SD. Cette méthode permet de préparer rapidement le système embarqué pour son utilisation sur le Raspberry Pi. </div>

```bash
Raspberry Pi Imager ---> 
  Raspberry Pi Device --->
    Raspberry Pi 3
  Operating System --->
    Use custom --->
      \\wsl.localhost\Ubuntu\home\sebas\buildroot\output\images\sdcard.img
  Storage --->
    SD card
```

### 5. Sauvegarde de la configuration dans le répertoire du projet

<div style="text-align: justify"> Pour garantir une maintenance efficace et permettre d’éventuelles améliorations futures du projet, l’image générée ainsi que les fichiers de configuration associés sont systématiquement sauvegardés dans le répertoire du projet. Cette approche assure une accessibilité optimale pour les modifications ultérieures et facilite la gestion des versions. </div>

```bash

```