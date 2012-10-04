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
#ifndef RR_EXPLORE_HPP__
#define RR_EXPLORE_HPP__

#include <set>
#include <deque>
#include <algorithm>
#include "plateau.hpp"
#include "portable_hash_set.hpp"

namespace rr {
   struct xxx {
       xxx() : m_energy(0) {}
       explicit xxx(State state_, int energy_=0) : m_state(state_), m_energy(energy_) {}
       explicit xxx(State state_, State old_, int energy_=0) : m_state(state_), m_energy(energy_), m_previous(old_) {}
       Robot const& operator[](Robot::names r) const { return m_state[r]; }
       Robot      & operator[](Robot::names r)       { return m_state[r]; }

       State move_robot_to(Robot::names r, Robot const& to) const {
           assert(r < Robot::MAX__);
           State res = m_state; // todo eliminate one useless copy
           // xxx res(m_state, m_state, m_energy+1);
           res[r] = to;
           return res;
       }
       int nrj() const { return m_energy; }
       State m_state;
       int   m_energy;
       State m_previous;
   };
   bool operator==(State const& lhs, State const& rhs) {
       for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; r=Robot::names(r+1))
       {
           //todo memcmp ?
           if (lhs.m_robots[r] != rhs.m_robots[r])
               return false;
       }
       return true; // equal
   }
   bool operator<(State const& lhs, State const& rhs) {
       for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; r=Robot::names(r+1))
       {
           if (lhs.m_robots[r] != rhs.m_robots[r])
               return lhs.m_robots[r] < rhs.m_robots[r];
       }
       return false; // equal
   }
   struct Cmp_less_xxx_State{
       bool operator()(State const& lhs, xxx   const& rhs) const { return lhs < rhs.m_state; }
       bool operator()(xxx   const& lhs, State const& rhs) const { return lhs.m_state < rhs; }
       bool operator()(xxx   const& lhs, xxx   const& rhs) const { return lhs.m_state < rhs.m_state; }
   };
   struct Cmp_eq_xxx_State{
       bool operator()(State const& lhs, xxx   const& rhs) const { return lhs == rhs.m_state; }
       bool operator()(xxx   const& lhs, State const& rhs) const { return lhs.m_state == rhs; }
       bool operator()(xxx   const& lhs, xxx   const& rhs) const { return lhs.m_state == rhs.m_state; }
   };
   bool operator==(xxx const& lhs, xxx   const& rhs) { return lhs.m_state == rhs.m_state; }
   bool operator==(xxx const& lhs, State const& rhs) { return lhs.m_state == rhs; }
   std::ostream & operator<<(std::ostream & os, xxx const& p) {
       return os 
           << "R(" << p[Robot::Rouge] << "), " 
           << "V(" << p[Robot::Vert] << "), " 
           << "G(" << p[Robot::Gris] << "), " 
           << "B(" << p[Robot::Bleu] << "), " 
           ;
   }


    Robot next_left_from(Board const& b_, Robot const& r_, State const& s_) {
        const Line   l = r_.p.l;
        Column c = r_.p.c;
        for (; Column(0)<c ; --c)
        {
            const Column c2 = c - Column(1);
            if (b_.hasVerticalWall(l,c2) || s_.hasRobot(l,c2))
                break;
        }
        return Robot(l,c);
    }

    Robot next_right_from(Board const& b_, Robot const& r_, State const& s_) {
        const Line   l = r_.p.l;
        Column c = r_.p.c;
        for (; c<Board::nb_V_walls() ; ++c)
        {
            const Column c2 = c + Column(1);
            if (b_.hasVerticalWall(l,c) || s_.hasRobot(l,c2))
                break;
        }
        return Robot(l,c);
    }

    Robot next_up_from(Board const& b_, Robot const& r_, State const& s_) {
        const Column c = r_.p.c;
        Line   l = r_.p.l;
        for (; Line(0) < l; --l)
        {
            const Line l2 = l-Line(1);
            if (b_.hasHorizontalWall(l2,c) || s_.hasRobot(l2,c))
                break;
        }
        return Robot(l,c);
    }

    Robot next_down_from(Board const& b_, Robot const& r_, State const& s_) {
        const Column c = r_.p.c;
        Line   l = r_.p.l;
        for (; l<Board::nb_H_walls() ; ++l)
        {
            const Line l2 = l+Line(1);
            if (b_.hasHorizontalWall(l,c) || s_.hasRobot(l2,c))
                break;
        }
        return Robot(l,c);
    }


