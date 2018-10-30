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
#ifndef RR_ROBOT_HPP__
#define RR_ROBOT_HPP__

#include <cassert>
#include "Position.hpp"

namespace rr
{ 
    /**\addtogroup gCore
     * @{
     */
    struct Goal;
    struct Robot ;
    bool operator==(Robot const& lhs, Robot const& rhs) ;
    bool operator!=(Robot const& lhs, Robot const& rhs) ;
    bool operator<(Robot const& lhs, Robot const& rhs) ;
    bool operator==(Goal const& lhs, Robot const& rhs) ;
    std::ostream & operator<<(std::ostream & os, Robot const& r) ;

    /** %Robot definition.
     * \note Comparing robots only consider their position, not their id.
     */
    struct Robot {
        Robot() {}
        Robot(Line l, Column c) ;
        void put_at(Line l, Column c) ;

        /** There are 4 robots.  */
        enum names { Rouge, Gris, Vert, Bleu, MAX__ };
        /** Iteration on robots set.  */
        static names next(names n_) {
            assert(n_ < MAX__);
            return names(n_+1);
        }
        /// Obtains a robot id (C++ enum) from its designation letter.
        static names char2robot(char c) ;
        /// Obtains a robot designation letter from its id (C++ enum).
        static char toString(names n) ;

        /// Line accessor.
        Line            l   () const { return p.l;}
        /// Column accessor.
        Column          c   () const { return p.c;}
        /// Position accessor.
        Position const& pos () const { return p;}
        /** Hash code associated to its position.
         * As the board is expected to be 16x16, the hash returned in 16 times
         * the line number plus the column number. 
         */
        size_t          hash() const { return l()*16 + c(); }

    private:
        Position p;
        friend bool rr::operator==(Robot const& lhs, Robot const& rhs)
#if   !defined(_MSC_VER)
        {
            return lhs.p == rhs.p;
        }
#else
        ;
#endif
        friend bool rr::operator!=(Robot const& lhs, Robot const& rhs)
#if   !defined(_MSC_VER)
        {
            return lhs.p != rhs.p;
        }
#else
        ;
#endif
        friend bool rr::operator<(Robot const& lhs, Robot const& rhs)
#if   !defined(_MSC_VER)
        {
            return lhs.p < rhs.p;
        }
#else
        ;
#endif
        friend bool rr::operator==(Goal const& lhs, Robot const& rhs) ;
        friend std::ostream & rr::operator<<(std::ostream & os, Robot const& r)
#if   !defined(_MSC_VER)
        {
            return os << r.p;
        }
#else
        ;
#endif
    };
    inline
        Robot::names& operator++(Robot::names & n_) {
            n_ = Robot::next(n_);
            return n_;
        }
#if defined(_MSC_VER)
    inline
        bool rr::operator==(Robot const& lhs, Robot const& rhs) {
            return lhs.p == rhs.p;
        }
    inline
        bool rr::operator!=(Robot const& lhs, Robot const& rhs) {
            return lhs.p != rhs.p;
        }
    inline
        bool rr::operator<(Robot const& lhs, Robot const& rhs) {
            return lhs.p < rhs.p;
        }
    inline
        std::ostream & rr::operator<<(std::ostream & os, Robot const& r) {
            return os << r.p;
        }
#endif

    /** %Goal where robots must go.
     */
    struct Goal
    {
        void set(Line l, Column c) ;

    private:
        Position p;
        friend bool rr::operator==(Goal const& lhs, Robot const& rhs) ;
    };
    inline
        bool operator==(Goal const& lhs, Robot const& rhs) {
            return ! (lhs.p != rhs.p);
        }

    //@}
} // rr namespace 


#endif // RR_ROBOT_HPP__
