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

#if defined(_MSC_VER)
#    pragma warning (disable: 4355) // 'this' : used in base member initializer list
#endif

#include "Game.hpp"
#include "Moves.hpp"
#include "Node.hpp"
#include "stack-hash.hpp"
#include "Stats.hpp"
#include "Timer.hpp"
#include "exploration-step_by_step.hpp"
#include "exploration-precomputed_paths.hpp"
#include "Seen_O1.hpp"

namespace rr {

/*===========================================================================*/
/*=======================[ Breath-first exploration ]========================*/
/*===========================================================================*/

    template <class Seen_t_> inline
    Robot::names printSolution(Node const& node_, Seen_t_ const& seen_, Robot::names r_) {
        (void)r_;
        const typename Seen_t_::const_iterator wh = seen_.find(node_.m_previous);
        if (wh == seen_.end()) {
            return Robot::MAX__;
        }
        Robot::names r = printSolution(*wh, seen_, node_.link().robot());
        if (r != node_.link().robot()) {
            std::cout << Robot::toString(node_.link().robot()) << " ";
        }
        std::cout <<  Directions::toString(node_.link().direction()) << " " << std::flush;
        return  node_.link().robot();
    }

   namespace Policies
   {

/*===========================================================================*/
/*===============================[ Interface ]===============================*/
/*===========================================================================*/
       struct Interface
       {
#if 0
           template <class This_t_>
               bool must_search_for_other_solutions(Node const& p_, Robot::names r_, This_t_ * const this_) {
                   return static_cast <This_t_*>(this)->must_search_for_other_solutions(p_, r_);
               }
#endif

           template <class This_t_>
           bool check_new_position_for_robot(
                   Node const& crt_position_, Robot const& new_position_, 
                   Robot::names r_, Directions::types dir_,
                   This_t_ *const this_)
           {
               (void)this_;
               return static_cast <This_t_*>(this)->check_new_position_for_robot(crt_position_, new_position_, r_, dir_);
           }
       };

/*===========================================================================*/
/*================================[ Holding ]================================*/
/*===========================================================================*/
       namespace Holding
       {

           template <class Super>
           struct Hash : Super
           {
               typedef Set   Seen_t;
               typedef Stack Stack_t;

               Node          pop          ()       { return m_stack.pop(); }
               size_t        nb_seen      () const { return m_seen.size(); }
               size_t        nb_pending   () const { return m_stack.nb_pending(); }
               Seen_t const& previous     () const { return m_seen; }
               bool          has_been_seen(State const& pl) const {
                   const Seen_t::const_iterator it = m_seen.find(pl);
                   const bool known = it != m_seen.end();
                   return known;
               }
               void insert_seen(Node const& p) {
                   m_seen.insert(p);
               }
               void insert_pending(Node const& p) {
                   m_stack.insert(p);
               }
               Stack_t::pointer find_pending(State const& pl) {
                   return m_stack.find(pl);
               }
           protected:
               Seen_t     m_seen;
               Stack_t    m_stack;
           };
       } // ns Holding

/*===========================================================================*/
/*================================[ Origin ]=================================*/
/*===========================================================================*/

       namespace Origin 
       { 
           template <class Super>
           struct None : Super
           {
               typedef RobotMove Origin_t;
               void updateOrigins(Node & node_, Robot::names r_, Directions::types dir_)
               { (void)node_; (void)r_; (void)dir_; }

               PossibleMoves const& moves(PossibleMoves const& m_)
               { return m_; }
           };

           template <class Super>
           struct Record : Super
           {
               typedef Move Origin_t[4];

               void updateOrigins(Node & node_, Robot::names r_, Directions::types dir_)
               {
                   // node_.addOrigin(r_, dir_);
               }

               PossibleMoves const& moves(PossibleMoves const& m_, Origin_t const& origins_)
               {
                   // return PossibleMoves(range(m_, opposite(origins_)));
                   return m_;
               }
           };
       } // Origin namespace 
   } // ns Policies


/*===========================================================================*/
/*================================[ Breadth ]================================*/
/*===========================================================================*/

   template <class Super>
   struct Breadth : Super
   {
       typedef typename Super::Seen_t  Seen_t;
       typedef typename Super::Stack_t Stack_t;

