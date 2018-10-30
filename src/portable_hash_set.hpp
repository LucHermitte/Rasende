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
#ifndef RR_SRC_PORTABLE_HASH_SET_HPP__
#define RR_SRC_PORTABLE_HASH_SET_HPP__
#include "config.h"

#if defined(_MSC_VER) && _MSC_VER >= 1600
#  define HAS_CXX11_UNORDERED_SET
#endif

#if defined(HAS_CXX11_UNORDERED_SET)
// Conformant compiler 
#  include <unordered_set>
#  define rr_hash_set std::unordered_set
#elif defined(HAS_CXX_TR1_UNORDERED_SET)
#  include <tr1/unordered_set>
#  define rr_hash_set std::tr1::unordered_set
#elif defined(HAS_STLPORT)
// STLPort & older versions of VC++
#  include <hash_set>
#  define rr_hash_set std::hash_set
#elif defined(HAS_STDEXT_HASH_SET)
// VC++ 2003 & +
#  include <hash_set>
#  define rr_hash_set stdext::hash_set
#elif defined(HAS_EXT_HASH_SET)
#  include <ext/hash_set>
#  define rr_hash_set __gnu_cxx::hash_set
#else
#  error No hash set found
#endif


#endif // RR_SRC_PORTABLE_HASH_SET_HPP__
