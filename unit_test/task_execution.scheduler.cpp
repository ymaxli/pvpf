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
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>

using namespace std;
using namespace pvpf;
using namespace rapidjson;

Document load_json_conf(const string &file) {
    boost::filesystem::path filePath(file);
    Document d;

    if (!boost::filesystem::exists(filePath)) {
        cerr << "Configuration file not found"<< endl;
        return d;
    }
    else if (boost::filesystem::is_regular_file(filePath)) {
        string extension = filePath.extension().string();
        if(extension != ".json") {
            cerr << "Path shoud be a json file" << endl;
            return d;
        }
    }
    else {
        cerr << "Path should be a json file" << endl;
        return d;
    }

    boost::filesystem::ifstream inFile;
    inFile.open(filePath);//open the input file

    stringstream strStream;
    strStream << inFile.rdbuf();//read the file
    string str = strStream.str();//str holds the content of the file

    char char_array[str.size()+1];
    strcpy(char_array, str.c_str());

    d.Parse(char_array);

    return d;
}

BOOST_AUTO_TEST_SUITE(task_execution_scheduler_suite)

    BOOST_AUTO_TEST_CASE(create_context) {
        Document d = load_json_conf("./test_json/context.json");
        task_execution::scheduler sch;
        shared_ptr<task_execution::context> cont = sch.create_context(d["graph"][0], d);
        task_execution::context* cont_ptr = cont.get();
        BOOST_CHECK_EQUAL((*cont_ptr).node_id,"node-1");
    }



BOOST_AUTO_TEST_SUITE_END()
