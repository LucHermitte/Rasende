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
#include "Board.hpp"

#include <ostream>

/*===========================================================================*/
/*==================================[ «» ]===================================*/
/*===========================================================================*/

void rr::Board::display_H_border(std::ostream & os) const
{
    os << "+-";
    for (Column c=0; c!=Board::nb_V_walls(); ++c) os << "+-";
    os << "+\n";
}

void rr::Board::display_H_step(std::ostream & os, Line l) const
{
    os << "| ";
    for (Column c=0; c!=Board::nb_V_walls(); ++c) 
        os << (hasVerticalWall(l,c)?'|':' ') << ' ' ;
    os << "|\n";
}

namespace rr {
    std::ostream & operator<<(std::ostream& os, Board const& b) {
        b.display_H_border(os);

        for (Line l=0 ; l!=Board::nb_H_walls() ; ++l)
        {
            b.display_H_step(os, l);

            for (Column c=0; c!=Board::nb_columns(); ++c) 
                os << '+' << (b.hasHorizontalWall(l,c)?'-':' ');
            os << "+\n";
        }

        b.display_H_step(os, Board::nb_H_walls());
        b.display_H_border(os);
        return os;
    }
} // namespace rr
