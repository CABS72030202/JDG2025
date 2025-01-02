# Description
La manette choisie pour le contrôle de la solution robotique par le pilote est la Logitech Wireless Gamepad F710.

# Choix du matériel
- **Nombre important de boutons** : permet un contrôle complet et flexible.
- **Facilité d'implémentation** : utilise un récepteur USB pour la communication sans fil, simplifiant l'intégration.

# Configuration
Aucune configuration requise.

# Commandes

| Bouton             | Fonction                                                                 |
|--------------------|-----------------------------------------------------------------------|
| **RB**            | Actionne la roue droite en marche arrière à vitesse constante.           |
| **LB**            | Actionne la roue gauche en marche arrière à vitesse constante.           |
| **RT**            | Actionne la roue droite en marche avant à une vitesse proportionnelle à l’enfoncement de la gâchette. |
| **LT**            | Actionne la roue gauche en marche avant à une vitesse proportionnelle à l’enfoncement de la gâchette. |
| **Croix directionnelle** | Oriente le robot à une vitesse constante.                                |
| **Y**       | Alterne entre deux valeurs prédéfinies pour la vitesse constante (vitesse d’approche ou vitesse normale). |
| **Bouton Back**    | Sélectionne le robot précédent pour le contrôle.                             |
| **Bouton Start**   | Sélectionne le robot suivant pour le contrôle.                               |
| **Bouton Home**    | Signale que le robot est prêt à être connecté à sa station.                  |
| **Gâchette gauche** | Commande le bras du Robot Cône.                                            |
| **Gâchette droite** | Commande la pince du Robot Cône.                                           |
| **A + Bouton gâchette gauche**       | Secret réservé au maître programmeur ! |
| **B + Bouton gâchette droite**       | Réinitialisation de la communication Bluetooth en cas d'erreur. |
| **X + Bouton gâchette droite**       | Réinitialisation des moteurs sans balais du bateau en cas de dysfonctionnement. |
| **X + Bouton gâchette gauche**       | Modification des moteurs du bateau : passage des hélices aux roues pour le contrôle. |
| **B + X + Bouton gâchette gauche**       | Ouverture des rampes du bateau pour permettre aux robots de débarquer. |

### Notes au pilote : 
- Pour mémoriser plus facilement les commandes moins intuitives et plus complexes utilisant les gâchettes, il est utile de recourir à ce moyen mnémotechnique : **L**eft pour critica**L** et **R**ight pour **R**eset.
- Si un robot continue d'avancer alors qu'il est censé s'arrêter, la méthode la plus efficace pour corriger ce comportement consiste à appuyer sur la touche **Y** afin de modifier la vitesse de base. Cette action permet de lui renvoyer la commande d'arrêt sans provoquer de déplacement involontaire.