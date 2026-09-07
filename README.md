# 2048

Jeu 2048 jouable dans le terminal, codé en C avec la bibliothèque ncurses.

## Le jeu

Un plateau de 4 cases sur 4. Les flèches font glisser toutes les tuiles dans une direction. Deux tuiles identiques qui se rencontrent fusionnent et leur somme s'ajoute au score. Une nouvelle tuile apparaît après chaque coup. La partie s'arrête quand plus aucun mouvement n'est possible.

## Ce qu'on voit à l'écran

Au lancement, deux tuiles sont posées au hasard :

```
====== 2048 ======
Score: 0

+------+------+------+------+
|      |    2 |      |      |
+------+------+------+------+
|      |      |      |      |
+------+------+------+------+
|      |      |    2 |      |
+------+------+------+------+
|      |      |      |      |
+------+------+------+------+

fleches = deplacer  |  q = quitter
```

L'affichage est entièrement redessiné après chaque touche. Il n'y a pas de défilement : `clear()` efface l'écran et le plateau se réaffiche au même endroit.

## Ce qui change quand on appuie sur une touche

### Un coup qui fusionne

Sur la première ligne, les deux `2` sont côte à côte. En appuyant sur **←** :

```
        AVANT                              APRES  ←

Score: 52                              Score: 56

+------+------+------+------+          +------+------+------+------+
|    2 |    2 |    4 |      |          |    4 |    4 |      |      |
+------+------+------+------+          +------+------+------+------+
|      |    8 |      |      |          |    8 |      |      |      |
+------+------+------+------+          +------+------+------+------+
|    4 |      |      |    2 |          |    4 |    2 |      |      |
+------+------+------+------+          +------+------+------+------+
|      |      |      |      |          |      |      |      |      |
+------+------+------+------+          +------+------+------+------+
```

Trois choses se produisent en même temps :

- **Les deux `2` fusionnent** en un `4`, et le `4` qui suivait vient se coller à côté
- **Le score passe de 52 à 56**, la valeur de la tuile créée
- **Toutes les autres tuiles glissent à gauche** : le `8` isolé, le `4` et le `2` de la troisième ligne

Une nouvelle tuile apparaît ensuite sur une des cases vides.

### Un coup vers le bas

Même principe, autre direction. En appuyant sur **↓** :

```
        AVANT                              APRES  ↓

Score: 60                              Score: 76

+------+------+------+------+          +------+------+------+------+
|    2 |      |      |      |          |      |      |      |      |
+------+------+------+------+          +------+------+------+------+
|    2 |    4 |      |      |          |      |      |      |      |
+------+------+------+------+          +------+------+------+------+
|      |    4 |      |    2 |          |    4 |      |      |      |
+------+------+------+------+          +------+------+------+------+
|    8 |      |      |    2 |          |    8 |    8 |      |    4 |
+------+------+------+------+          +------+------+------+------+
```

Trois fusions d'un coup : les deux `2` de la première colonne donnent `4`, les deux `4` de la deuxième donnent `8`, les deux `2` de la dernière donnent `4`. Le score gagne 16 points.

### Un coup qui ne change rien

Si aucune tuile ne peut bouger dans la direction demandée, **rien ne se passe** : l'affichage reste identique et aucune tuile n'apparaît.

C'est le rôle du booléen renvoyé par les fonctions de déplacement. Sans ce test, chaque touche inutile ajouterait quand même une tuile et remplirait le plateau pour rien.

### La fin de partie

Quand plus aucune case n'est vide et qu'aucune voisine n'est identique, la partie s'arrête :

```
====== 2048 ======
Score: 988

+------+------+------+------+
|    2 |    4 |    2 |    4 |
+------+------+------+------+
|    4 |    2 |    4 |    2 |
+------+------+------+------+
|    2 |    4 |    2 |    4 |
+------+------+------+------+
|    4 |    2 |    4 |    2 |
+------+------+------+------+

GAME OVER — Score final: 988
Appuyez sur une touche...
```

## Comment marche le code

**Une seule direction est codée.** Seul le déplacement vers la gauche existe vraiment (`compacter_gauche`). Les trois autres sont obtenus en retournant le plateau, en appliquant le mouvement gauche, puis en le remettant à l'endroit :

- droite = miroir → gauche → miroir
- haut = transposition → gauche → transposition
- bas = transposition + miroir → gauche → miroir + transposition

**Le déplacement se fait en trois temps.** D'abord `tasser` pousse toutes les tuiles vers la gauche pour supprimer les trous. Ensuite on fusionne les paires de voisines identiques. Enfin on tasse à nouveau pour combler les trous laissés par les fusions.

Cet ordre évite les fusions en chaîne : `[2 2 2 2]` donne bien `[4 4 0 0]` et non `[8 0 0 0]`.

**Une tuile n'apparaît que si le plateau a bougé.** Les fonctions de déplacement renvoient 1 en cas de changement.

**ncurses gère l'affichage et le clavier.** Il permet de lire les flèches directement, sans appuyer sur Entrée à chaque coup.

## Les fonctions

| Fonction | Rôle |
|---|---|
| `miroir_lignes` | Inverse chaque ligne horizontalement |
| `symetrie_diagonale` | Transpose le plateau |
| `tasser` | Pousse les tuiles vers la gauche |
| `compacter_gauche` | Tasse, fusionne, retasse |
| `aller_gauche/droite/haut/bas` | Les quatre déplacements |
| `nouvelle_tuile` | Place un 2 sur une case libre au hasard |
| `coup_possible` | Vérifie si la partie peut continuer |
| `dessiner` | Efface l'écran et réaffiche le plateau et le score |

Flèches pour jouer, `q` pour quitter.
