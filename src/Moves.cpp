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
#include "Moves.hpp"

#include <ostream>

/*===========================================================================*/
/*==================================[ «» ]===================================*/
/*===========================================================================*/

namespace { // anonymous namespace 
    rr::Position next_left_from(rr::Board const& b_, rr::Position const& p_) {
        const rr::Line   l = p_.l;
        rr::Column c = p_.c;
        for (; rr::Column(0)<c ; --c)
        {
            const rr::Column c2 = c - rr::Column(1);
            if (b_.hasVerticalWall(l,c2))
                break;
        }
        return rr::Position(l,c);
    }

    rr::Position next_right_from(rr::Board const& b_, rr::Position const& p_) {
        const rr::Line   l = p_.l;
        rr::Column c = p_.c;
        for (; c<rr::Board::nb_V_walls() ; ++c)
        {
            if (b_.hasVerticalWall(l,c))
                break;
        }
        return rr::Position(l,c);
    }

    rr::Position next_up_from(rr::Board const& b_, rr::Position const& p_) {
        const rr::Column c = p_.c;
        rr::Line   l = p_.l;
        for (; rr::Line(0) < l; --l)
        {
            const rr::Line l2 = l-rr::Line(1);
            if (b_.hasHorizontalWall(l2,c))
                break;
        }
        return rr::Position(l,c);
    }

    rr::Position next_down_from(rr::Board const& b_, rr::Position const& p_) {
        const rr::Column c = p_.c;
        rr::Line   l = p_.l;
        for (; l<rr::Board::nb_H_walls() ; ++l)
        {
            if (b_.hasHorizontalWall(l,c))
                break;
        }
        return rr::Position(l,c);
    }
} // anonymous namespace 





rr::PossibleMoves::PossibleMoves(Board const& board_)
{
    for (Line l=0; l!=board_.nb_lines(); ++l) {
        for (Column c=0; c!=board_.nb_columns() ; ++c) {
            const Position p0(l,c);
            Moves & m = moves(p0);

            // check UP
            {
                const Position pu = next_up_from(board_, p0);
                if (p0 != pu)
                    m.push_back(Move(Directions::UP, pu));
            }

            // check DOWN
            {
                const Position pd = next_down_from(board_, p0);
                if (p0 != pd)
                    m.push_back(Move(Directions::DOWN, pd));
            }

            // check LEFT
            {
                const Position pl = next_left_from(board_, p0);
                if (p0 != pl)
                    m.push_back(Move(Directions::LEFT, pl));
            }

            // check RIGHT
            {
                const Position pr = next_right_from(board_, p0);
                if (p0 != pr)
                    m.push_back(Move(Directions::RIGHT, pr));
            }
        }
    }
}

rr::Move::Move(Directions::types direction_, Position const& p_)
: direction(direction_), position(p_)
{
}

std::ostream & rr::operator<<(std::ostream &os, rr::PossibleMoves const& pm_)
{
    for (rr::Line l=0; l<rr::Board::nb_lines() ; ++l)
    {
        for (rr::Column c=0; c<rr::Board::nb_columns() ; ++c)
        {
            const rr::Position p = Position(l,c);
            rr::PossibleMoves::Moves const& m = pm_.moves(p);
            os << p << " --->  ";
            for (rr::PossibleMoves::Moves::const_iterator b = m.begin(), e = m.end()
                    ; b != e
                    ; ++b
                )
            {
                os << Directions::toString(b->direction) << " " 
                    << b->position
                    << "  |  ";
            }
            os << "\n";
        }
    }
    return os;
}

