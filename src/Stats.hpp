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
#ifndef RR_SRC_STATS_HPP__
#define RR_SRC_STATS_HPP__

#include <iostream>

namespace rr {
    // template <class Seen_t_, class Queue_t_> struct Stats
    template <class Holding_t> struct Stats
    {
        // Stats(Seen_t_ const& seen_, Queue_t_ const& queue_) 
        Stats(Holding_t const& holding_) 
            // : m_seen(seen_), m_queue(queue_)
            : m_holding(holding_)
            , m_discarded_at_this_move(0)
            , m_discarded(0)
            , m_current_nrj(0)
            , m_nb_at_this_level(1)
            {}
        void inc_discarded()           { ++m_discarded_at_this_move; ++m_discarded; }
        size_t explored()        const { return m_holding.nb_seen(); }
        size_t discarded()       const { return m_discarded_at_this_move; }
        size_t total_discarded() const { return m_discarded; }
        size_t pending()         const { return m_holding.nb_pending(); }
        size_t left()            const { return m_nb_at_this_level - explored() ;}

        void printSituationSoFar(bool force_) const {
            const int k_reset_count_situation_print = 50;
            static int count_situation_print = k_reset_count_situation_print;

            if (force_ || ! --count_situation_print) {
                std::cout << "\r" << (1+m_current_nrj) << " moves ; " 
                    << explored() << " explored ; "
                    << left() << " | " << pending() << " pending ; "
                    << discarded() <<"/" << total_discarded() << " discarded"
                    << std::flush;
                count_situation_print = k_reset_count_situation_print;
                // m_queue.show_stats(std::cout << " ; ");
            }
        }
        void setCurrent_nrj(int nrj_) {
            if (m_current_nrj != nrj_) {
                // When the function is called, the node has already been
                // fetched from the queue and added to the list of seen nodes.
                std::cout << "\r" << (explored()-1) << " situations up to " << (m_current_nrj+1) << "-moves have been explored. "
                    << "Now proceeding with the " << (1+pending()) << " " << (nrj_+1) << "-moves pending."
                    << std::endl;
                m_current_nrj = nrj_;
                m_discarded_at_this_move = 0;
                m_nb_at_this_level = pending() + explored();
            }
        }
    private:
        Holding_t const& m_holding;
        // Seen_t_  const& m_seen;
        // Queue_t_ const& m_queue;
        size_t           m_discarded_at_this_move;
        size_t           m_discarded;
        int              m_current_nrj;
        size_t           m_nb_at_this_level;
    };
} // namespace rr


#endif // RR_SRC_STATS_HPP__
