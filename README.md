This program is a solver for Rasende Roboter (a.k.a. Ricochet Robot).


«RR = puzzle ...»
    http://jeux.prise2tete.fr/p2t-ricochet/p2t-ricochet.php5
    http://www.trictrac.net/index.php3?id=jeux&rub=detail&inf=detail&jeu=2259
    http://www.trictrac.net/index.php3?id=jeux&rub=detail&inf=detail&jeu=203/

    another solver here: http://aldel.com/freeware/ricochet.html

The solver looks, for each robot, for the best solution that brings that
robot to the goal.

How to use the solver:
----------------------
The game takes as argument a maze definition in ASCII art where:
  | symbolizes vertical walls
  - symbolizes horizontal walls
  + symbolizes where walls may intersect (mandatory)
  * symbolizes robots' goal
  G, R, V, and B symbolize the 4 robots.

When executed without any argument, the program will echo an empty maze.


How the solver works:
---------------------
The solver does a breadth-first exploration of all possible positions
for the robots.
Unlike heuritics used by human players, this way of processing is
likelly to take a longer time on various mazes. However the solution
given is one optimal solution (actually one solution per robot).

See the «HOW-TO-CONTRIBUTE» document regarding the various (linear)
optimizations performed.
