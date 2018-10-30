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
/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include <iostream>
#include <stdexcept>
#include <fstream>

#define private public

#include "Game.hpp"
#include "Moves.hpp"
#include "explore-hash.hpp"
#include "Seen_O1.hpp"


/*===========================================================================*/
/*==================================[ «» ]===================================*/
/*===========================================================================*/

int main (int argc, char **argv)
{
    try {
        rr::Game g;
        if (argc > 1) {
            std::ifstream f(argv[1]);
            if (!f)
            {
                throw std::runtime_error(std::string()+"Cannot read `"+argv[1]+"'");
            }
            f >> g;

#if 0
            // todo: add debug option to print the possible moves
            rr::PossibleMoves pm(g.m_board);
            std::cout << pm;

            const rr::Robot Red = g.m_start_state[rr::Robot::Rouge];
            rr::PossibleMoves::Moves const& m = pm.moves(Red.pos());
            std::cout << "hasR(0,0)" << g.m_start_state.hasRobot(0,0) << "\n";
            std::cout << "Red at " << Red << std::endl;
            for (rr::PossibleMoves::Moves::const_iterator b = m.begin(), e = m.end()
                    ; b != e
                    ; ++b
                )
            {
                const rr::Robot r_moved = next_from(g.m_start_state, rr::Robot::Rouge, *b );
                std::cout << rr::Directions::toString(b->direction) << " " 
                    << r_moved << " wall at " << b->position << std::endl;
            }
#else
	    using namespace rr;
            using namespace rr::Policies;
#if defined(HAS_BITSET_0xffffffff)
            Breadth<Exploration::PathsPrecomputed<Holding::Seen_O1<Origin::None<Interface> > > > search1(g.m_start_state, g.m_goal, g.m_board, "seen in O(1), hash, paths precomputed");
#endif

            Breadth<Exploration::PathsPrecomputed<Holding::Hash<Origin::None<Interface> > > > search2(g.m_start_state, g.m_goal, g.m_board, "hash, paths precomputed");
            // const size_t explored = search1.explored();
            // const size_t discarded = search1.total_discarded();

            Breadth<Exploration::StepByStep<Holding::Hash<Origin::None<Interface> > > > search3(g.m_start_state, g.m_goal, g.m_board, "hash, step by step");
            const size_t explored = search3.explored();
            const size_t discarded = search3.total_discarded();
            summarize(("rasende for "+std::string(argv[1])).c_str(),
                    explored+discarded, 1, 
                    kShowGMeans, kShowPenalty );
#endif
        } else
        {
#if 0
            for (rr::Line l=0 ; l!=rr::Board::nb_lines(); ++l)
            {
                for (rr::Column c=0; c!=rr::Board::nb_V_walls(); ++c)
                {
                    std::cout << rr::Board::offset_for_V_walls(l,c) << "\n";
                    g.putVerticalWall(l,c);
                    assert(g.m_board.hasVerticalWall(l,c));
                }
            }
#else
            g.putVerticalWall(7,6); g.putVerticalWall(8,6);
            g.putVerticalWall(7,8); g.putVerticalWall(8,8);
            g.putHorizontalWall(6,7); g.putHorizontalWall(6,8);
            g.putHorizontalWall(8,7); g.putHorizontalWall(8,8);
#endif
            std::cout << g;
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

