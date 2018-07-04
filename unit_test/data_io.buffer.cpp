////
//// Created by Max on 2018/6/23.
////
//
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE DataIOBuffer
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/data_io/buffer.hpp>
#include <pvpf/utils/data_bucket.hpp>
#include <thread>
#include <chrono>
#include <iostream>

#define THREAD_WAITING_TIME 500

using namespace pvpf::data_io;
using namespace pvpf;
using namespace std;

BOOST_AUTO_TEST_SUITE(data_io_buffer_suite)

    BOOST_AUTO_TEST_CASE(write_blocking) {
        buffer b(5, true);
        data_bucket bucket;
        bucket.put("abc", 111);

        int flag = 0;

        thread first([&]() -> void {
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            if (flag == 0) flag = 2;
        });
        thread second([&]() -> void {
            this_thread::sleep_for(chrono::milliseconds(THREAD_WAITING_TIME));
            flag = 1;
            b.read();
        });

        first.join();
        second.join();

        BOOST_CHECK_EQUAL(flag, 1);
    }

    BOOST_AUTO_TEST_CASE(write_non_blocking) {
        buffer b(5, false);
        data_bucket bucket;
        bucket.put("abc", 111);

        int flag = 0;

        thread first([&]() -> void {
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));
            b.write(std::move(bucket));

            if(flag == 0) flag = 1;
        });
        thread second([&]() -> void {
            this_thread::sleep_for(chrono::milliseconds(THREAD_WAITING_TIME));
            b.read();
            this_thread::sleep_for(chrono::milliseconds(THREAD_WAITING_TIME));
            b.read();

            if(flag == 0) flag = 2;
        });

        first.join();
        second.join();

        BOOST_CHECK_EQUAL(flag, 1);
    }

    BOOST_AUTO_TEST_CASE(read_waiting) {
        buffer b(5, false);
        data_bucket bucket;
        bucket.put("abc", 111);

        int flag = 0;

        thread first([&]() -> void {
            this_thread::sleep_for(chrono::milliseconds(THREAD_WAITING_TIME));
            flag = 1;
            b.write(std::move(bucket));

        });
        thread second([&]() -> void {
            b.read();
            if(flag == 0) flag = 2;
        });

        first.join();
        second.join();

        BOOST_CHECK_EQUAL(flag, 1);
    }

    BOOST_AUTO_TEST_CASE(frame_dropping) {
        buffer b(10, false);
        const int WRITE_FPS = 60;
        const int READ_FPS = 24;
        const float RATIO = (float) WRITE_FPS / READ_FPS;

        thread first([&]() -> void {
            int index = 0;
            while (index < 600) {
                data_bucket bucket;
                bucket.put("index", index);
                b.write(std::move(bucket));
                index++;
                this_thread::sleep_for(chrono::milliseconds(1000 / WRITE_FPS));
            }
        });
        thread second([&]() -> void {
            int last_index = 0;
            for (int i = 0; i < 200; i++) {
                data_bucket result = b.read();
                auto index = result.get_copy<int>("index");
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
                b.write(std::move(bucket));
                index++;
                this_thread::sleep_for(chrono::milliseconds(1000 / WRITE_FPS));
            }
        });
        thread second([&]() -> void {
            int last_index = 0;
            for (int i = 0; i < 100; i++) {
                data_bucket result = b.read();
                auto index = result.get_copy<int>("index");
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
        b.write(std::move(bucket));
        b.stop_writing();
        BOOST_CHECK_EQUAL(b.is_empty(), false);
        b.write(std::move(bucket));
        b.read();
        BOOST_CHECK_EQUAL(b.is_empty(), true);
    }

BOOST_AUTO_TEST_SUITE_END()
