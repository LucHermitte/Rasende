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
#ifndef RR_POSITION_HPP__
#define RR_POSITION_HPP__

#include <ostream>
#include "Number.hpp"

namespace rr {
    /**\addtogroup gCore
     * @{
     */
    /** %Position on the \c Board. 
     * A position is a pair of coordinates: line + colunm.
     *
     * Semantics: <em>Regular type</em>.
     */
    struct Position
    {
        Line l;
        Column c;
        Position() : l(0), c(0) {}
        Position(Line l_, Column c_) : l(l_), c(c_) {}
    };

    inline
    bool operator!=(Position const& lhs, Position const& rhs) {
        return (lhs.l != rhs.l) || (lhs.c != rhs.c);
    }
    inline
    bool operator==(Position const& lhs, Position const& rhs) {
        return (lhs.l == rhs.l) && (lhs.c == rhs.c);
    }
    inline
    bool operator<(Position const& lhs, Position const& rhs) {
        return (lhs.l < rhs.l)
            || (lhs.l == rhs.l && lhs.c < rhs.c);
    }
    inline
    std::ostream & operator<<(std::ostream & os, Position const& p) {
        return os << int(p.l.v) << "," << int(p.c.v);
    }

    //@}
} // namespace rr

#endif // RR_POSITION_HPP__
