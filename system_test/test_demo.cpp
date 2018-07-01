#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE DemoTests
#endif

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(foo_suite)

  BOOST_AUTO_TEST_CASE(constructor_test)
  {
//    BOOST_CHECK_EQUAL(1, 0);
    BOOST_CHECK_EQUAL(1, 0);
  }

BOOST_AUTO_TEST_SUITE_END()
