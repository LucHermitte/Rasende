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
#ifndef RR_SRC_PORTABLE_HASH_MAP_HPP__
#define RR_SRC_PORTABLE_HASH_MAP_HPP__
#include "config.h"

#if defined(_MSC_VER) && _MSC_VER >= 1600
#  define HAVE_CXX11
#endif

#if   defined(HAVE_CXX11)
#  include <unordered_map>
#  define rr_hash_map std::unordered_map
#elif defined(HAVE_TR1)
#  include <tr1/unordered_map>
#  define rr_hash_map std::tr1::unordered_map
#elif defined(HAVE_STLPORT)
#  include <hash_map>
#  define rr_hash_map std::hash_map
#elif defined(HAVE_SGISTL)
#  include <ext/hash_map.h>
#  define rr_hash_map std::hash_map
#else
#  error No hash map found
#endif

#endif // RR_SRC_PORTABLE_HASH_MAP_HPP__
