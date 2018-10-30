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
#ifndef RR_STACK_HASH_HPP__
#define RR_STACK_HASH_HPP__

#include <cassert>
#include <ostream>
#include <algorithm>
#if 0
#   include <set>
#   include <deque>
#else
#   include "portable_hash_set.hpp"
#endif
#include "Node.hpp"
#include "Robot.hpp"


namespace rr { 
   
#if 0
    struct Stack {
	void insert(Node const& p) {
	    assert(m_queue.empty() || m_queue.back().nrj() <= p.nrj());
	    m_queue.push_back(p);
	}
	bool empty() const { return m_queue.empty(); }
	Node pop() {
	    // exception-safe as long as Node copy does not throw
	    Node position = m_queue.front();
	    m_queue.pop_front();
	    return position;
	}
	bool contains(Node const& p) const {
	    std::deque<Node>::const_iterator wh = std::find(
		    m_queue.begin(),m_queue.end(),
		    p);
	    return wh != m_queue.end();
	}
	size_t size() const { return m_queue.size();}
    private:
	std::deque<Node> m_queue; // sorted by energy
    };
#else
    struct the_hash {
		size_t operator()(Node const& p) const { return (*this)(p.m_state); }
		size_t operator()(State const& p) const { return hash(p); }
    };
#if defined(HAS_STDEXT_HASH_SET)
    typedef rr_hash_set<Node> Set;
#else
    typedef rr_hash_set<Node,the_hash, Cmp_eq_Node_State> Set;
#endif
    // typedef std::set<Node,Cmp_less_Node_State> Set;
    struct Stack 
    {
        typedef Set::value_type* pointer;

	void insert(Node const& p) {
	    if (m_low.empty() || m_low.begin()->nrj() == p.nrj())
		m_low.insert(p);
	    else {
		assert(m_high.empty() || m_high.begin()->nrj() == p.nrj());
		m_high.insert(p);
	    }
	}
        // bool empty() const { return 0 == size(); }
        size_t nb_pending() const { return size(); }
	Node pop() {
	    assert(nb_pending() && "no pending position");
	    assert(!m_low.empty() && "empty low nrj stack");
	    // exception-safe as long as Node copy does not throw
	    Node position = *m_low.begin();
	    m_low.erase(m_low.begin());

	    if (m_low.empty())
	    {
			// std::cout << "swap low <-> high" << std::endl;
			m_low.swap(m_high);
	    }
	    return position;
	}
	bool contains(State const& p) const {
	    return find_in(m_low, p) || find_in(m_high, p);
	}
	pointer find(State const& p) {
            const pointer low = find_in(m_low, p);
	    return low ? low : find_in(m_high, p);
	}
	size_t size() const { return m_low.size() + m_high.size();}

#if defined(HAVE_STLPORT)
	static std::ostream & hash_stats(std::ostream &os, Set const& set_)
	{
	    const size_t N=set_.bucket_count();
	    const double expected = 1.*set_.size()/set_.bucket_count();
	    size_t Max = 0, sum=0;
	    double chi = 0.;
	    for (size_t i=0 ;i!=N;++i)
	    {
		const size_t e = set_.elems_in_bucket(i);
		// Mean
		sum += e;
		// Max
		if (Max < e) Max = e;
		// Chi-square
		const double diff = expected - e;
		chi += diff * diff;
	    }
	    chi /= expected;
	    return os << "max="<<Max<<", mean="<<(1.0*sum/N)<<", exp="<<expected<< ", chi²="<<chi;
	}
#endif
	void show_stats(std::ostream & os) {
	    // os << "max_size="<<m_low.max_size()<<"/"<<m_high.max_size();
#if ! defined(HAS_STDEXT_HASH_SET)
            os << "buckets="<<m_low.bucket_count()<<"/"<<m_high.bucket_count();
#endif
	    // os << " ; max_nb_buckets="<<m_low.max_bucket_count()<<"/"<<m_high.max_bucket_count();
	    // os << " ; elems_in_bucket="<<m_low.elems_in_bucket()<<"/"<<m_high.elems_in_bucket();
#if defined(HAVE_STLPORT)
	    hash_stats(os<<" ; LOW=>", m_low);
	    hash_stats(os<<" ; HIGH=>", m_high);
#endif
	    os <<"      "<< std::flush;
	}
    private:
	static bool find_in(Set const& set_, State const& p)
	{
        const Set::const_iterator wh = set_.find(p);
	    return wh != set_.end();
	}
	static pointer find_in(Set & set_, State const& p)
	{
		const Set::const_iterator wh = set_.find(p);
		return (wh != set_.end()) ? const_cast <pointer>(&(*wh)) : 0; // todo: use a map instead
	}
		Set m_low; 
		Set m_high; 
    };
#endif

} // namespace rr


#endif // RR_STACK_HASH_HPP__
