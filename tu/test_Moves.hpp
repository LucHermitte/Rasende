//========================================================================
/**@file	rr/tu/test_Moves.hpp
 * @author	Luc Hermitte <EMAIL:hermitte {at} free {dot} fr>
 * @version	«version»
 * @date
 * 	Created:	10th Apr 2010 \n
 * 	Last Update:	10th Apr 2010
 */
/*------------------------------------------------------------------------
 * Description:	«description»
 * 
 *------------------------------------------------------------------------
 * History:	«history»
 * TODO:	«missing features»
 *========================================================================
 */

#ifndef RR_TU_TEST_MOVES_HPP__
#define RR_TU_TEST_MOVES_HPP__

#include <cxxtest/TestSuite.h>
#include "../src/Moves.hpp"

using namespace rr;
class MyTestSuite : public CxxTest::TestSuite
{
public:
    void test_opposite()
    {
        using namespace rr::Directions;
        TS_ASSERT_EQUALS(UP, opposite(DOWN));
        TS_ASSERT_EQUALS(DOWN, opposite(UP));
        TS_ASSERT_EQUALS(UP|DOWN, opposite(UP|DOWN));

        TS_ASSERT_EQUALS(RIGHT, opposite(LEFT));
        TS_ASSERT_EQUALS(LEFT, opposite(RIGHT));
        TS_ASSERT_EQUALS(LEFT|RIGHT, opposite(LEFT|RIGHT));

        TS_ASSERT_EQUALS(UP|LEFT, opposite(DOWN|RIGHT));
        TS_ASSERT_EQUALS(UP|RIGHT, opposite(DOWN|LEFT));
        TS_ASSERT_EQUALS(DOWN|LEFT, opposite(UP|RIGHT));
        TS_ASSERT_EQUALS(DOWN|RIGHT, opposite(UP|LEFT));

        TS_ASSERT_EQUALS(UP|DOWN|LEFT, opposite(UP|DOWN|RIGHT));
        TS_ASSERT_EQUALS(UP|DOWN|RIGHT, opposite(UP|DOWN|LEFT));
        TS_ASSERT_EQUALS(DOWN|LEFT|RIGHT, opposite(UP|LEFT|RIGHT));
        TS_ASSERT_EQUALS(UP|LEFT|RIGHT, opposite(DOWN|LEFT|RIGHT));

        TS_ASSERT_EQUALS(UP|DOWN|LEFT|RIGHT, opposite(UP|DOWN|LEFT|RIGHT));
    }
};

#endif // RR_TU_TEST_MOVES_HPP__
