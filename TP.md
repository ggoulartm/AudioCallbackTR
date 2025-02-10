# 0. Dépendances et compilation des codes fournis

Rappel : le fichier README.md fourni :
* précise les dépendance de ce TP et comment les installer sur votre machine personnelle si besoin

* explique comment compiler les codes fournis pour réaliser le TP.

# 0. Credits

**Remerciements chaleureux à Eric Hostettler et au groupe musical Fergessen pour les fichiers audio du répertoire AudioFilesSong.**

Il s'agit de pistes séparées du morceau « Nos palpitants » de Fergessen  disponible sur l’album Far Est de 2013 - https://www.fergessen.fr.

**Merci de ne pas diffuser ces fichiers sous copyright**

# 1. Premiers pas

#### Compiler et lancer les programme  `audioprobe` puis `playsaw` (extraits des fichiers exemples de test de la librairie RTAudio)

Pour le programme `playsaw`, utilisez par exemple 2 canaux (**channels**) et une fréquence d'échantillonnage (sample rate) de 44100 Hz.

**Que font ces deux programmes ?**

####  Lire le code de `playsaw`

Aidez vous de la documentation de `RTAudio` pour répondre à ces questions.

 
 **Dans le `main`, repérer la création de l'objet instance de la classe `RtAudio`.**

**Identifier l'appel à la méthode `openStream()` de l'objet instance de la classe `RtAudio. A quoi sert-elle ?**

**Quel est le rôle du 6e argument, appelé  `RtAudioCallback callback` dans la doc ?**

**Quel est le rôle du 7e argument, appelé `userData` dans la doc ?**

**Quel lien faites-vous entre l’argument data du callback inout() et les arguments de la fonction `openStream()` ?**

**Quel est le rôle de la fonction 	`rtaudio_callback` ?** Identifier ses paramètres.

**Que fait le thread principal (dans la fonction `main`) pendant que que le *stream* audio est en cours d'exécution ?**

**A quoi sert l'appel à la fonction `signal` (dans le main) ?**

Par la suite, nous travaillerons avec des échantillons de type `double`. 

En traitement audio, par convention, lorsqu'on utilise des échantillons de type `double`, les valeurs de ces échantillons doivent être comprises entre `-1.0` et `1.0`.


**Dans `playsaw`, configurez le *stream* RtAudio pour fonctionner avec ce format d'échantillon (cf. flag RTAUDIO_FLOAT64). Tester.**


# 2. Ecoutons 2 sinus...

Vous souhaitez (si, si, vraiment !) maintenant envoyer sur la carte audio et écouter deux sons sinusïdaux (sons purs) : un sur la voix droite, l'autre sur la voix gauche.

Les fréquences et amplitudes des deux sinus seront choisis par l'utilisateur en argument sur la ligne de commande.

Les échantillons seront générés dans le callback audio, en appelant la fonction `sin`.

**Un embryon de code est fourni dans le répertoire `playsine`.**

##### Prise en main du code fourni

**Lire le code fourni et repérer ses différents éléments.**

En particulier :

* Repérer la déclaration de la classe `CallbackData`.

Noter que, pour simplifier, tous les attributs de cette classe sont publiques et la classe n'a été dotée d'aucune méthode (cette classe se présente donc comme une structure en `C`).

A quoi sert cette classe dans le code fourni ?

* Observer que les arguments passés au `main` sur la ligne de commande sont récupérés et stockés dans un objet instance d'une classe nommée `CallbackData`.

* Pourquoi le programme est-il arrêté si la fréquence demandée par l'utilisateur pour l'un des deux sinus est supérieure à 22050 Hz ?

* Repérer que le *stream* audio ouvert puis lancé. Remarquer dans le code qu'on passe à la méthode `openStream` l'adresse de l'objet de type `CallbackData`.

* Réperer également comment la fonction `rtaudio_callback` est passée en paramètre à la méthode  `openStream`  (pointeur de fonction).

* Observer comment la fonction `rtaudio_callback` retrouve l'objet de type `CallbackData`.


Dans la version fournie, la fonction *callback* audio `rtaudio_callback` ne fait rien : tous les échantillons envoyés sur la carte son valent `0.` (cf. son code, à lire !).





#### En route pour écouter 2 beaux sinus !

**Quelques explications :**

Si l'on veut générer les échantillons d'un signal (audio) sinusoïdal d'amplitude `amp` à la fréquence `fsin`, et dont les échhantillons sont de type `double`, la valeur de l'échantillon au temps `t` (en seconde) se calcule par :

```
sample_value = amp * sin( 2 * PI * fsin *t)
```

Considérant que les échantillons doivent être envoyés à la fréquence d'échantillonngage `samplerate` (qui vaut pour nous `44100` Hz), la valeur de l'échantillon numéro `i` du signal est donnée par (attention à la division entière !) :

```
sample_value = amplitude * sin( 2 * PI * fsin * ( i / samplerate))
```


**Compléter le callback audio pour générer et envoyer sur la carte son les deux sons sinusoïdaux**

... Noter qu'il sera nécessaire d'ajouter un attribut à la classe `CallbackData`.

**Compiler et tester**

# 3. Ecoutons des fichiers audio

Nous souhaitons maintenant écouter un ou plusieurs fichiers audio simultanément.

**Noter que tous les fichiers audio que nous manipulerons dans ce TP seront monophoniques (un seul canal).**

Dans cette partie, la totalité des échantillons seront préalablement chargés en mémoire (avant de démarrer le *stream* audio).


Un embryon de code est fourni dans le répertoire `playfiles`. 

Dans cet embryon de code :

* Repérer la classe fournie `AudioFileReader` qui permet de lire pas à pas des échantillons audio dans un fichier son (pour info : au moyen de la librairie C *open source* `libsndfile`). Parcourir son fichier header .h et repérer les fonctions disponibles.

* Repérer l'usage de cette classe dans la fonction `main` et la structure de données dans laquelle sont stockés tous les échantillons de tous les fichiers audio à jouer.

* Repérer la déclaration et l'usage du conteneur ("collection") `std::vector`.

*Dans la librairie standard C++, la classe `std::vector` implante le type abstrait "tableau redimensionnable" : ajout d'un élément à la fin en O(1) coût amorti ; accès à un élément en O(1) strict ; etc.*

* Visiter la documentation de la classe C++ `std::vector` (et notamment lire et comprendre l'exemple en bas) : https://en.cppreference.com/w/cpp/container/vector.

* Noter comment, dans le main, tous les échantillons audio de tous les fichiers à lire sont **préchargés en mémoire**. Repérer l'appel à la méthode `readAllFrames()` de l'objet de type `AudioFileReader` et comprendre la structure de données dans laquelle les samples sont stockés en mémoire.

* Noter que, dans le main, le *stream* audio est lancé, mais le *callback* audio ne fait rien (tous les échantillons envoyés sur la carte son valent `0.`).

**Compléter la classe `CallbackData` pour stocker dans l'objet instance de cette classe les données nécessaires à la fonction *callback* du *stream* audio, c'est à dire les échantillons audio chargés en mémoire avant l'ouverture du *stream* audio.**

**Compléter la fonction `rtaudio_callback` (callback audio) pour envoyer sur la carte son les échantillons des fichiers audio à jouer.**

Compiler et tester.

# 4. Ecoutons des fichiers audio - la suite (ou : "comment fait-on en vrai")

**Commencer par copier votre répertoire tplayfiles` dans un novueau réperoire** 

