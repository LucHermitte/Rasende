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
#ifndef RR_NUMBER_HPP__
#define RR_NUMBER_HPP__

namespace rr {
    /**\addtogroup gCore
     * @{
     */
    /**
     * Strongly typed numbers.
     * This hacks is an experiment at strongly typed numbers. This way
     * \c Line and \c Column cannot be mixed, and there is no need
     * to play with <em>Hungarian System notation</em>.
     *
     * @tparam unit kind of number (distance/time/speed, line/column,
     * ...)
     * @tparam type internal representation type (\c int, \c double,
     * etc).
     */
    template <typename unit, typename type> struct Number {
	Number(type n) : v(n) {}
	type v; //!< Number value.

	Number& operator--() { --v; return *this; }
	Number& operator++() { ++v; return *this; }

        Number& operator+=(Number const& rhs_) { v+= rhs_.v; return *this; }
    };
    template <typename unit, typename type, typename t2>
        inline
	t2 const operator+(t2 lhs, Number<unit,type> rhs) {
	    return lhs+rhs.v;
	}
    template <typename unit, typename type>
        inline
	Number<unit,type>  const operator-(Number<unit,type>  lhs, Number<unit,type> rhs) {
	    return lhs.v-rhs.v;
	}
    template <typename unit, typename type>
        inline
	bool operator!=(Number<unit,type> lhs, Number<unit,type> rhs) {
	    return lhs.v != rhs.v;
	}
    template <typename unit, typename type>
        inline
	bool operator<(Number<unit,type> lhs, Number<unit,type> rhs) {
	    return lhs.v < rhs.v;
	}
    template <typename unit, typename type>
        inline
	bool operator<=(Number<unit,type> lhs, Number<unit,type> rhs) {
	    return lhs.v <= rhs.v;
	}
    template <typename unit, typename type>
        inline
	bool operator==(Number<unit,type> lhs, Number<unit,type> rhs) {
	    return lhs.v == rhs.v;
	}
    template <typename unit, typename type>
        inline
	Number<unit,type> const operator+(Number<unit,type> lhs, Number<unit,type> rhs) {
	    return Number<unit,type>(lhs.v+rhs.v);
	}
    template <typename unit, typename type>
        inline
	Number<unit,type> const operator*(Number<unit,type> lhs, Number<unit,type> rhs) {
	    return Number<unit,type>(lhs.v*rhs.v);
	}

    /** Helper type to define \c Line type. */
    struct Line_t { };
    /** Helper type to define \c Column type. */
    struct Column_t { };
    /** Strongly typed line number type. */
    typedef Number<Line_t, unsigned char> Line;
    /** Strongly typed column number type. */
    typedef Number<Column_t, unsigned char> Column;

    inline
    size_t operator*(Line   l, size_t s) { return l.v * s; }
    inline
    size_t operator*(Column c, size_t s) { return c.v * s; }

    //@}
} // namespace rr

#endif // RR_NUMBER_HPP__
