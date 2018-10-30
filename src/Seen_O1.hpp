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
#ifndef RR_SEEN_O1_HPP__
#define RR_SEEN_O1_HPP__

#include <bitset>
#include <deque>
#include <algorithm>
#include "portable_hash_set.hpp"
#include "Game.hpp" // State
#include "Node.hpp"
#include <limits>

namespace rr { namespace Policies { namespace Holding {
    //======================================================================
    struct Seen {
        Seen() : m_states(new states_t), m_nb_seen(0)
        { }
        ~Seen() { delete m_states; }

        bool contains(State const& p) const {
            // std::cout << "check @ " << p << std::endl;
            const size_t h = hash(p);
            // const bool   c = m_states->test(h);
            const bool   c = (*m_states)[h];
            return c;
        }
        void insert(State const& p) {
            //std::cout << "set @ " << p << std::flush;
            const size_t h = hash(p);
            //std::cout << "check unset(" << h << ")"<< std::flush;
            assert(!m_states->test(h) && "already set...");
            //std::cout << " ; set " << std::flush;
            // m_states->set(h);
            (*m_states)[h] = true;
            //std::cout << " ... done" << std::endl;
            ++m_nb_seen;
        }
        size_t size () const { return m_nb_seen; }
        bool   empty() const { return 0 == size(); }
    private:

        // It should be 256^4, but as the 4 robots cannot be on the same
        // position, 256^4-1 is enough
        // const uint64_t M = std::numeric_limits<uint64_t>::max();
        typedef std::bitset<4294967295ULL> states_t;
        states_t *m_states;
        size_t    m_nb_seen;
    };

    //======================================================================
    struct PendingQueue {
        void insert(Node const& p) {
            m_queue.push_back(p);
        }
        Node pop() {
            Node position = m_queue.front();
            m_queue.pop_front();
            // exception-safe as long as Node copy does not throw
            return position;
        }
        size_t size() const { return m_queue.size(); }

    private:
        std::deque<Node> m_queue;
    };

    //======================================================================
    struct PreviousStateMatrix {
#if defined(HAS_STDEXT_HASH_SET)
        typedef rr_hash_set<Node> Set;
#else
        typedef rr_hash_set<Node,the_hash, Cmp_eq_Node_State> Set;
#endif

        void insert(Node const& p) {
            m_matrix.insert(p);
        }

#if 0
        Node const* find(Node const& p) const {
            const Set::const_iterator wh = std::find(
                    m_matrix.begin(),m_matrix.end(),
                    p);
            return wh != m_matrix.end() ? &*wh : 0;
        }
#endif
        typedef Set::const_iterator const_iterator;
        const_iterator find(Node const& p) const {
            const Set::const_iterator wh = m_matrix.find(p);
            return wh;
        }
        const_iterator end() const {
            return m_matrix.end();
        }
    private:
        Set m_matrix;
    };

    //======================================================================
    template <class Super>
    struct Seen_O1 : Super
    {
        typedef Seen         Seen_t;
        typedef PendingQueue Stack_t;

        bool has_been_seen(State const& p) const {
            return m_seen.contains(p);
        }
        void insert_seen(Node const& p) {
            (void) p;
            // m_seen.insert(p);
        }
        void insert_pending(Node const& p) {
            m_seen.insert(p.m_state);
            m_pending.insert(p);
            m_previous.insert(p);
        }
        Node * find_pending(State const& pl) {
            (void)pl;
            return 0;
        }
        Node const* previous(Node const& p) {
            return m_previous.find(p);
        }
        PreviousStateMatrix const& previous   () const { return m_previous; }
        size_t                     nb_seen    () const { return m_seen.size(); }
        size_t                     nb_pending () const { return m_pending.size(); }
        Node                       pop        ()       { return m_pending.pop(); }
    private:
        Seen                m_seen;
        PendingQueue        m_pending;
        PreviousStateMatrix m_previous;
    };
} } } // namespace rr::Holding::Policies


#endif // RR_SEEN_O1_HPP__
