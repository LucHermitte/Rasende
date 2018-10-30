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
#ifndef RR_SRC_EXPLORATION_PRECOMPUTED_PATHS_HPP__
#define RR_SRC_EXPLORATION_PRECOMPUTED_PATHS_HPP__

#include "Game.hpp"
#include "Board.hpp"
#include "Robot.hpp"

namespace rr {

    Robot next_from(Node const& node_, Robot::names r_, Move const& m_)
    {
        Robot const& r_crt = node_[r_];
        const Line   l = r_crt.l();
        const Column c = r_crt.c();
        const Position p2 = m_.position;
        Line   l2 = p2.l;
        Column c2 = p2.c;
#if 0
        // todo: check each 4 robots once instead of each possible position
        if (c == p2.c)
        {
            assert(m_.direction == Directions::UP || m_.direction==Directions::DOWN);
            Line d = p2.l < l ? -1 : 1;
            for (Line l2=l+d;l!=p2.l; l2=l+d) {
                if (s_.hasRobot(l2, c)) 
                    break;
                l=l2;
            }
        } else {
            assert(m_.direction == Directions::LEFT || m_.direction==Directions::RIGHT);
            Column d = p2.c < c ? -1 : 1;
            for (Column c2=c+d;c!=p2.c; c2+=d) {
                if (s_.hasRobot(l, c2)) 
                    break;
                c = c2;
            }
        }
        return Robot(l,c);
#else
        switch (m_.direction) {
            case Directions::UP:
            case Directions::DOWN:
                for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; ++r) {
                    if (r == r_) { continue; }
                    Robot const& rb = node_[r];
                    if (rb.c() == c) {
                        // std::cout << Robot::toString(r) << "at same col\n";
                        const Line rb_l = rb.l();
                        if (l2 <= rb_l && rb_l < l) {        // UP
                            l2 = rb_l; ++l2;
                        } else if (l < rb_l && rb_l <= l2) { // DOWN
                            l2 = rb_l; --l2;
                        }
                    }
                }
                break;
            case Directions::LEFT:
            case Directions::RIGHT:
                for (Robot::names r=Robot::names(0) ; r!=Robot::MAX__; ++r) {
                    if (r == r_) { continue; }
                    Robot const& rb = node_[r];
                    if (rb.l() == l) {
                        // std::cout << Robot::toString(r) << "at same line\n";
                        const Column rb_c = rb.c();
                        if (c2 <= rb_c && rb_c < c) {        // LEFT
                            c2 = rb_c; ++c2;
                        } else if (c < rb_c && rb_c <= c2) { // RIGHT
                            c2 = rb_c; --c2;
                        }
                    }
                }
                break;
            default:
                assert(!"Invalid aggregation of directions");
        }
        return Robot(l2,c2);
#endif
    }


   namespace Policies
   {
/*===========================================================================*/
/*==============================[ Exploration ]==============================*/
/*===========================================================================*/
       namespace Exploration
       {
           template <class Super>
           struct PathsPrecomputed : Super
           {
               PathsPrecomputed(Board const& board_) : m_possibleMoves(board_) {}

               template <class This_t_>
               bool explore(Robot::names r_, Node const& node_, This_t_ *const this_)
               {
                   PossibleMoves::Moves const& m = m_possibleMoves.moves(node_[r_].pos());
                   for (rr::PossibleMoves::Moves::const_iterator b = m.begin(), e = m.end()
                           ; b != e
                           ; ++b
                       )
                   {
                       // const Robot r_moved = next_from(node_[r], node_.m_state, *b );
                       const Robot r_moved = next_from(node_, r_, *b );
                       if (this->check_new_position_for_robot(node_, r_moved, r_, b->direction, this_) ) {
                           return true;
                       }
                   }
                   return false;

               }
           private:
               PossibleMoves m_possibleMoves;
           };

           template <class Super>
           struct PathsPrecomputedAndRecordOrigins : Super
           {
               PathsPrecomputedAndRecordOrigins(Board const& board_) : m_possibleMoves(board_) {}

               template <class This_t_>
               bool explore(Robot::names r_, Node const& node_, This_t_ *const this_)
               {
                   PossibleMoves::Moves const& m = m_possibleMoves.moves(node_[r_].pos());
                   // filter moves to accept only opposites

                   for (rr::PossibleMoves::Moves::const_iterator b = m.begin(), e = m.end()
                           ; b != e
                           ; ++b
                       )
                   {
                       // const Robot r_moved = next_from(node_[r], node_.m_state, *b );
                       const Robot r_moved = next_from(node_, r_, *b );
                       if (this->check_new_position_for_robot(node_, r_moved, r_, b->direction, this_) ) {
                           return true;
                       }
                   }
                   return false;

               }
           private:
               PossibleMoves m_possibleMoves;
           };
       } // ns Exploration

   } // ns Policies
} // namespace rr

#endif // RR_SRC_EXPLORATION_PRECOMPUTED_PATHS_HPP__
