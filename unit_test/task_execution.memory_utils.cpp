//
// Created by Icay on 16/07/2018.
//

#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE MemoryUtilsTests
#endif


#include <boost/test/unit_test.hpp>
#include <rapidjson/document.h>
#include "pvpf/task_execution/memory_utils.hpp"
#include <vector>
#include <iostream>

using namespace std;
using namespace pvpf::task_execution;


class A {
public:
    string id;
    int content;

    explicit A(string id = "a", int content = 0) noexcept :
            id(id), content(content) {
    }

    A(const A &a) : id(a.id + ".copy"), content(a.content) {
    }
};

BOOST_AUTO_TEST_SUITE(memory_utils_test)

    BOOST_AUTO_TEST_CASE(copy_cpu_single)
    {
        A data("abc",1);

        //copy
        const shared_ptr<core::any> cpu_ptr = make_shared<core::any>(data);

        //copy
        shared_ptr<core::any> new_cpu_ptr = copy_cpu(cpu_ptr);

        //2 copy
        A new_data = core::any_cast<A>(*new_cpu_ptr);
        BOOST_CHECK_EQUAL(new_data.id, "abc.copy.copy.copy.copy");
        BOOST_CHECK_EQUAL(new_data.content, 1);

    }

    BOOST_AUTO_TEST_CASE(copy_cpu_data_bucket)
    {

        A data1("key1", 1);
        A data2("key2", 2);
        data_bucket data_bucket1;
        string key1 = "abc";
        string key2 = "bcd";

        //copy
        data_bucket1.put(key1,data1);
        data_bucket1.put(key2,std::move(data2));

        //copy
        copy_cpu(data_bucket1);
        //copy
        A* a = data_bucket1.get_ptr<A>(key1);
        A* b = data_bucket1.get_ptr<A>(key2);
        BOOST_CHECK_EQUAL(a->content, 1);
        BOOST_CHECK_EQUAL(a->id, "key1.copy.copy.copy");
        BOOST_CHECK_EQUAL(b->content, 2);
        BOOST_CHECK_EQUAL(b->id, "key2.copy.copy.copy");


    }


BOOST_AUTO_TEST_SUITE_END()