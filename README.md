# Attention !

* Si vous utilisez un dépot Git durant le tp, veiller SVP à **ne pas ajouter les fichiers audio .wav sur le git !** pour ne pas charger inutilement le serveur.

* Vous pouvez avancer le TP sur machine virtuelle Linux. Mais attention : les flux audio temps réel posent souvent problème sur machine virtuelle ! Eh oui... une machine virtuelle diminue la réactivité du système et nuit à ses qualité pour le temps réel. Pas d'inquiétude : c'est, en fait, un aspect intéressant à constater / expérimenter, et vous pourrez toujours en séance travailler sur les machine école. 

# Dépendances

Sur les machines école, Linux, tout est installé sauf RTAudio (voir section suivante).

### Sur votre machine Linux personnelle 

Les dépendances suivantes doivent être installées (en tant que `root`, i.e. au niveau Système d'exploitation) avec votre gestionnaire de paquets préféré :

* la librairie Qt (paquets de développement). Sur Ubuntu 22 ce devrait être : `sudo apt install -y qtcreator qtbase5-dev qt5-qmake cmake
`
* la librairie libsndfile (paquets de développement). Sur Ubuntu 22 ce devrait être : `sudo apt-get install libsndfile1-dev`

### Sous MacOS :

Ce TP pourrait bien pouvoir fonctionner sous un MacOS suffisamment récent. A vérifier toutefois... et installation / configuration à gérer en autonomie.

Pour installer les dépendances, vous devriez par exemple pouvoir utiliser le gestionnaire de paquet `brew` https://brew.sh/

	* qt5 dev : https://formulae.brew.sh/formula/qt@5#default
	* libsndfile dev : https://formulae.brew.sh/formula/libsndfile#default

# Installation et configuration de RTaudio

Sur les machines école ou sur vos machines personnelles : 

* télécharger la dernière version de RTaudio : https://github.com/thestk/rtaudio (choisir le package Zip)

* compiler RTaudio sur Linux en local sur votre compte utilisateur (dans le répertoire $HOME/local)
```
	cd RTaudio
	# configurer RTaudio pour installation dans $HOME/local 
	./autogen.sh --prefix=$HOME/local --without-jack --without-pulse --without-oss --with-alsa 
	make 
	make install

	# Vérifier ce qui a été installé
	ls ~/local/include/rtaudio
	ls ~/local/lib
```


* A noter : **La documentation de RTaudio se trouvera dans le répertoire `doc/html` du répertoire de RTAudio téléchargé.**


* Ajouter $HOME/local aux variables d'environnement dans le `~/.bashrc` :

Ouvrir le fichier `~/.bashrc` de votre compte utilisateur avec votre éditeur préféré et y ajouter :

```
	# Ajout de ~/local/lib à LD_LIBRARY_PATH pour le launcher dynamique
	export LD_LIBRARY_PATH="$HOME/local/lib:$LD_LIBRARY_PATH"
```

* Sauver le fichier `~/.bashrc`

* Déloguer et reloguer pour que le .bashrc soit pris en compte

# Pour compiler les projets C++ fournis pour ce TP


* Aller dans le répertoire du programme souhaité (contenant un fichier `.pro`)

* exécuter `qmake` 

Ce programme, fourni avec la librairie d'inteface graphique Qt, génère un fichier `Makefile`  à partir du fichier de configuration de projet logiciel d'extension `.pro`.

* exécuter make

* le programme est prêt à être lancé !
