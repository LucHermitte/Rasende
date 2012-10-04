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
    struct Goal;
    struct Robot ;
    bool operator==(Robot const& lhs, Robot const& rhs) ;
    bool operator!=(Robot const& lhs, Robot const& rhs) ;
    bool operator<(Robot const& lhs, Robot const& rhs) ;
    bool operator==(Goal const& lhs, Robot const& rhs) ;
    std::ostream & operator<<(std::ostream & os, Robot const& r) ;

    /** Robot definition.
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
        static names char2robot(char c) ;
        static char toString(names n) ;

        Line            l   () const { return p.l;}
        Column          c   () const { return p.c;}
        Position const& pos () const { return p;}
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

    /** Goal where robot must go.
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
} // rr namespace 


#endif // RR_ROBOT_HPP__