... pour ne pas perdre ce que vous avez réalisé à l'étape précédente !

#### Introduction

Notre implantation précédente est bien sympathique, mais elle pré-charge en mémoire la totalité des fichiers audio.

Dans la vraie vie, on peut avoir affaire à un grand nombre de fichiers (par exemple lorsqu'on mixe un enregistrement d'orchestre) contenant chacun un grand nombre d'échantillons.

L'ensemble peut représenter plusieurs Giga Octets.

Il n'est donc bien sûr pas raisonnable de pré-charger en mémoire la totalité des échantillons audio de tous les fichiers.

Rappelons qu'il n'est pas non plus acceptable de lire "petit à petit" les échantillons depuis la fonction callback audio `rtaudio_callback`.

En effet, une lecture fichier est un appel système, et un *callback* audio, réputé "temps réel", ne doit faire aucun appel système, donc par exemple : pas d'ouverture ou lecture fichier, pas d'allocation dynamique, pas de réservation de mutex ou autre verrous, etc.

Dès lors, comment procéder ?

Il s'agit :

* qu'un autre thread (pour nous, ce sera simplement le thread principal, c'est à dire la pile d'appel de la fonction `main`) lise les fichiers audio petit bout par petit bout et enfile les échantillons lus dans une structure de donnée bien choisie ;

* et que, de son côté, le callback audio vide cette même structure de donnée petit à petit, en consommant les échantillons depuis cette structure de donnée au fur et à mesure qu'ils sont demandés par la carte audio.

La structure de donnée en question se trouve alors au centre d'un mécanisme  **producteur / consommateur**.

Cette structure de donnée doit donc implanter le type abstrait file (*FIFO*) entre le thread principal et la fonction callabck audio (qui vit dans son propre thread) :
* le thread principal (fonction main) enfile dans la file les échanantillons lus dans les fichiers ;
* le thread audio, au travers du callback audio, défile les échantillons de la file.


#### Quelle structure de donnée envisageriez-vous pour implanter la file ?

... en se rappelant que le callback audio n'est pas autorisé à réaliser des allocations dynamiques...

#### Comment proposez vous de gérer les accès concurrent à cette structure de donnée par les deux *threads* en jeu (le *thread* principal de la fonction *main* et le *thread* audio géré par RTAudio) ?

... en se rappelant que le callback audio n'est pas autorisé à acquérir de verrou (mutex par exemple)...


#### Implanter cette structure de donnée dans une nouvelle classe C++


#### Utiliser votre classe pour modifier la lecture et l'écoute de fichiers audio au moyen du mécanisme producteur - consommateur.

Compiler, tester, corriger.

# 5. Interface graphique de lecture et mixage de fichiers audio

A la fin du TP, le code d'un programme créant une interface graphique (avec la librairie Qt) poru la lecture et le mixage basique (volume + balance droite/gauche) de fichiers audio vous sera fourni.

Vous pourrez librement explorer ce code et comprendre comment il fonctionne.

