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
#ifndef RR_PLATEAU_HPP__
#define RR_PLATEAU_HPP__

#include <iosfwd>

#include "Number.hpp"
#include "Board.hpp"
// #include "Position.hpp"
#include "Robot.hpp"

namespace rr {
    struct Game;
}

std::istream & operator>>(std::istream& is, rr::Game & g) ;
std::ostream & operator<<(std::ostream& os, rr::Game const& g) ;

namespace rr {

/*===========================================================================*/
/*=================================[ Board ]=================================*/
/*===========================================================================*/
    struct State {
        Robot m_robots[4];
        Robot const& operator[](Robot::names r) const { return m_robots[r]; }
        Robot      & operator[](Robot::names r)       { return m_robots[r]; }

        bool hasRobot(Line l, Column c) const {
            const Robot p(l,c);
            for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; ++r)
                if (m_robots[r] == p)
                    return true;
            return false;
        }
    };

    inline
    size_t hash(Robot const& r) {
	return r.l()*16+r.c();
    }
    inline
    size_t hash(State const& p) {
        return ((hash(p[Robot::Rouge])*256+hash(p[Robot::Bleu]))*256+hash(p[Robot::Vert]))*256+hash(p[Robot::Gris]);
    }

    /** Initial Game state (Board+Goal+initial Robots State).
     */
    struct Game {
        void putVerticalWall(Line l, Column c) ;
        bool hasVerticalWall(Line l, Column c) const {
            return m_board.hasVerticalWall(l,c);
        }
        void putHorizontalWall(Line l, Column c) ;
        bool hasHorizontalWall(Line l, Column c) const {
            return m_board.hasHorizontalWall(l,c);
        }
        void putRobot(Line l, Column c, char r) ;
        void setGoal(Line l, Column c) ;

    private:
        State m_start_state;
        Board m_board;
        Goal m_goal;

        void checkCase(std::string const& str, Line l, Column c) ; 

        void readGameLine(std::string const& str, Line l) ;

        friend std::istream & ::operator>>(std::istream& is, Game & g) ;
        friend std::ostream & ::operator<<(std::ostream& os, Game const& g) ;
    };

} // namespace 

#if defined(HAS_STDEXT_HASH_SET)
namespace stdext {
    inline std::size_t hash_value(State const& p) {
        return rr::hash(p);
    }
}
#endif

#endif // RR_BOARD_HPP__
