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
#ifndef RR_BOARD_HPP__
#define RR_BOARD_HPP__

#include <bitset>
#include <cassert>

#include "Number.hpp"

namespace rr {
    /**
     * Stores the board configuration (walls).
     */
    struct Board // : noncopyable
    {
        static const unsigned char k_width = 16;

        static Line nb_lines()            { return k_width; }
        static Column nb_columns()        { return k_width; }
        static Line nb_H_walls()          { return k_width-1; }
        static Column nb_V_walls()        { return k_width-1; }

        /**@name Accessors and mutators */
        //@{
        bool hasVerticalWall(Line l, Column c) const {
            if (c==nb_V_walls()) return true;
            const size_t offset = offset_for_V_walls(l,c);
            return m_board[offset];
        }
        void putVerticalWall(Line l, Column c) {
            const size_t offset = offset_for_V_walls(l,c);
            m_board[offset] = true;
        }
        bool hasHorizontalWall(Line l, Column c) const {
            if (l==nb_H_walls()) return true;
            const size_t offset = offset_for_H_walls(l,c);
            return m_board[offset];
        }
        void putHorizontalWall(Line l, Column c) {
            const size_t offset = offset_for_H_walls(l,c);
            m_board[offset] = true;
        }
        //@}

        void display_H_border(std::ostream & os) const ;
        void display_H_step(std::ostream & os, Line l) const ;

    private:
        static size_t offset_for_H_walls(Line l, Column c)
        {
            assert(l < nb_H_walls() && "Invalid value for H wall x-pos");
            assert(c < nb_columns() && "Invalid value for H wall y-pos");
            const size_t offset
                = (k_width-1)*k_width
                + c * (k_width-1)
                + l ;
            return offset;
        }
        static size_t offset_for_V_walls(Line l, Column c)
        {
            assert(l < nb_lines() && "Invalid value for V wall x-pos");
            assert(c < nb_V_walls() && "Invalid value for V wall y-pos");
            const size_t offset = 0
                + l * (k_width-1)
                + c ;
            return offset;
        }
        /** Internal structure for walls.
         * first half = vertical walls
         * second half = horizontal walls.
         */
        std::bitset<(k_width-1)*k_width*2> m_board;

        friend std::ostream & operator<<(std::ostream& os, Board const& b) ;
    };

} // namespace rr

#endif // RR_BOARD_HPP__
