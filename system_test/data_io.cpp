//
// Created by Max on 2018/7/2.
//

#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE DemoTests
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/data_io/factory.hpp>
#include <pvpf/data_io/io_pipe.hpp>
#include <pvpf/utils/data_bucket.hpp>
#include <thread>
#include <chrono>
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace pvpf::data_io;
using namespace pvpf;
using namespace std;

string const &TEST_IMAGE_DIR("test_images/");

BOOST_AUTO_TEST_SUITE(system_testing_data_io_suite)

    BOOST_AUTO_TEST_CASE(read_continuous_data) {
        auto pair = create_source(10, true);
        auto source = std::move(std::get<0>(pair));
        auto node = std::move(std::get<1>(pair));

        cv::Mat images[7];
        cout<<"before thread"<<endl;

        std::thread first([&]() -> void {
            for (int i = 0; i < 6; i++) {
                images[i] = cv::imread(TEST_IMAGE_DIR + to_string(i + 1) + string(".png"));
                source->source_write("kkk", images[i]);
            }
            cout<<"before complete"<<endl;
            source->source_complete();
            cout<<"after complete"<<endl;
        });
        std::thread second([&]() -> void {
            int index = 0;
            while (!node->is_empty()) {
                cout<<index<<endl;
                data_bucket data = node->read();
                auto result = data.get_copy<cv::Mat>("kkk");
                bool isEqual = (cv::sum(images[index] != result) == cv::Scalar(0, 0, 0, 0));
                BOOST_TEST(isEqual);
                index++;
            }
            cout<<"after while"<<endl;
        });
        cout<<"after thread"<<endl;

        first.join();
        second.join();
        cout<<"read_continuous_data"<<endl;
    }

    BOOST_AUTO_TEST_CASE(read_single_data) {
        auto pair = create_source(10, true);
        auto source = std::move(std::get<0>(pair));
        auto node = std::move(std::get<1>(pair));

        cv::Mat image;

        std::thread first([&]() -> void {
            image = cv::imread(TEST_IMAGE_DIR + string("4.png"));
            source->source_write("bbb", image);
            source->source_complete();
        });
        std::thread second([&]() -> void {
            while (!node->is_empty()) {
                data_bucket data = node->read();
                auto result = data.get_copy<cv::Mat>("bbb");
                bool isEqual = (cv::sum(image != result) == cv::Scalar(0, 0, 0, 0));
                BOOST_TEST(isEqual);
            }
        });

        first.join();
        second.join();
        cout<<"read_single_data"<<endl;
    }

    BOOST_AUTO_TEST_CASE(write_continuous_data) {
        auto pair = create_sink(10, true);
        auto sink = std::move(std::get<0>(pair));
        auto node = std::move(std::get<1>(pair));

        cv::Mat images[7];

        std::thread first([&]() -> void {
            for (int i = 0; i < 7; i++) {
                images[i] = cv::imread(TEST_IMAGE_DIR + to_string(i + 1) + string(".png"));
                data_bucket data;
                data.put("abc", images[i]);
                node->write(std::move(data));
            }
            node->stop_writing();
        });
        std::thread second([&]() -> void {
            int index = 0;
            while (!sink->sink_is_complete()) {
                cv::Mat result = sink->sink_read().get_copy<cv::Mat>("abc");
                bool isEqual = (cv::sum(images[index] != result) == cv::Scalar(0, 0, 0, 0));
                BOOST_TEST(isEqual);
                index++;
            }
        });

        first.join();
        second.join();
        cout<<"write_continuous_data"<<endl;
    }

    BOOST_AUTO_TEST_CASE(write_single_data) {
        auto pair = create_sink(10, true);
        auto sink = std::move(std::get<0>(pair));
        auto node = std::move(std::get<1>(pair));

        std::map<string, int> map;
        string key("a");
        map[key] = 123;

        std::thread first([&]() -> void {
            data_bucket data;
            data.put("abc", std::move(map));
            node->write(std::move(data));
            node->stop_writing();
        });
        std::thread second([&]() -> void {
            while (!sink->sink_is_complete()) {
                auto data = sink->sink_read();
                auto result = data.get_ptr<std::map<string, int>>("abc");
                BOOST_TEST((*result)[key] == 123);
            }
        });

        first.join();
        second.join();
        cout<<"write_single_data"<<endl;
    }

BOOST_AUTO_TEST_SUITE_END()
