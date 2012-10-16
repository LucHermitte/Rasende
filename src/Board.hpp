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
    /**\ingroup gCore
     * Compact storage of the board configuration (walls).
     */
    struct Board // : noncopyable
    {
        /// The game dimension is 16 x 16
        static const unsigned char k_width = 16;

        /// Accessor the number of lines
        static Line nb_lines()            { return k_width; }
        /// Accessor the number of columns
        static Column nb_columns()        { return k_width; }
        /// Accessor the possible number of horizontal walls on a colunm.
        static Line nb_H_walls()          { return k_width-1; }
        /// Accessor the possible number of vertical walls on a line.
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
        /**
         * Returns the offset in \c m_board where a horizontal wall will be
         * stored.
         * @param[in] l  line coordinate of the horizontal wall
         * @param[in] c  colunm coordinate of the horizontal wall
         *
         * @return the associated offset
         * @throw None
         * \pre <tt>l < nb_H_walls()</tt>
         * \pre <tt>c < nb_columns()</tt>
         * \note the special case of the last horizontal wall is handled
         * directly by \c hasHorizontalWall().
         */
        static size_t offset_for_H_walls(Line l, Column c)
        {
            assert(l < nb_H_walls() && "Invalid value for H wall x-pos");
            assert(c < nb_columns() && "Invalid value for H wall y-pos");
            const size_t offset
                = (k_width-1)*k_width // H walls are in the second half
                + c * (k_width-1)
                + l ;
            return offset;
        }
        /**
         * Returns the offset in \c m_board where a vertical wall will be
         * stored.
         * @param[in] l  line coordinate of the vertical wall
         * @param[in] c  colunm coordinate of the vertical wall
         *
         * @return the associated offset
         * @throw None
         * \pre <tt>l < nb_lines()</tt>
         * \pre <tt>c < nb_V_walls()</tt>
         * \note the special case of the last vertical wall is handled
         * directly by \c hasVerticalWall().
         */
        static size_t offset_for_V_walls(Line l, Column c)
        {
            assert(l < nb_lines() && "Invalid value for V wall x-pos");
            assert(c < nb_V_walls() && "Invalid value for V wall y-pos");
            const size_t offset = 0 // V walls are in the first half
                + l * (k_width-1)
                + c ;
            return offset;
        }
        /** Internal structure for walls.
         * - first half = vertical walls
         * - second half = horizontal walls.
         * \see \c offset_for_H_walls() and \c offset_for_V_walls() to see how
         * they are actually organized in the bitset.
         */
        std::bitset<(k_width-1)*k_width*2> m_board;

        friend std::ostream & operator<<(std::ostream& os, Board const& b) ;
    };

} // namespace rr

#endif // RR_BOARD_HPP__
