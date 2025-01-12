## TP - Création d'un Shell Personnalisé

Ce travail pratique (TP) est réalisé dans le cadre d'un cours de systèmes d'exploitation. L'objectif principal de ce TP est de développer un interpréteur de commandes personnalisé (shell) qui permet d'exécuter les commandes du système, tout en reproduisant les fonctionnalités essentielles d'un shell UNIX.

### Objectifs Pédagogiques

Les objectifs pédagogiques de ce TP sont les suivants :
1. **Création de processus** : Appliquer les mécanismes vus en cours pour créer des processus via les appels systèmes.
2. **Gestion des processus** : Assurer une gestion correcte des processus pour éviter les zombies.
3. **Redirection et Pipes** : Implémenter les redirections d'entrées/sorties (symboles `>`, `>>`, et `<`) et les tubes (pipes, symbole `|`).
4. **Jobs en tâche de fond** : Permettre l'exécution de processus en tâche de fond avec le symbole `&`.
5. **Gestion des signaux** : Manipuler les signaux (SIGTERM, SIGQUIT, SIGINT, SIGHUP) pour une gestion correcte des processus et de l'interaction avec le shell.

### Fonctionnalités à Implémenter

Le shell développé devra être capable de :
1. Exécuter des commandes système standards.
2. Gérer des commandes intégrées (builtin) comme `exit` et `cd`.
3. Manipuler les redirections d'entrée/sortie et les pipes.
4. Gérer l'exécution de jobs en tâche de fond.
5. Manipuler et gérer les signaux pour assurer un comportement optimal du shell.

### Contraintes Techniques

- Un seul job en tâche de fond est autorisé à la fois.
- Les commandes intégrées (builtin) ne doivent jamais être lancées en tâche de fond.
- Une gestion stricte des processus est requise pour éviter les zombies.
- Le parsing des commandes et la gestion des redirections et pipes doivent être réalisés conformément à des règles spécifiques.

Vous pouvez retrouver le pdf [ici](/TP_shell.pdf)
