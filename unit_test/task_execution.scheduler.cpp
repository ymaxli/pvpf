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
#include <pvpf/task_execution/context_generator.hpp>

BOOST_AUTO_TEST_SUITE(task_execution_scheduler_suite)

using namespace std;
using namespace pvpf;
using namespace rapidjson;
using namespace pvpf::os_agnostic;

Document load_json_conf(const string &file)
{
    boost::filesystem::path filePath(file);
    Document d;

    if (!boost::filesystem::exists(filePath))
    {
        cerr << "Configuration file not found" << endl;
        return d;
    }
    else if (boost::filesystem::is_regular_file(filePath))
    {
        string extension = filePath.extension().string();
        if (extension != ".json")
        {
            cerr << "Path shoud be a json file" << endl;
            return d;
        }
    }
    else
    {
        cerr << "Path should be a json file" << endl;
        return d;
    }

    boost::filesystem::ifstream inFile;
    inFile.open(filePath); //open the input file

    stringstream strStream;
    strStream << inFile.rdbuf();  //read the file
    string str = strStream.str(); //str holds the content of the file

    char char_array[str.size() + 1];
    strcpy(char_array, str.c_str());

    d.Parse(char_array);

    return d;
}

// BOOST_AUTO_TEST_CASE(node_id) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][0], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).node_id,"node-1");
// }

// BOOST_AUTO_TEST_CASE(pre) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][0], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).pre.size(),1);
//     BOOST_CHECK_EQUAL((*cont_ptr).pre[0],"source-1");
// }

// BOOST_AUTO_TEST_CASE(successor) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][0], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).succ.size(),2);
//     BOOST_CHECK_EQUAL((*cont_ptr).succ[0], "sink-1");
//     BOOST_CHECK_EQUAL((*cont_ptr).succ[1], "sink-2");
// }

// BOOST_AUTO_TEST_CASE(input) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][0], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).input_mapping.size(),2);
//     BOOST_CHECK_EQUAL((*cont_ptr).input_mapping["image_arr"].size(), 1);
//     BOOST_CHECK_EQUAL((*cont_ptr).input_mapping["image_arr"][0].first, 0);
//     BOOST_CHECK_EQUAL((*cont_ptr).input_mapping["image_arr"][0].second, "key1");
//     BOOST_CHECK_EQUAL((*cont_ptr).input_mapping["image_arr2"].size(), 2);
//     BOOST_CHECK_EQUAL((*cont_ptr).input_mapping["image_arr2"][0].first, 0);
//     BOOST_CHECK_EQUAL((*cont_ptr).input_mapping["image_arr2"][0].second, "key1");
//     BOOST_CHECK_EQUAL((*cont_ptr).input_mapping["image_arr2"][1].first, 0);
//     BOOST_CHECK_EQUAL((*cont_ptr).input_mapping["image_arr2"][1].second, "key1");

// }

// BOOST_AUTO_TEST_CASE(is_cpu_set_gpu_true) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][0], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).is_cpu, false);
// }

// BOOST_AUTO_TEST_CASE(is_cpu_set_gpu_false) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][1], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).is_cpu, true);
// }

// BOOST_AUTO_TEST_CASE(is_cpu_not_set_gpu) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][2], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).is_cpu, true);
// }

// BOOST_AUTO_TEST_CASE(pre_is_cpu) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][0], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).pre_is_cpu.size(), 1);
//     BOOST_CHECK_EQUAL((*cont_ptr).pre_is_cpu[0], true);
// }

// BOOST_AUTO_TEST_CASE(read_only_set_to) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][0], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).read_only.size(), 1);
//     BOOST_CHECK_EQUAL((*cont_ptr).read_only[0], true);
// }

// BOOST_AUTO_TEST_CASE(read_only_not_set) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][1], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).read_only.size(), 1);
//     BOOST_CHECK_EQUAL((*cont_ptr).read_only[0], true);
// }

// BOOST_AUTO_TEST_CASE(read_only_set_false) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][2], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).read_only.size(), 2);
//     BOOST_CHECK_EQUAL((*cont_ptr).read_only[0], true);
//     BOOST_CHECK_EQUAL((*cont_ptr).read_only[1], false);
// }

// BOOST_AUTO_TEST_CASE(output) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["graph"][0], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).output_mapping.size(), 2);
//     BOOST_CHECK_EQUAL((*cont_ptr).output_mapping["result_key"],"output_key");
//     BOOST_CHECK_EQUAL((*cont_ptr).output_mapping["key1"],"hea");
// }

// BOOST_AUTO_TEST_CASE(data) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     shared_ptr<task_execution::context> cont = task_execution::create_context(d["source"][0], sch);
//     task_execution::context* cont_ptr = cont.get();
//     BOOST_CHECK_EQUAL((*cont_ptr).output_data["key1"],"any");
//     BOOST_CHECK_EQUAL((*cont_ptr).output_data["key2"],"int");
// }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(io_node)

using namespace std;
using namespace pvpf;
using namespace rapidjson;
using namespace pvpf::os_agnostic;

dynamic_lib_func_manager &manager = dynamic_lib_func_manager::get_instance();

Document load_json_conf(const string &file)
{
    boost::filesystem::path filePath(file);
    Document d;

    if (!boost::filesystem::exists(filePath))
    {
        cerr << "Configuration file not found" << endl;
        return d;
    }
    else if (boost::filesystem::is_regular_file(filePath))
    {
        string extension = filePath.extension().string();
        if (extension != ".json")
        {
            cerr << "Path shoud be a json file" << endl;
            return d;
        }
    }
    else
    {
        cerr << "Path should be a json file" << endl;
        return d;
    }

    boost::filesystem::ifstream inFile;
    inFile.open(filePath); //open the input file

    stringstream strStream;
    strStream << inFile.rdbuf();  //read the file
    string str = strStream.str(); //str holds the content of the file

    char char_array[str.size() + 1];
    strcpy(char_array, str.c_str());

    d.Parse(char_array);

    return d;
}

// BOOST_AUTO_TEST_CASE(source) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     sch.source_node_list(d["source"]);
//     cout<<"here is the start"<<endl;
//     sch.start_source_functions();
//     sch.stop_io_threads();
// }

// BOOST_AUTO_TEST_CASE(sink) {
//     Document d = load_json_conf("./test_json/context.json");
//     task_execution::scheduler sch;
//     task_execution::figure_out_json_object(d, sch);
//     sch.source_node_list(d["source"]);
//     cout<<"here is the start"<<endl;
//     sch.start_source_functions();
//     sch.stop_io_threads();
// }

BOOST_AUTO_TEST_SUITE_END()