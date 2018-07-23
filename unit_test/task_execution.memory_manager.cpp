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




BOOST_AUTO_TEST_SUITE(memory_manager_test)

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

        //no copy
        vector<data_bucket> vdb = {db0, db00};

        // copy
        mmg.pre_process(vdb,cxt);


        //copy
        A* a = vdb[0].get_ptr<A>("n0k1");
        A* b = vdb[1].get_ptr<A>("n00k2");

        BOOST_CHECK_EQUAL(a->content, 1);
        BOOST_CHECK_EQUAL(a->id, "node0-key1.copy");
        BOOST_CHECK_EQUAL(b->content, 2);
        BOOST_CHECK_EQUAL(b->id, "node00-key2.copy.copy");


    }


    BOOST_AUTO_TEST_CASE(post_process)
    {
        memory_manager mmg;
        A data1("pa1",1);
        A data2("pa2",2);
        A data3("notuse",3);

        //copy
        data_bucket db0;
        db0.put("pa1", data1);
        db0.put("pa2", data2);
        db0.put("notuse", data3);


        mmg.post_process(db0, cxt);

        A* a = db0.get_ptr<A>("pa1");

        BOOST_CHECK_EQUAL(db0.size(), 2);
        BOOST_CHECK_EQUAL(a->content, 1);
        BOOST_CHECK_EQUAL(a->id, "pa1.copy");
        try {
            A* b = db0.get_ptr<A>("notuse");

        } catch (utils::pvpf_exception &e) {
            BOOST_TEST(string("key:notuse does not exist") == string(e.what()));
            return;
        }

        BOOST_TEST(false);
    }


BOOST_AUTO_TEST_SUITE_END()