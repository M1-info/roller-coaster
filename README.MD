# Projet montagnes russes

## Setup

Lancer le script './scripts/script.bat' pour créer le projet.
(Pensez à mettre les droits dans vos fichiers)

Pour le lancer vous devez passer un parametre.
Parmis les choix disponibles :

- vs => il vous créé une solution pour visual studio 2022
- makefile => il vous créé les makefiles pour compiler avec mingw
- runDebug => il vous lance l'executable créé en configuration de debug
- runRelease => il vous lance l'executable créé en configuration release
- clean => il supprimes tous les fichier créés .sln ou makefile

## Description

Projet de simulation de montagnes russes.
La simulation est effectuée à l'aide de OPENGL (avec glad et glfw).
L'objectif était d'avoir un chariot pouvant se déplacer sur des montagnes russes générées à l'aide de courbes (soit Bezier, soit Bspline, soit CatmullRom).
On peut se déplacer librement sur la scène ou fixer la caméra sur le chariot quand il est en mouvement.

## Classes et structure

Les classes principales sont : (classes qui gérent l'affichage)

- Renderer : qui gère l'affichage de la scène
- Camera : qui gère la caméra
- Scene : qui gère la scène
- Light : qui gère la lumière
- Window : qui gère la fenêtre

Les buffers sont tous dans le dossier buffers

- IndexBuffer : qui gère les IBO
- VertexBuffer : qui gère les VBO
- VertexArray : qui gère les VAO
- FrameBuffer : qui gère les FBO
- VertexBufferElement : qui gère les attributs des VBO passés aux shaders

Les courbes sont dans le dossier curves

Il y a une classe principale Curve qui gère les courbes et permet d'abstraire certeines méthodes.
Et trois classes :

- BezierCubic : pour les courbes de bezier
- Bspline : pour les courbes de bspline
- CatmullRom : pour les courbes de catmull rom

Tous les objets de la scène étendent de la classe Mesh
Cette classe permet d'avoir des attributs communs à tous les classes qui héritent de Mesh.

Après, pour chaque objet on a une classe :

- Cart : qui gère le chariot
- Rails : qui gère l'ensemble des rails
- Rail : qui gère un seul rail
- ControlPoint : utilisé par Rails pour générér la courbe
- Ground : pour le terrain
- Skybox : pour le ciel et la cubemap
- Object3D : qui gère les tous les objets qui n'ont pas de logique particulière.

En suite, il y a une classe UI qui permet de gèrer l'interface utilisateur
La classe a été subdivisé en d'autres sous-classe :

- UIScene : qui affiche le graphique de la scène et le rendu du FBO
- UIConsole : qui affiche une console dans l'interface
- UIFilesController : qui est utilisée pour gèrer les fichiers contenant les points de contrôle

Finalement, il y a un dossier utils, qui contient des classes utilitaires

- Material : qui gère les matériaux d'un mesh
- Transform : qui gère les transformations d'un mesh
- Shader : chaque matériau a son propre shader
- OBJLoader : pour charger et lire les fichiers .obj
- Texture et Cubemap : pour charger les textures et les cubemaps
