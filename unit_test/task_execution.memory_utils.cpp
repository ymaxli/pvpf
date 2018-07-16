//
// Created by Icay on 16/07/2018.
//

#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE ConfigReaderTests
#endif


#include <boost/test/unit_test.hpp>
#include <rapidjson/document.h>
#include "pvpf/task_execution/memory_utils.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace pvpf::task_execution;


BOOST_AUTO_TEST_SUITE(memory_utils_test)

    BOOST_AUTO_TEST_CASE(copy_cpu_single)
    {
        vector<int> data = {12};
        const shared_ptr<core::any> cpu_ptr = make_shared<core::any>(data);
        shared_ptr<core::any> new_cpu_ptr = copy_cpu(cpu_ptr);
        vector<int> new_data = core::any_cast<vector<int>>(*cpu_ptr);
        BOOST_CHECK_EQUAL(new_data.size(), 1);
        BOOST_CHECK_EQUAL(new_data[0], 12);

    }

    BOOST_AUTO_TEST_CASE(copy_cpu_data_bucket)
    {
        std::unordered_map<std::string, std::shared_ptr<core::any>> map;
        int data1 = 1;
        map["0"] = make_shared<core::any>(data1);
//        vector<int> data = {12};
//        const shared_ptr<core::any> cpu_ptr = make_shared<core::any>(data);
//        shared_ptr<core::any> new_cpu_ptr = copy_cpu(cpu_ptr);
//        vector<int> new_data = core::any_cast<vector<int>>(*cpu_ptr);
//        BOOST_CHECK_EQUAL(new_data.size(), 1);
//        BOOST_CHECK_EQUAL(new_data[0], 12);

    }


BOOST_AUTO_TEST_SUITE_END()