       // Stats<Seen_t,Stack_t> m_stats;
       Stats<Super> m_stats;

       Breadth(State start_, Goal g_, Board const& board_, char const* message_)
           : Super(board_)
           // , m_stats(Super::m_seen, Super::m_stack)
           , m_stats(static_cast<Super&>(*this))
           , m_goal(g_)
       {
           Timer t(message_);

           this->insert_pending(Node(start_));
           // 4 solutions: une par robot

           while (this->nb_pending())
           {
               // pop first
               const Node position = this->pop();
               this->insert_seen(position);
               m_stats.setCurrent_nrj(position.nrj());
               // std::cout << std::string(position.nrj(), ' ') << "exploring from " << position << "\n";

               // for each robot
               for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; ++r) {
                   // find the 4 possible positions from here
                   if (this->explore(r, position, this)) return ;
               }
           }
       }

       bool must_search_for_other_solutions(Node const& p, Robot::names r_)
       {
           if (m_solutions[r_].nrj() == 0) // new solution
           {
               {
                   this->m_stats.printSituationSoFar(true);
                   std::cout << "\nGOAL reached for " << Robot::toString(r_) << " in " << p.nrj() << " moves" << ": " <<std::flush;
                   // print solution
#if 0
                   Node last=p;
                   while (true)
                   {
                       std::cout << "\n <- " << last << std::flush;
                       const Set::iterator wh = this->m_seen.find(last.m_previous);
                       if (wh == this->m_seen.end()) break;
                       std::cout << "[" 
                           << Robot::toString(last.link().robot()) << Directions::toString(last.link().direction())
                           << "]" << std::flush;
                       last = *wh;
                   }
                   std::cout << std::endl;
#endif

                   printSolution(p, this->previous(), Robot::MAX__);
                   std::cout << "\n" << std::endl;
               }

               m_solutions[r_] = p;
               for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; ++r)
                   if (m_solutions[r].nrj()==0)
                       return true;
               return false;
           }
           return true;
       }

       bool check_new_position_for_robot(
               Node const& crt_position_, Robot const& new_position_, 
               Robot::names r_, Directions::types dir_)
       {
           // std::cout << new_position_ << std::flush;
           if (crt_position_.m_state[r_] == new_position_) {
               // The specific case (crt == new) is an optimzation as a
               // robot against a wall won't be able to move in the
               // wall direction
               // std::cout << " same place => discard" << std::endl;
               this->m_stats.inc_discarded();
               return false;
           }

           const State pl = crt_position_.move_robot_to(r_, new_position_);
           // const Set::const_iterator it = this->m_seen.find(pl);
           // const bool known = it == this->m_seen.end();
           const bool known = this->has_been_seen(pl);
           if (!known)
           {
               Node* node = this->find_pending(pl);
               if (node) {
                   // ignore this new path as the Node are stored by
                   // growing energy
                   // std::cout << " in stack => discard" << std::endl;
                   this->m_stats.inc_discarded();
                   this->updateOrigins(*node, r_, dir_); // no-op when origin-policies==None
               } else {
                   Node path(pl, crt_position_.m_state, RobotMove(r_, dir_), crt_position_.nrj()+1); // link to previous state, and to nrj, here only
                   this->insert_pending(path);
                   // std::cout << " not in stack " << std::flush;
                   if (m_goal == new_position_)
                   {
                       // std::cout << "; GOAL " << std::flush;
                       // update solution for this robot
                       if (! must_search_for_other_solutions(path, r_))
                       {
                           std::cout << "; all solutions found!" << std::endl;
                           return true;
                       }
                   }
                   // std::cout << std::endl;
               }
           }
           else
           {
               // std::cout << " seen => discard" << std::endl;
               this->m_stats.inc_discarded();
               // by construction, seen elements are searched by
               // growing energy
               // assert(it->m_energy <= crt_position_.nrj());
               // ignore this new path
           }

           this->m_stats.printSituationSoFar(false);
           return false;
       }
       size_t explored() const {
           return m_stats.explored();
       }
       size_t total_discarded() const {
           return m_stats.total_discarded();
       }
   private:
       Node m_solutions[Robot::MAX__];
       const Goal m_goal;
   };
} // namespace rr


#endif // RR_EXPLORE_HPP__
