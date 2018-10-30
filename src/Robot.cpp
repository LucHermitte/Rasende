/*
   Rasende Roboter Solver, GPL Source Code

   This file is part of the Rasende Roboter Solver Source Code

   Rasende Roboter Solver Source Code is free software: you can
   redistribute it and/or modify it under the terms of the GNU General
   Public License as published by the Free Software Foundation, either
   version 3 of the License, or (at your option) any later version.

   Rasende Roboter Solver  Source Code is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
   the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Rasende Roboter Solver Source Code. If not, see
   <http://www.gnu.org/licenses/>.
 */

/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "Robot.hpp"

#include <cassert>


/*===========================================================================*/
/*==================================[ «» ]===================================*/
/*===========================================================================*/

rr::Robot::Robot(Line l, Column c)
{
    p = Position(l,c);
}

void rr::Robot::put_at(Line l, Column c)
{
    p = Position(l,c);
}

/*static*/ rr::Robot::names rr::Robot::char2robot(char c)
{
    switch (c)
    {
        case 'b': case 'B': return Bleu;
        case 'r': case 'R': return Rouge;
        case 'v': case 'V': return Vert;
        case 'g': case 'G': return Gris;
        default: assert(!"invalid robot id");
                 return MAX__; // to disable gcc warning
    }
}

/*static*/ char rr::Robot::toString(names n)
{
    switch (n)
    {
        case Vert: return 'V';
        case Bleu: return 'B';
        case Rouge: return 'R';
        case Gris: return 'G';
        default: assert(!"invalid robot id");
                 return MAX__; // to disable gcc warning
    }
}

void rr::Goal::set(Line l, Column c)
{
    p = Position(l,c);
}

