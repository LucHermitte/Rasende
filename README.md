This program is a solver for Rasende Roboter (a.k.a. Ricochet Robot).


Rasende Roboter is a puzzle board game by Alex Randolph. For more information,
you can have a look at:
* [Board Game Geek's sheet](http://boardgamegeek.com/boardgame/51/ricochet-robots)
* [Tric Trac's sheet 1](http://www.trictrac.net/index.php3?id=jeux&rub=detail&inf=detail&jeu=2259) (French)
* [Tric Trac's sheet 2](http://www.trictrac.net/index.php3?id=jeux&rub=detail&inf=detail&jeu=203/) (French)


The solver looks, for each robot, for the best solution that brings that
robot to the goal.

#How to use the solver:
----------------------
The game takes as argument a maze definition in ASCII art where:
* `|` symbolizes vertical walls
* `-` symbolizes horizontal walls
* `+` symbolizes where walls may intersect (mandatory)
* `*` symbolizes robots' goal
* `G`, `R`, `V`, and `B` symbolize the 4 robots.

Note: No robot may be on the goal with the current interface specification.

When executed without any argument, the program will echo an empty maze.


#How the solver works:
---------------------
The solver does a breadth-first exploration of all possible positions
for the robots, and prune states already explored, and that are in the list of
states to explore.

Unlike heuristics used by human players, this way of processing is
likely to take a longer time on various mazes. However the solution
given is one optimal solution (actually one solution per robot).

See the «TODO» document regarding the various (linear) optimizations performed.


#Current performances:
---------------------
Under a 32-bit Windows XP PC with 2GB RAM, the solver can explore puzzles that
need up to 17-19 moves. The actual limitation comes from the memory required to
remember states already explored and next states to explore.

The best algorithm (not implemented yet) that I can think about would require
(16\*16)^4 bytes (4GB) + up to several GB for the next states to explore.


#Other links
---------------------
There exist many places where it is possible to play online. You can check the
links from Tric Trac, BGG, or even the following one:
* http://jeux.prise2tete.fr/p2t-ricochet/p2t-ricochet.php5

BTW, there also exist several other solvers. Here is a few I came to notice:
* http://aldel.com/freeware/ricochet.html, which does a depth-first search (and
  as a consequence gives a solution that may not be optimal)
* [Drifting Droids](http://github.com/smack42/DriftingDroids) which is a
  complete game that embeds a solver that gives an optimal solution.


#License
---------------------
Rasende Roboter Solver Source Code is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

Rasende Roboter Solver  Source Code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License along with
Rasende Roboter Solver Source Code. If not, see <http://www.gnu.org/licenses/>.
