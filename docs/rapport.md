# Rapport technique - Radar et comptage de personnes

Auteurs : MULLER Lucie, LONGA Benjamin

Ce projet a pour but de permettre la prise d'images ainsi que la détection de présence de personnes tout en assurant la collecte de ces données.

## Matériel utilisé

Le matériel utilisé pour la prise d'images et le calcul de présence de personnes est un Raspberry Pi 3 équipé d'une RAPICam et d'une caméra infrarouge Grid-EYE.
Une machine distante permet l'hébergement d'une base de données temporelles InfluxDB ainsi que d'une instance Grafana permettant une bonne lisibilité des données enregistrées. Les images envoyées sont également stockées sur cette machine distante. Dans notre cas, tout a été effectué en local.

## Programmes exécutés

**Raspberry :**
- Script bash fusion.sh

*Détails de fusion.sh :*
- `cd thermalCam ; make ; cd ..`
> permet de recompiler le programme si un changement est détecté
- `watch -n 10 'commande'`
> répète la commande 'commande' toutes les 10 secondes
- `thermalCam/thermalCam`
> lance le programme thermalCam, générant une image de sortie testIR.bmp et un fichier de sortie persondata.txt
- `base64 testIR.bmp > testIR.b64`
> encode l'image en base 64
- `mosquitto_pub -h 192.168.0.22 -p 1883 -t 'fusion/ir' -f 'testIR.b64'`
> publie l'image encodée sur la machine distante sous le topic fusion/ir
- `curl -i -XPOST 'http://192.168.0.22:8086/write?db=fusion' --data-binary @persondata.txt`
> inscrit le résultat de la détection dans la base de données

*Détails du programme thermalCam :*
Ce programme effectue une prise d'image avec les deux caméras, puis modifie l'image "classique" pour faire apparaître les zones de chaleur. On y trouve également un test de détection de personnes trivial, à savoir si plus de 10% des pixels voient leur température au dessus de 25°, alors on considère une personne détectée.
Enfin, les fichiers de sortie sont créés, l'un étant l'image modifiée, l'autre un fichier txt dans un format approprié à l'insertion dans une base de données InfluxDB.

**Machine distante** :
	- Script bash subscript.sh

*Détails de subscript.sh :*
- `x=1`
> initialisation de la variable de nommage des images
- `while true ; do ... done`
> boucle infinie
- `mosquitto_sub -t fusion/ir -C 1 > output.b64`
> récupère un seul et unique message (-C 1) concernant le topic fusion/ir et place le contenu dans un fichier output.b64
- `sleep 1`
> permet la création du fichier avant l'exécution de la commande suivante
- `base64 -d output.b64 > images/image${x}.bmp`
> décodage du message et placement du fichier image dans le répertoire correspondant
- `x=$(( $x + 1 ))`
> incrémentation

## Exemple d'exécution

Afin que cette chaîne de récupération et transmission de données fonctionne sans erreur, l'on doit s'assurer qu'une base de données 'fusion' a bien été créée sur la machine distante et qu'elle est accessible.
Il n'y a pas d'ordre d'exécution entre les deux scripts décrits ci-dessus.

Ainsi, voici une description d'un cycle d'exécution :

**Raspberry :**
1. capture d'images, traitement et génération de fichiers
2. encodage en base64 et envoi via mosquitto
3. écriture via curl dans la base de données

**Machine distante :**
1. récupération de l'image en base64
2. décodage et stockage
3. dans notre navigateur, visualisation des données temporelles dans Grafana