/*===========================================================================*/
/*=======================[ Breath-first exploration ]========================*/
/*===========================================================================*/

    struct the_hash {
        size_t operator()(xxx const& p) const { return (*this)(p.m_state); }
        size_t operator()(State const& p) const { return the_hash::hash(p); }
        static size_t hash(Robot const& r) {
            return r.l()*16+r.c();
        }
        static size_t hash(State const& p) {
            return ((hash(p[Robot::Rouge])*256+hash(p[Robot::Bleu]))*256+hash(p[Robot::Vert]))*256+hash(p[Robot::Gris]);
        }
    };
    struct Set {
        Set() : m_states(new tStates()) {}
        ~ Set() { delete m_states; }
        bool hasSeen(State const& p) const {
            const size_t o = the_hash::hash(p);
            return (*m_states)[o];
        }
        bool hasSeen(xxx const& p) const { return hasSeen(p.m_state); }
        bool checkAndSet(State const& p) {
            const size_t o = the_hash::hash(p);
            const bool seen = (*m_states)[o];
            (*m_states)[o] = true;
            return seen;
        }
        void set(xxx const& p) { hasSeen(p.m_state); }
        void set(State const& p) {
            const size_t o = the_hash::hash(p);
            (*m_states)[o] = true;
        }
    private:
        typedef std::bitset<256UL*256*256*256> tStates;
        tStates * m_states;
    };

    struct Stack 
    {
        void insert(xxx const& p) {
            m_seen.set(p);
            assert(m_queue.empty() || m_queue.back().nrj() <= p.nrj());
            m_queue.push_back(p);
        }
        bool empty() const { return m_queue.empty(); }
        xxx pop() {
            // exception-safe as long as xxx copy does not throw
            xxx position = m_queue.front();
            m_queue.pop_front();
            return position;
        }
        bool contains(State const& p) const {
            return m_seen.hasSeen(p);
        }
        size_t size() const { return m_queue.size(); }

   private:
       Set             m_seen; 
       std::deque<xxx> m_queue; 
   };

   struct Breadth {
       xxx m_solutions[Robot::MAX__];
       bool must_search_for_other_solutions(xxx const& p, Robot::names r_) {
           if (m_solutions[r_].nrj() == 0) // new solution
           {
               {
                   std::cout << "\nGOAL reached for " << Robot::toString(r_) << ": " <<std::flush;
                   xxx last=p;
                   while (true)
                   {
                       std::cout << "\n  <-" << last <<std::flush;
                       const bool seen = m_seen.hasSeen(last.m_previous);
                       if (!seen) break;
                       last = *wh;
                   }
                   std::cout << std::endl;
               }

               m_solutions[r_] = p;
               for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; r=Robot::names(r+1))
                   if (m_solutions[r].nrj()==0)
                       return true;
               return false;
           }
           return true;
       }

       Breadth(State start_, Goal g_, Board const& b) : m_goal(g_) {
           m_stack.insert(xxx(start_));
           // 4 solutions: une par robot

           while (! m_stack.empty())
           {
               // pop first
               const xxx position = m_stack.pop();
               m_seen.insert(position);
               // std::cout << std::string(position.nrj(), ' ') << "exploring from " << position << "\n";


               // for each robot
               for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; r=Robot::names(r+1)) {
                   // find the 4 possible positions from here
                   {
                       // std::cout << std::string(1+position.nrj(), ' ') << Robot::toString(r) << " <- " << std::flush;
                       Robot left  = next_left_from(b, position[r], position.m_state);
                       check_new_position_for_robot(position, left, r);
                   }
                   {
                       // std::cout << std::string(1+position.nrj(), ' ') << Robot::toString(r) << " -> " << std::flush;
                       Robot right = next_right_from(b, position[r], position.m_state);
                       check_new_position_for_robot(position, right, r);
                   }
                   {
                       // std::cout << std::string(1+position.nrj(), ' ') << Robot::toString(r) << " ^ " << std::flush;
                       Robot up    = next_up_from(b, position[r], position.m_state);
                       check_new_position_for_robot(position, up, r);
                   }
                   {
                       // std::cout << std::string(1+position.nrj(), ' ') << Robot::toString(r) << " v " << std::flush;
                       Robot down  = next_down_from(b, position[r], position.m_state);
                       check_new_position_for_robot(position, down, r);
                   }
               }
           }
       }

       void check_new_position_for_robot(xxx const& crt_position, Robot const& new_position, Robot::names r)
       {
           // std::cout << new_position << std::flush;
           if (crt_position.m_state[r] == new_position) {
               // The specific case (crt == new) is an optimzation as a
               // robot in an angle won't be able to move in the angle
               // direction
               // std::cout << " same place => discard" << std::endl;
               return ;
           }
           
           const State pl = crt_position.move_robot_to(r, new_position);
           const Set::const_iterator it = m_seen.find(pl);
           if (it == m_seen.end())
           {
               if (m_stack.contains(pl)) {
                   // ignore this new path as the xxx are stored by
                   // growing energy
                   // std::cout << " in stack => discard" << std::endl;
               } else {
                   xxx path(pl, crt_position.m_state, crt_position.nrj()+1); // link to previous state, and to nrj, here only
                   m_stack.insert(path);
                   // std::cout << " not in stack " << std::flush;
                   if (m_goal == new_position)
                   {
                       // std::cout << "; GOAL " << std::flush;
                       // update solution for this robot
                       if (! must_search_for_other_solutions(path, r))
                       {
                           std::cout << "; all solutions found!" << std::endl;
                           return ;
                       }
                   }
                   // std::cout << std::endl;
               }
           }
           else
           {
               // std::cout << " seen => discard" << std::endl;
               // by construction, seen elements are searched by
               // growing energy
               assert(it->m_energy <= crt_position.nrj());
               // ignore this new path
           }
           std::cout << "\r" << (1+crt_position.nrj()) << " coups ; " << m_seen.size() << " explored ; " << m_stack.size() << " pending" << std::flush;
           // m_stack.show_stats(std::cout << " ; ");

       }

       Set m_seen;
       Stack m_stack;
       const Goal m_goal;
   };
} // typespace rr


#endif // ESSAIS_RR_EXPLORE_HPP__
