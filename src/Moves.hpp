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
#ifndef RR_MOVES_HPP__
#define RR_MOVES_HPP__

#include <vector>
#include <iosfwd>
#include <string>

#include "Board.hpp"
#include "Position.hpp"
#include "Robot.hpp"

namespace rr { 

    namespace Directions
    {
        typedef unsigned char directions;
        enum types { 
            UP=1, DOWN=2, 
            LEFT=4, RIGHT=8
        };
        static directions opposite(directions dir_) {
            // const directions ud = UP | DOWN;
            // const directions lr = LEFT | RIGHT;

            const directions u = (dir_ & DOWN ) / 2;
            const directions d = (dir_ & UP   ) * 2;
            const directions l = (dir_ & RIGHT) / 2;
            const directions r = (dir_ & LEFT ) * 2;
            return u | d | l | r;
        }
        static std::string toString(Directions::types t_) {
            switch (t_)
            {
                case UP: return "^";
                case DOWN: return "v";
                case LEFT: return "<-";
                case RIGHT: return "->";
                default:
                            assert(!"multiple direction at once can't be printed");
                            return "@@";
            }
        }
    }

    struct Move {
        Move(Directions::types direction_, Position const& p_);
        Directions::types direction;
        Position          position;
    };

    inline unsigned char bitof(unsigned char raw) {
        // http://graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
        assert(raw && !(raw & (raw - 1)) && "Not a power of two (<=> one and only one robot set");
        unsigned char c = 0;
        for (; raw && raw!=1; raw >>= 1, ++c)
        {
        }
        return c;
    }
    struct RobotMove {
        typedef unsigned char byte;
        RobotMove()
            : m_raw(0)
            {}
        RobotMove(Robot::names r_, Directions::types dir_)
            : m_raw((dir_<<4) | (1<<r_))
            {
                assert(robot() == r_);
                assert(direction() == dir_);
            }

        Robot::names robot() const {
            const byte r_mask = 0x0f;
            const byte v = m_raw & r_mask;
            const Robot::names r = Robot::names( bitof(v));
            return r;
        }

        Directions::types direction() const {
            using namespace Directions;
            const byte d_mask = UP | DOWN | LEFT | RIGHT;
            const types dir = types((m_raw>>4) & d_mask);
            return dir;
        }
    private:
        byte m_raw;
    };

    /** Analyse the board to list all possible moves from any position.
     */
    struct PossibleMoves
    {
        typedef std::vector<Move> Moves;
        PossibleMoves(Board const& board_) ;

        Moves const& moves(Position const& p_) const 
		{ return m_moves[p_.l.v][p_.c.v]; }
    private:
        Moves & moves(Position const& p_)
		{ return m_moves[p_.l.v][p_.c.v]; }

        Moves m_moves[Board::k_width][Board::k_width];
    };

    std::ostream & operator<<(std::ostream &os, PossibleMoves const& pm_);
    
} // rr namespace 


#endif // RR_MOVES_HPP__
