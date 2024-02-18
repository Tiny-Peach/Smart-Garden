Fonctionnalités principales :
Interface Utilisateur avec Tkinter : Une interface graphique intuitive permettant aux utilisateurs de surveiller les conditions environnementales de leur jardin en temps réel.
Simulation Wokwi : Nous avons utilisé Wokwi pour simuler notre système avant de le déployer physiquement.
Capteurs : Des capteurs d'humidité, de lumière et de température sont utilisés pour surveiller les conditions environnementales du jardin.
Communication MQTT : Les données des capteurs sont publiées sur des topics MQTT, permettant une communication efficace entre les différents composants du système.
Contrôle du Servomoteur : Les utilisateurs ont la possibilité de contrôler un servomoteur via l'interface pour automatiser des actions telles que l'arrosage des plantes.

Technologies utilisées :
Python : Utilisé pour le développement de l'interface utilisateur avec Tkinter et pour la logique métier du système.
Wokwi : Plateforme de simulation en ligne utilisée pour tester et valider le fonctionnement du système avant déploiement.
MQTT : Protocole de messagerie utilisé pour la communication entre les capteurs, l'interface utilisateur et le servomoteur.
Capteurs : Des capteurs d'humidité, de lumière et de température sont intégrés au système pour surveiller les conditions du jardin.
