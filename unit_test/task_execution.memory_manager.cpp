//
// Created by Icay on 16/07/2018.
//

#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE MemoryManagerTests
#endif


#include <boost/test/unit_test.hpp>
#include <rapidjson/document.h>
#include "pvpf/task_execution/memory_manager.hpp"
#include <vector>
#include <unordered_map>
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

string id = "node-1";
vector<string> pre = {"node-0","node-00"};
std::vector<string> succ = {"node-2","node-22"};
unordered_map<string, vector<pair<int, std::string> >> input;
unordered_map<string, string> output = {{ "okey1", "pa1" },{ "okey2", "pa2" }};
unordered_map<string, string> data = {{}};
vector<bool> read_only = {true, false};
vector<bool> pre_is_cpu = {true,true};
bool is_cpu = true;

context cxt = context(id, pre, succ, input,output,data,read_only,pre_is_cpu,is_cpu);

BOOST_AUTO_TEST_SUITE(memory_manager_test)

    BOOST_AUTO_TEST_CASE(pre_process_cpu_to_cpu)
    {
        memory_manager mmg;
        A data1("node0-key1",1);
        A data2("node0-key2",2);
        A data3("node00-key1",1);
        A data4("node00-key2",2);

        //copy
        data_bucket db0;
        db0.put("n0k1", data1);
        db0.put("n0k2", data2);
        data_bucket db00;
        db00.put("n00k1", data3);
        db00.put("n00k2", data4);

        //copy
        vector<data_bucket> vdb = {db0, db00};

        mmg.pre_process(vdb,cxt);


    }


    BOOST_AUTO_TEST_CASE(post_process)
    {

    }


BOOST_AUTO_TEST_SUITE_END()