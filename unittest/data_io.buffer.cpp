//
// Created by Max on 2018/6/23.
//

#ifdef STAND_ALONE
#define BOOST_TEST_MODULE DemoTests
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/data_io/buffer.h>

using namespace pvpf::data_io;

BOOST_AUTO_TEST_SUITE(data_io_buffer_suite)

    BOOST_AUTO_TEST_CASE(sample_test)
    {
        Buffer b;
        b.write(3);
        BOOST_CHECK_EQUAL(3, b.read());
    }

BOOST_AUTO_TEST_SUITE_END()
