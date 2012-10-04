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
#ifndef RR_TIMER_HPP__
#define RR_TIMER_HPP__

#include <string>

#include "bench/benchmark_timer.h"
#include "bench/benchmark_results.h"

namespace rr {
    /**
     * Timer class.
     * Relies on Stepanov's benchmarking library for STL implementations.  
     */
   struct Timer {
       /**
        * Init Constructor.
        *
        * @param[in] message_ Test description. 
        *
        * @internal 
        * As \c record_result()'s parameter must continue to exist long after
        * the call, it cannot be a member of this class. Indeed, once
        * destructed, \c m_message.c_str() would be invalidated. As a
        * consequence, \c message_ shall point to a \c char array that
        * continues to exist ; typically a plain quoted-string.
        */
       Timer(char const* message_)
           : m_message(message_)
           {
               start_timer();
           }
       ~Timer() {
           record_result(timer(), m_message);
       }
   private:
       const char* m_message;
   };

} // namespace rr


#endif // RR_TIMER_HPP__
