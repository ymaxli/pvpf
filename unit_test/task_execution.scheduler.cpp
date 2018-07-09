//
// Created by jiabei on 2018/7/8.
//

#ifdef STAND_ALONE
#define BOOST_TEST_MODULE TaskExecutionScheduler
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/task_execution/scheduler.hpp>
#include <pvpf/task_execution/body.hpp>
#include <rapidjson/document.h>

using namespace std;
using namespace pvpf;
using namespace rapidjson;

BOOST_AUTO_TEST_SUITE(task_execution_scheduler_suite)

    BOOST_AUTO_TEST_CASE(create_context) {
        Document document;
        document.SetObject();
        Document::AllocatorType& allocator = document.GetAllocator();
        document.AddMember("")
    }

BOOST_AUTO_TEST_SUITE_END()
