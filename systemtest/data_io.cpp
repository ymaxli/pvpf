//
// Created by Max on 2018/6/25.
//

#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE DemoTests
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/utils/data_bucket.h>
#include <pvpf/data_io/factory.h>
#include <pvpf/data_io/io_pipe.h>
#include <thread>
#include <chrono>

using namespace pvpf::data_io;
using namespace std;

struct TEST_IMAGE {
    int a;
};

BOOST_AUTO_TEST_SUITE(data_io_system_test_suite)

    BOOST_AUTO_TEST_CASE(read_one_image) {
        auto pipes = create_source(10, false);
        unique_ptr<source_io_pipe>& data_io_pipe = get<0>(pipes);
        unique_ptr<io_pipe_for_source_node>& source_node = get<1>(pipes);

        thread first([&]() -> void {
            TEST_IMAGE image{10};
            data_io_pipe->source_write("image", image);
            data_io_pipe->source_complete();
        });
        thread second([&]() -> void {
            while (!source_node ->is_empty()) {
                data_bucket data = source_node->read();
                auto image = boost::any_cast<TEST_IMAGE>(data.get("image"));
                BOOST_TEST(image.a == 10);
            }
        });

        first.join();
        second.join();
    }

BOOST_AUTO_TEST_SUITE_END()
