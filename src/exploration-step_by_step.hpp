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
#ifndef  RR_SRC_EXPLORATION_STEP_BY_STEP_HPP__
#define RR_SRC_EXPLORATION_STEP_BY_STEP_HPP__

#include "Game.hpp"
#include "Board.hpp"
#include "Robot.hpp"

namespace rr {

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

    namespace Policies
    {
        /*===========================================================================*/
        /*==============================[ Exploration ]==============================*/
        /*===========================================================================*/
        namespace Exploration
        {
            template <class Super> struct StepByStep : Super
            {
                StepByStep(Board const& board_) : m_board(board_) {}

                template <class This_t_> bool explore(Robot::names r_, Node const& node_, This_t_ *const this_)
                {
                    {
                        // std::cout << std::string(1+node_.nrj(), ' ') << Robot::toString(r) << " <- " << std::flush;
                        Robot left  = next_left_from(m_board, node_[r_], node_.m_state);
                        if (this->check_new_position_for_robot(node_, left, r_, Directions::LEFT, this_)) 
                            return true;
                    }
                    {
                        // std::cout << std::string(1+node_.nrj(), ' ') << Robot::toString(r_) << " -> " << std::flush;
                        Robot right = next_right_from(m_board, node_[r_], node_.m_state);
                        if (this->check_new_position_for_robot(node_, right, r_, Directions::RIGHT, this_)) 
                            return true;
                    }
                    {
                        // std::cout << std::string(1+node_.nrj(), ' ') << Robot::toString(r_) << " ^ " << std::flush;
                        Robot up    = next_up_from(m_board, node_[r_], node_.m_state);
                        if (this->check_new_position_for_robot(node_, up, r_, Directions::UP, this_)) 
                            return true;
                    }
                    {
                        // std::cout << std::string(1+node_.nrj(), ' ') << Robot::toString(r_) << " v " << std::flush;
                        Robot down  = next_down_from(m_board, node_[r_], node_.m_state);
                        if (this->check_new_position_for_robot(node_, down, r_, Directions::DOWN, this_)) 
                            return true;
                    }
                    return false;
                }
                Board m_board;
            };

        } // namespace Exploration
    } // ns Policies
} // namespace rr

#endif // RR_SRC_EXPLORATION_STEP_BY_STEP_HPP__
