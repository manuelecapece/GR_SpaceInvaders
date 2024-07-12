# ITA
## Progetto Esame Grafica tridimensionale avanzata anno 2023/24 - Università degli studi della Basilicata.

Questo repository contiene il progetto svolto per l'esame del corso di Grafica Tridimensionale Avanzata presso l'Università degli studi della Basilicata tenuto dal prof Ugo Erra.
Il progetto prevede di effettuare una remaster in 3D del famoso gioco Space Invaders.

## Istruzioni esecuzione codice - metodo 1
1) Scaricare Visual Studio 
2) Scaricare il repository contenente il progetto
3) Importare il progetto in VS come soluzione
4) Eseguire il main

## Istruzioni esecuzione codice - metodo 2
1) Scaricare il repository contenente il progetto
2) Posizionarsi nella cartella spaceInvaders/Eseguibile/game
3) Eseguire SpaceInvaders.exe

## Space invaders
Una divertente remaster in chiave moderna del celebre gioco arcade Space Invaders, 1978 si Tomohiro Nishikado e Taito.

![](https://github.com/manuelecapece/GR_SpaceInvaders/blob/actionCamera/screen/Release%20Screenshot%202024.07.12%20-%2009.26.05.50.png)

Il gioco offre la possibilita di giocare con la tradizionale vista 2D dall'alto oppure con una più avvincente vista 3D che offre una prospettiva in terza persona della navicella.

### Branch main
  - Nel branch principale all'avvio del gioco viene chiesta quale delle due camere utilizzare durante il gameplay.
### Branch actionCamera
  - In questo branch non è possibile scegliere la camera, bensì questa è dinamica e varia durante il gameplay. In questo branch la vista viene settata a 2D all'inizio di ogni livello.
  - La camera 3D si attiva quando viene attivato un bonus oppure quando vengono eliminati i 2/3 degli alieni.
  - Il passaggio da una camera all'altra viene effettuato attraverso una transizione omogenea interpolata.
### Bonus
  - Nel gioco sono presenti due bonus che si possono trovare in modo random all'interno degli alieni evidenziati. Ci sono due bonus:
    1) Bonus scudo: per 5 secondi la navicella avrà uno scudo in alpha blending che la renderà immune dai proiettili dei nemici.
    2) Bonus proiettile speciale: la navicella, evidenziata in stencil testing, potrà sparare un colpo indistruttibile che eliminerà tutto quello che incontrerà sul suo cammino, ufo compreso.
### Altri screen
![](https://github.com/manuelecapece/GR_SpaceInvaders/blob/actionCamera/screen/Release%20Screenshot%202024.07.12%20-%2009.26.16.77.png)
![](https://github.com/manuelecapece/GR_SpaceInvaders/blob/actionCamera/screen/Release%20Screenshot%202024.07.12%20-%2009.26.31.39.png)
![](https://github.com/manuelecapece/GR_SpaceInvaders/blob/actionCamera/screen/Release%20Screenshot%202024.07.12%20-%2009.27.21.69.png)
![](https://github.com/manuelecapece/GR_SpaceInvaders/blob/actionCamera/screen/Release%20Screenshot%202024.07.12%20-%2009.27.44.80.png)
![](https://github.com/manuelecapece/GR_SpaceInvaders/blob/actionCamera/screen/Release%20Screenshot%202024.07.12%20-%2009.29.26.53.png)
![](https://github.com/manuelecapece/GR_SpaceInvaders/blob/actionCamera/screen/Release%20Screenshot%202024.07.12%20-%2009.29.40.34.png)

# ENG
## Advanced Three-Dimensional Graphics Exam Project 2023/24 - University of Basilicata

This repository contains the project carried out for the Advanced Three-Dimensional Graphics course exam at the University of Basilicata, taught by Prof. Ugo Erra. The project involves creating a 3D remaster of the famous game Space Invaders.

## Code Execution Instructions - Method 1
1) Download Visual Studio
2) Download the repository containing the project
3) Import the project into VS as a solution
4) Run the main

## Code Execution Instructions - Method 2
1) Download the repository containing the project
2) Navigate to the folder spaceInvaders/Executable/game
3) Run SpaceInvaders.exe

## Space Invaders
A fun modern remaster of the famous arcade game Space Invaders, 1978 by Tomohiro Nishikado and Taito. The game offers the ability to play with the traditional top-down 2D view or with a more engaging 3D view that offers a third-person perspective of the spaceship.

### Branch main
  - In the main branch, at the start of the game, you are asked which of the two cameras to use during gameplay.
### Branch actionCamera
  - In this branch, it is not possible to choose the camera; instead, it is dynamic and changes during gameplay. In this branch, the view is set to 2D at the beginning of each level.
  - The 3D camera activates when a bonus is triggered or when 2/3 of the aliens are eliminated.
  - The switch from one camera to the other is done through a smooth interpolated transition.
### Bonus
  - The game features two bonuses that can be randomly found within highlighted aliens. There are two bonuses:
    1) Shield Bonus: for 5 seconds, the spaceship will have an alpha blending shield that makes it immune to enemy bullets.
    2) Special Bullet Bonus: the spaceship, highlighted through stencil testing, can fire an indestructible shot that will eliminate everything in its path, including UFOs.



