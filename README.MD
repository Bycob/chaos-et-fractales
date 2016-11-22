
Ce projet a été réalisé dans le cadre du module d'ouverture "Chaos et
fractales".

L'application permet de simuler des systèmes à plusieurs corps physiques
et de les rendre en 3 dimensions.

Compilation des sources
=======================

Dépendances
-----------
Pour pouvoir compiler le projet, il est nécessaire d'installer les librairies
suivantes :
* GLFW (http://www.glfw.org/download.html)
* OpenCV (http://opencv.org/downloads.html)

Pour installer les librairies ci-dessus, se réferer à la documentation respective
de chacune d'elles.

Compilation
-----------
Le projet **chaos et fractales** utilise CMake pour génerer les fichiers de
build. Pour installer CMake, c'est ici : https://cmake.org/download/.

Une fois que vous avez téléchargé les sources du projet, créez un dossier build...
quelque part, et, dans ce répertoire, executer les commandes :
```
cmake <chemin_vers_les_sources>
make
```
Si CMake ne trouve pas GLFW, ou OpenCV, ou les deux, vous pouvez indiquer les
chemins de leurs dossiers d'installation respective comme il suit :
```
cmake -DCMAKE_PREFIX_PATH="<chemin_vers_glfw;chemin_vers_opencv>"
```
