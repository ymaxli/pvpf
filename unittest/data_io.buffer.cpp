//
// Created by Max on 2018/6/23.
//

//#ifdef STAND_ALONE
//#define BOOST_TEST_MODULE DemoTests
//#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/data_io/buffer.h>
#include <pvpf/utils/data_bucket.h>
#include <boost/any.hpp>
#include <thread>
#include <chrono>
#include <iostream>

#define THREAD_WAITING_TIME 500

using namespace pvpf::data_io;
using namespace std;

BOOST_AUTO_TEST_SUITE(data_io_buffer_suite)

    BOOST_AUTO_TEST_CASE(write_blocking) {
        buffer b(5, true);
        data_bucket bucket;
        int k = 111;
        bucket.put("abc", k);

        thread first([&]() -> void {
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            cout << "then write" << endl;
        });
        thread second([&]() -> void {
            this_thread::sleep_for(chrono::milliseconds(THREAD_WAITING_TIME));
            cout << "read first" << endl;
            b.read();
        });

        first.join();
        second.join();
    }

    BOOST_AUTO_TEST_CASE(write_non_blocking) {
        buffer b(5, false);
        data_bucket bucket;
        int k = 111;
        bucket.put("abc", k);

        thread first([&]() -> void {
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            b.write(bucket);
            cout << "write finished first" << endl;
        });
        thread second([&]() -> void {
            this_thread::sleep_for(chrono::milliseconds(THREAD_WAITING_TIME));
            b.read();
            this_thread::sleep_for(chrono::milliseconds(THREAD_WAITING_TIME));
            b.read();
            cout << "then read finished" << endl;
        });

        first.join();
        second.join();
    }

    BOOST_AUTO_TEST_CASE(read_waiting) {
        buffer b(5, false);
        data_bucket bucket;
        int k = 111;
        bucket.put("abc", k);

        thread first([&]() -> void {
            this_thread::sleep_for(chrono::milliseconds(THREAD_WAITING_TIME));
            cout << "write first" << endl;
            b.write(bucket);

        });
        thread second([&]() -> void {
            b.read();
            cout << "then read" << endl;
        });

        first.join();
        second.join();
    }

    BOOST_AUTO_TEST_CASE(frame_dropping) {
        buffer b(10, false);
        const int WRITE_FPS = 60;
        const int READ_FPS = 24;
        const float RATIO = (float) WRITE_FPS / READ_FPS;

        thread first([&]() -> void {
            int index = 0;
            while (index < 1000) {
                data_bucket bucket;
                bucket.put("index", index);
                b.write(bucket);
                index++;
                this_thread::sleep_for(chrono::milliseconds(1000 / WRITE_FPS));
            }
        });
        thread second([&]() -> void {
            int last_index = 0;
            for (int i = 0; i < 200; i++) {
                data_bucket result = b.read();
                int index = boost::any_cast<int>(result.get("index"));
                if (i != 0) {
                    BOOST_TEST((index - last_index) < (RATIO + 1));
                    BOOST_TEST((index - last_index) > (RATIO - 1));
                }
                last_index = index;
                this_thread::sleep_for(chrono::milliseconds(1000 / READ_FPS));
            }
        });

        first.join();
        second.join();
    }

    BOOST_AUTO_TEST_CASE(no_frame_dropping) {
        buffer b(10, true);
        const int WRITE_FPS = 60;
        const int READ_FPS = 24;

        thread first([&]() -> void {
            int index = 0;
            while (index < 100) {
                data_bucket bucket;
                bucket.put("index", index);
                b.write(bucket);
                index++;
                this_thread::sleep_for(chrono::milliseconds(1000 / WRITE_FPS));
            }
        });
        thread second([&]() -> void {
            int last_index = 0;
            for (int i = 0; i < 100; i++) {
                data_bucket result = b.read();
                int index = boost::any_cast<int>(result.get("index"));
                if (i != 0) {
                    BOOST_TEST((index - last_index) == 1);
                }
                last_index = index;
                this_thread::sleep_for(chrono::milliseconds(1000 / READ_FPS));
            }
        });

        first.join();
        second.join();
    }

    BOOST_AUTO_TEST_CASE(stop_writing) {
        buffer b(5, false);
        data_bucket bucket;
        int k = 111;
        bucket.put("abc", k);
        b.write(bucket);
        b.stop_writing();
        BOOST_CHECK_EQUAL(b.is_empty(), false);
        b.write(bucket);
        b.read();
        BOOST_CHECK_EQUAL(b.is_empty(), true);
    }

BOOST_AUTO_TEST_SUITE_END()
