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
#ifndef RR_SRC_NODE_HPP__
#define RR_SRC_NODE_HPP__

#include <cassert>
#include "config.h"
#include "Robot.hpp"
#include "Game.hpp" // rr::State
#include "Moves.hpp"

namespace rr { 
    /** Exploration node.
     * It's:
     * - a current state,
     * - its depth,
     * - and a previous state. 
     */
    struct Node
    {
	Node() 
            : m_energy(0)
            {}
	/*explicit*/ Node(State state_, int energy_=0)
            : m_state(state_)
            , m_energy(energy_)
            {}
	explicit Node(
                State state_,
                State old_,
                RobotMove link_,
                int energy_=0)
	    : m_state(state_), m_energy(energy_)
            , m_previous(old_) 
            , m_link(link_)
            {}
	Robot const& operator[](Robot::names r) const { return m_state[r]; }
	Robot      & operator[](Robot::names r)       { return m_state[r]; }
        RobotMove    link()                     const { return m_link; }

	State move_robot_to(Robot::names r, Robot const& to) const {
	    assert(r < Robot::MAX__);
	    State res = m_state; // todo eliminate one useless copy
	    // Node res(m_state, m_state, m_energy+1);
	    res[r] = to;
	    return res;
	}
	int nrj() const { return m_energy; }
	State m_state;
	int   m_energy;
        State m_previous;
        RobotMove m_link;
    };
    bool operator==(State const& lhs, State const& rhs) {
	for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; ++r)
	{
	    //todo memcmp ?
	    if (lhs.m_robots[r] != rhs.m_robots[r])
		return false;
	}
	return true; // equal
    }
    bool operator<(State const& lhs, State const& rhs) {
	for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; ++r)
	{
	    if (lhs.m_robots[r] != rhs.m_robots[r])
		return lhs.m_robots[r] < rhs.m_robots[r];
	}
	return false; // equal
    }
    struct Cmp_less_Node_State{
	bool operator()(State const& lhs, Node   const& rhs) const { return lhs < rhs.m_state; }
	bool operator()(Node   const& lhs, State const& rhs) const { return lhs.m_state < rhs; }
	bool operator()(Node   const& lhs, Node   const& rhs) const { return lhs.m_state < rhs.m_state; }
    };
    struct Cmp_eq_Node_State{
	bool operator()(State const& lhs, Node   const& rhs) const { return lhs == rhs.m_state; }
	bool operator()(Node   const& lhs, State const& rhs) const { return lhs.m_state == rhs; }
	bool operator()(Node   const& lhs, Node   const& rhs) const { return lhs.m_state == rhs.m_state; }
    };
    bool operator==(Node const& lhs, Node   const& rhs) { return lhs.m_state == rhs.m_state; }
    bool operator==(Node const& lhs, State const& rhs) { return lhs.m_state == rhs; }
    std::ostream & operator<<(std::ostream & os, Node const& p) {
	return os 
	    << "R(" << p[Robot::Rouge] << "), " 
	    << "V(" << p[Robot::Vert] << "), " 
	    << "G(" << p[Robot::Gris] << "), " 
	    << "B(" << p[Robot::Bleu] << "), " 
	    ;
    }

} // rr namespace 

#if defined(HAS_STDEXT_HASH_SET)
#include <functional>
namespace std {
    template <> struct less<rr::Node>
        : std::binary_function <rr::Node,rr::Node,bool>
        , rr::Cmp_less_Node_State
    {
    };
} // std namespace
namespace stdext {
    inline std::size_t hash_value(rr::Node const& p) {
        return rr::hash(p.m_state);
    }
}
#endif


#endif // RR_SRC_NODE_HPP__
