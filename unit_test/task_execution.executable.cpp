//
// Created by Max on 2018/7/16.
//

#ifdef STAND_ALONE
#define BOOST_TEST_MODULE Executable
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/utils/data_bucket.hpp>
#include <iostream>
#include <pvpf/task_execution/executable.hpp>
#include <vector>
#include <unordered_map>

using namespace std;
using namespace pvpf;
using namespace pvpf::task_execution;

int run1 = 0;
int run2 = 0;
int run3 = 0;
int order = 0;

void test_func_1(data_bucket &data, data_bucket const &params) {
    run1 = 1;
    order = 1;
}

void test_func_2(data_bucket &data, data_bucket const &params) {
    run2 = 2;
    order = 2;
}

void test_func_3(data_bucket &data, data_bucket const &params) {
    run3 = 3;
    order = 3;
}

void test_loop_func_1(data_bucket &data, data_bucket const &params) {
    data.put("piece_out", data.get_copy<int>("piece_in") + data.get_copy<int>("h"));
}

void test_loop_func_2(data_bucket &data, data_bucket const &params) {
    data.put("piece_out", data.get_copy<int>("piece_out") + data.get_copy<int>("q"));
}

BOOST_AUTO_TEST_SUITE(task_execution_executable_suite)

    BOOST_AUTO_TEST_CASE(dynamic_library_func_test) {
        executable *exe = new dynamic_library_func(&test_func_1);

        data_bucket dummy;
        exe->exec(dummy, dummy);
        BOOST_TEST(run1 == 1);

        delete exe;
    }

    BOOST_AUTO_TEST_CASE(normal_algorithm_test) {
        unique_ptr<executable> exe1 = make_unique<dynamic_library_func>(&test_func_1);
        unique_ptr<executable> exe2 = make_unique<dynamic_library_func>(&test_func_2);

        normal_algorithm algo;
        algo.add_executable(std::move(exe1));
        algo.add_executable(std::move(exe2));

        data_bucket dummy;
        algo.exec(dummy, dummy);

        BOOST_TEST(run1 == 1);
        BOOST_TEST(run2 == 2);
    }

    BOOST_AUTO_TEST_CASE(nested_algorithm_test) {
        unique_ptr<executable> exe1 = make_unique<dynamic_library_func>(&test_func_1);
        unique_ptr<executable> exe2 = make_unique<dynamic_library_func>(&test_func_2);
        unique_ptr<executable> exe3 = make_unique<dynamic_library_func>(&test_func_3);

        auto algo = make_unique<normal_algorithm>();
        algo->add_executable(std::move(exe1));
        algo->add_executable(std::move(exe2));

        normal_algorithm algo2;
        algo2.add_executable(std::move(algo));
        algo2.add_executable(std::move(exe3));

        data_bucket dummy;
        algo2.exec(dummy, dummy);

        BOOST_TEST(run1 == 1);
        BOOST_TEST(run2 == 2);
        BOOST_TEST(run3 == 3);
        BOOST_TEST(order == 3);
    }

    BOOST_AUTO_TEST_CASE(loop_algorithm_test) {
        vector<int> vec;
        for (int i = 0; i < 10; i++) vec.push_back(i);
        data_bucket data;
        data.put("h", 10);
        data.put("q", 20);
        data.put_collection_by_move("piece_in", vec);

        unordered_map<string, string> key_map;
        key_map["piece_in"] = "piece_out";

        unique_ptr<executable> exe1 = make_unique<dynamic_library_func>(&test_loop_func_1);
        unique_ptr<executable> exe2 = make_unique<dynamic_library_func>(&test_loop_func_2);

        loop_algorithm algo(key_map);
        algo.add_executable(std::move(exe1));
        algo.add_executable(std::move(exe2));

        algo.exec(data, data);

        for (int i = 0; i < 10; i++) {
            BOOST_TEST(*data.get_item_in_collection<int>("piece_out", i) == (i + 30));
        }
    }

    BOOST_AUTO_TEST_CASE(loop_algorithm_exception) {
        vector<int> vec, vec2;
        for (int i = 0; i < 10; i++) vec.push_back(i);
        for (int i = 0; i < 20; i++) vec2.push_back(i);
        data_bucket data;
        data.put("h", 10);
        data.put("q", 20);
        data.put_collection_by_move("piece_in", vec);
        data.put_collection_by_move("piece_in2", vec2);

        unordered_map<string, string> key_map;
        key_map["piece_in"] = "piece_out";
        key_map["piece_in2"] = "piece_out2";

        unique_ptr<executable> exe1 = make_unique<dynamic_library_func>(&test_loop_func_1);
        unique_ptr<executable> exe2 = make_unique<dynamic_library_func>(&test_loop_func_2);

        loop_algorithm algo(key_map);
        algo.add_executable(std::move(exe1));
        algo.add_executable(std::move(exe2));

        try {
            algo.exec(data, data);
        } catch (utils::pvpf_exception &e) {
            BOOST_TEST(string("sizes of data objects in loop algorithm do not equal") == string(e.what()));
            return;
        }

        BOOST_TEST(false);
    }

BOOST_AUTO_TEST_SUITE_END()