# Doc premake hyper simple

## Ajouter un fichier pour l'utiliser dans le programme

Il faut le mettre dans le dossier `dev-assets/`. L'entièreté du dossier est copiée pour être envoyée dans le même dossier que l'exécutable.

## Comment ça marche premake ?

[Docs de premake](https://premake.github.io/docs/)

Premake est un petit programme tout sympa qui permet de générer des projets pour plein d'IDE différents. En gros ça permet de pouvoir faciliter le partage du projet sur plusieurs plateformes différentes.

Il utilise le fichier `premake5.lua` pour connaître la spécification du projet. J'ai mis plein de jolis commentaires le fichier pour pas être perdu si vous devez vous y aventurer.

### Ajouter une bibliothèque

Pour ajouter une bibliothèque il faut :
- Ajouter les fichiers `*.lib` dans le dossier `libs/windows/` du projet.
- Aller dans `premake5.lua`, dans `filter 'system:windows'` trouver la partie `links`
- Ajouter le nom de la bibliothèque sans l'extension
- ???
- Profit