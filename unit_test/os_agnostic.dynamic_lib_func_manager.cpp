//
// Created by Max on 2018/7/14.
//
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE OS_AGNOSTIC_DYNAMIC_LIB_FUNC_MANAGER
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/os_agnostic/dynamic_lib_func_manager.hpp>
#include <boost/filesystem/path.hpp>
#include <pvpf/utils/data_bucket.hpp>
#include <pvpf/utils/exception.hpp>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include <pvpf/data_io/factory.hpp>

using namespace std;
using namespace pvpf;
using namespace pvpf::os_agnostic;
using namespace boost::filesystem;

BOOST_AUTO_TEST_SUITE(os_agnostic_dynamic_lib_func_manager_suite)

    string const &TEST_IMAGE_DIR("./test_images/");
    dynamic_lib_func_manager &manager = dynamic_lib_func_manager::get_instance();

    BOOST_AUTO_TEST_CASE(load_algorithm_dylib) {
        path p("./test_dylib/algorithm.dylib");

        auto ptr = manager.load_algorithm(p, "test1");

        data_bucket data;
        data_bucket params;
        auto image = cv::imread(TEST_IMAGE_DIR + string("1.png"));
        data.put("image", image);

        ptr(data, params);

        bool isEqual = (cv::sum(image != data.get_copy<cv::Mat>("result")) == cv::Scalar(0, 0, 0, 0));
        BOOST_TEST(isEqual);
    }

    BOOST_AUTO_TEST_CASE(load_algorithm_dylib_cached) {
        path p("./test_dylib/algorithm2.dylib");

        auto ptr1 = manager.load_algorithm(p, "test1");
        auto ptr2 = manager.load_algorithm(p, "test2");
        auto ptr3 = manager.load_algorithm(p, "test1");

        data_bucket data;
        data_bucket params;
        auto image = cv::imread(TEST_IMAGE_DIR + string("1.png"));
        data.put("image", image);

        ptr1(data, params);
        bool isEqual = (cv::sum(image != data.get_copy<cv::Mat>("result")) == cv::Scalar(0, 0, 0, 0));
        BOOST_TEST(isEqual);

        params.put("p", 10);
        ptr2(data, params);
        int a = data.get_copy<int>("result");
        BOOST_TEST(a == 10);

        ptr3(data, params);
        isEqual = (cv::sum(image != data.get_copy<cv::Mat>("result")) == cv::Scalar(0, 0, 0, 0));
        BOOST_TEST(isEqual);
    }

    BOOST_AUTO_TEST_CASE(load_algorithm_dylib_exception_1) {
        path p("./test_dylib/algor.dylib");

        try {
            manager.load_algorithm(p, "test1");

        } catch (utils::pvpf_exception &e) {
            BOOST_TEST(string(e.what()).rfind("could not open library: ./test_dylib/algor.dylib", 0) == 0);
            return;
        }

        BOOST_TEST(false);
    }

    BOOST_AUTO_TEST_CASE(load_algorithm_dylib_exception_2) {
        path p("./test_dylib/algorithm.dylib");

        try {
            manager.load_algorithm(p, "tes");

        } catch (utils::pvpf_exception &e) {
            BOOST_TEST(string(e.what()).rfind("could not find function: tes", 0) == 0);
            return;
        }

        BOOST_TEST(false);
    }

    BOOST_AUTO_TEST_CASE(load_and_invoke_input_func_dylib) {
        path p("./test_dylib/io_lib.dylib");

        auto id = manager.load_input_func(p, "input");

        auto pair = data_io::create_source(10, false);
        auto source_io_pipe = std::move(get<0>(pair));
        manager.invoke_io_func(id, *source_io_pipe);
    }

    BOOST_AUTO_TEST_CASE(load_and_invoke_output_func_dylib) {
        path p("./test_dylib/io_lib.dylib");

        auto id = manager.load_output_func(p, "output");

        auto pair = data_io::create_sink(10, false);
        auto sink_io_pipe = std::move(get<0>(pair));
        manager.invoke_io_func(id, *sink_io_pipe);
    }

    BOOST_AUTO_TEST_CASE(input_func_dylib_cached) {
        path p("./test_dylib/io_lib.dylib");

        auto id = manager.load_input_func(p, "input");

        BOOST_TEST(id == 0);
    }

    BOOST_AUTO_TEST_CASE(output_func_dylib_cached) {
        path p("./test_dylib/io_lib.dylib");

        auto id = manager.load_output_func(p, "output");

        BOOST_TEST(id == 0);
    }

    BOOST_AUTO_TEST_CASE(load_output_func_dylib_uncached) {
        path p("./test_dylib/io_lib2.dylib");

        auto id = manager.load_output_func(p, "output");

        BOOST_TEST(id == 1);
    }

    BOOST_AUTO_TEST_CASE(load_input_func_dylib_exception_1) {
        path p("./test_dylib/algor.dylib");

        try {
            manager.load_input_func(p, "test1");

        } catch (utils::pvpf_exception &e) {
            BOOST_TEST(string(e.what()).rfind("could not open library: ./test_dylib/algor.dylib", 0) == 0);
            return;
        }

        BOOST_TEST(false);
    }

    BOOST_AUTO_TEST_CASE(load_input_func_dylib_exception_2) {
        path p("./test_dylib/io_lib.dylib");

        try {
            manager.load_input_func(p, "test1");

        } catch (utils::pvpf_exception &e) {
            BOOST_TEST(string("could not find function: test1") == string(e.what()));
            return;
        }

        BOOST_TEST(false);
    }

    BOOST_AUTO_TEST_CASE(load_output_func_dylib_exception_1) {
        path p("./test_dylib/algor.dylib");

        try {
            manager.load_output_func(p, "test1");

        } catch (utils::pvpf_exception &e) {
            BOOST_TEST(string(e.what()).rfind("could not open library: ./test_dylib/algor.dylib", 0) == 0);
            return;
        }

        BOOST_TEST(false);
    }

    BOOST_AUTO_TEST_CASE(load_output_func_dylib_exception_2) {
        path p("./test_dylib/io_lib.dylib");

        try {
            manager.load_output_func(p, "test1");

        } catch (utils::pvpf_exception &e) {
            BOOST_TEST(string("could not find function: test1") == string(e.what()));
            return;
        }

        BOOST_TEST(false);
    }

    BOOST_AUTO_TEST_CASE(invoke_inputput_func_exception) {
        try {
            auto pair = data_io::create_source(10, false);
            auto source_io_pipe = std::move(get<0>(pair));
            manager.invoke_io_func(10, *source_io_pipe);

        } catch (utils::pvpf_exception &e) {
            BOOST_TEST(string("input library id: 10 not found") == string(e.what()));
            return;
        }

        BOOST_TEST(false);
    }

    BOOST_AUTO_TEST_CASE(invoke_output_func_exception) {
        try {
            auto pair = data_io::create_sink(10, false);
            auto sink_io_pipe = std::move(get<0>(pair));
            manager.invoke_io_func(10, *sink_io_pipe);

        } catch (utils::pvpf_exception &e) {
            BOOST_TEST(string("output library id: 10 not found") == string(e.what()));
            return;
        }

        BOOST_TEST(false);
    }

BOOST_AUTO_TEST_SUITE_END()
