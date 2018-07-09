//
// Created by Icay on 23/06/2018.
//


#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE ConfigValidatorTests
#endif


#include <boost/test/unit_test.hpp>
#include <pvpf/config/validation_rule.hpp>
#include <rapidjson/document.h>

using namespace std;
using namespace rapidjson;
using namespace pvpf::config;

BOOST_AUTO_TEST_SUITE(config_validation_rule_test)

    // concrete_rule_format check
    BOOST_AUTO_TEST_CASE(rule_valid_format)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_format(d);

        BOOST_CHECK_EQUAL(res.get_type(), 0);
        BOOST_CHECK_EQUAL(res.get_message(), "Pass: valid JSON file");
    }

    BOOST_AUTO_TEST_CASE(rule_invalid_format)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [,\n"
                           "    \"graph\": ],\n"
                           "    \"sink\": []\n"
                           "";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_format(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: invalid JSON file");
    }

    // concrete_rule_source check
    BOOST_AUTO_TEST_CASE(rule_lack_source)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: lack \"source\" field");
    }

    BOOST_AUTO_TEST_CASE(rule_empty_source_array)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: \"source\" field has to have at least one child");
    }

    BOOST_AUTO_TEST_CASE(rule_source_empty_object)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [{}],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: \"source\" field has to have at least one child");
    }

    BOOST_AUTO_TEST_CASE(rule_source_not_array)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": 1,\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: the content of \"source\" field is not an array");
    }

    BOOST_AUTO_TEST_CASE(rule_source_without_id)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"/usr/local/libraries/a.so\",\n"
                           "                    \"func\": \"func1\"\n"
                           "                }\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"data\": {\n"
                           "                    \"image\": \"any\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: node has to have an id");
    }

    BOOST_AUTO_TEST_CASE(rule_source_id_not_string)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": 1,\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"/usr/local/libraries/a.so\",\n"
                           "                    \"func\": \"func1\"\n"
                           "                }\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"data\": {\n"
                           "                    \"image\": \"any\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node id should be nonempty string");
    }

    BOOST_AUTO_TEST_CASE(rule_source_id_length)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"\",\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"/usr/local/libraries/a.so\",\n"
                           "                    \"func\": \"func1\"\n"
                           "                }\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"data\": {\n"
                           "                    \"image\": \"any\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node id should be nonempty string");
    }

    BOOST_AUTO_TEST_CASE(rule_source_without_task)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"data\": {\n"
                           "                    \"image\": \"any\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node has to have a task field");
    }

    BOOST_AUTO_TEST_CASE(rule_source_task_not_object)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"task\": \"\",\n"
                           "            \"output\": {\n"
                           "                \"data\": {\n"
                           "                    \"image\": \"any\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node task should be an object");
    }

    BOOST_AUTO_TEST_CASE(rule_source_task_no_dylib)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"task\": {},\n"
                           "            \"output\": {\n"
                           "                \"data\": {\n"
                           "                    \"image\": \"any\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node task should have a dylib field");
    }

    BOOST_AUTO_TEST_CASE(rule_source_task_dylib_no_func)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                }\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"data\": {\n"
                           "                    \"image\": \"any\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node task dylib should have location and func");
    }

    BOOST_AUTO_TEST_CASE(rule_source_task_dylib_func_empty)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"\",\n"
                           "                    \"func\": \"\"\n"
                           "                }\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"data\": {\n"
                           "                    \"image\": \"any\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node task dylib and func should be nonempty string");
    }


    BOOST_AUTO_TEST_CASE(rule_source_without_output)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"/usr/local/libraries/a.so\",\n"
                           "                    \"func\": \"func1\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node has to have an output field");
    }


    BOOST_AUTO_TEST_CASE(rule_source_output_not_object)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"/usr/local/libraries/a.so\",\n"
                           "                    \"func\": \"func1\"\n"
                           "                }\n"
                           "            },\n"
                           "            \"output\": \"\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node output should be an object");
    }

    BOOST_AUTO_TEST_CASE(rule_source_output_no_data)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"/usr/local/libraries/a.so\",\n"
                           "                    \"func\": \"func1\"\n"
                           "                }\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node output should have a data field");
    }



    BOOST_AUTO_TEST_CASE(rule_valid_source)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"/usr/local/libraries/a.so\",\n"
                           "                    \"func\": \"func1\"\n"
                           "                }\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"data\": {\n"
                           "                    \"image\": \"any\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_source(d);

        BOOST_CHECK_EQUAL(res.get_type(), 0);
        BOOST_CHECK_EQUAL(res.get_message(), "Pass: source field check");
    }

    ///////////////////////graph



    BOOST_AUTO_TEST_CASE(rule_lack_graph)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: lack \"graph\" field");
    }

    BOOST_AUTO_TEST_CASE(rule_empty_graph)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: \"graph\" field has to have at least one child");
    }


    BOOST_AUTO_TEST_CASE(rule_graph_not_array)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": 1,\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: the content of \"graph\" field is not an array");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_empty_object)
    {
        const char* json = "{\n"
                           "    \"meta\": {},\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {}\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: \"graph\" field has to have at least one child");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_without_id)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"algorithm\": \"algorithm_1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: node has to have an id");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_id_empty_string)
    {
        const char* json = "{\n"
                           "    \"meta\": {},\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"algorithm\": \"algorithm_1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node id should be nonempty string");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_without_task)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node has to have a task field");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_task_not_object)
    {
        const char* json = "{\n"
                           "    \"meta\": {},\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            },\n"
                           "            \"task\": \"\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node task should be an object");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_task_no_algorithm)
    {
        const char* json = "{\n"
                           "    \"meta\": {},\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"a\": \"algorithm_1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node task should have an algorithm field");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_task_algorithm_empty_string)
    {
        const char* json = "{\n"
                           "    \"meta\": {},\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"algorithm\": \"\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node task algorithm should be nonempty string");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_without_input)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"task\": {\n"
                           "                \"algorithm\": \"algorithm_1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node has to have an input field");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_input_not_object)
    {
        const char* json = "{\n"
                           "    \"meta\": {},\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": \"\",\n"
                           "            \"task\": {\n"
                           "                \"algorithm\": \"algorithm_1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node input should be an object");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_input_no_pre)
    {
        const char* json = "{\n"
                           "    \"meta\": {},\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"algorithm\": \"algorithm_1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node input should have a pre field");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_input_pre_empty_string)
    {
        const char* json = "{\n"
                           "    \"meta\": {},\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"algorithm\": \"algorithm_1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node input pre should be nonempty string or string list");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_input_pre_list_empty_string)
    {
        const char* json = "{\n"
                           "    \"meta\": {},\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\"source-1\", \"\"]\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"algorithm\": \"algorithm_1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node input pre should be nonempty string or string list");
    }


    BOOST_AUTO_TEST_CASE(rule_valid_graph)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"algorithm\": \"algorithm_1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_graph(d);

        BOOST_CHECK_EQUAL(res.get_type(), 0);
        BOOST_CHECK_EQUAL(res.get_message(), "Pass: graph field check");
    }



    /////////////////////sink


    BOOST_AUTO_TEST_CASE(rule_lack_sink)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: lack \"sink\" field");
    }

    BOOST_AUTO_TEST_CASE(rule_empty_sink)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: \"sink\" field has to have at least one child");
    }


    BOOST_AUTO_TEST_CASE(rule_sink_empty_object)
    {
        const char* json = "{\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: \"sink\" field has to have at least one child");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_not_array)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": 1\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: the content of \"sink\" field is not an array");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_without_id)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"./write.so\",\n"
                           "                    \"func\": \"writer\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: node has to have an id");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_id_empty_string)
    {
        const char* json = "{\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"./write.so\",\n"
                           "                    \"func\": \"writer\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node id should be nonempty string");
    }


    BOOST_AUTO_TEST_CASE(rule_sink_without_task)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node has to have a task field");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_task_not_object)
    {
        const char* json = "{\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            },\n"
                           "            \"task\": \"\"\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node task should be an object");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_task_no_dylib)
    {
        const char* json = "{\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node task should have a dylib field");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_task_dylib_no_func)
    {
        const char* json = "{\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"./write.so\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node task dylib should have location and func");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_task_dylib_location_empty_string)
    {
        const char* json = "{\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"\",\n"
                           "                    \"func\": \"writer\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node task dylib and func should be nonempty string");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_without_input)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"./write.so\",\n"
                           "                    \"func\": \"writer\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node has to have an input field");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_input_not_object)
    {
        const char* json = "{\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": \"\",\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"./write.so\",\n"
                           "                    \"func\": \"writer\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node input should be an object");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_input_no_pre)
    {
        const char* json = "{\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"./write.so\",\n"
                           "                    \"func\": \"writer\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node input should have a pre field");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_input_pre_empty_string)
    {
        const char* json = "{\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"./write.so\",\n"
                           "                    \"func\": \"writer\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node input pre should be nonempty string or string list");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_input_pre_list_empty_string)
    {
        const char* json = "{\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\"node-1\",\"\"]\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"./write.so\",\n"
                           "                    \"func\": \"writer\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: sink node input pre should be nonempty string or string list");
    }

    BOOST_AUTO_TEST_CASE(rule_valid_sink)
    {
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            },\n"
                           "            \"task\": {\n"
                           "                \"dylib\": {\n"
                           "                    \"location\": \"./write.so\",\n"
                           "                    \"func\": \"writer\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_sink(d);

        BOOST_CHECK_EQUAL(res.get_type(), 0);
        BOOST_CHECK_EQUAL(res.get_message(), "Pass: sink field check");
    }

    BOOST_AUTO_TEST_CASE(rule_duplicate_id_source)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"pvp-project-1\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"1\"\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\"\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_duplicate_id(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: detect duplicate node id \"1\"");
    }

    BOOST_AUTO_TEST_CASE(rule_duplicate_id_sink)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"pvp-project-1\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"1\"\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"1\"\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_duplicate_id(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: detect duplicate node id \"1\"");
    }

    BOOST_AUTO_TEST_CASE(rule_duplicate_id_graph)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"pvp-project-1\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"1\"\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\"\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_duplicate_id(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: detect duplicate node id \"1\"");
    }

    BOOST_AUTO_TEST_CASE(rule_no_duplicate_id)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"2\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"3\"\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_duplicate_id(d);

        BOOST_CHECK_EQUAL(res.get_type(), 0);
        BOOST_CHECK_EQUAL(res.get_message(), "Pass: no duplicate node id");
    }


    ////////////////////// predecessor
    BOOST_AUTO_TEST_CASE(rule_valid_predecessor)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_predecessor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 0);
        BOOST_CHECK_EQUAL(res.get_message(), "Pass: predecessors exist");
    }


    BOOST_AUTO_TEST_CASE(rule_graph_predecessor_not_array)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-2\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_predecessor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: predecessor \"source-2\" does not exist");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_predecessor_is_array)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"source-1\",\n"
                           "                    \"source-2\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_predecessor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: predecessor \"source-2\" does not exist");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_predecessor_not_array)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\":\n"
                           "                    \"source-1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"node-3\"\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_predecessor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: predecessor \"node-3\" does not exist");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_predecessor_is_array)
    {
        const char* json = "{\n"
                           "    \"meta\": {\n"
                           "        \"name\": \"minimal-conf-project\",\n"
                           "        \"version\": \"0.0.1\"\n"
                           "    },\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\":\n"
                           "                    \"source-1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"node-3\",\n"
                           "                    \"node-1\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_predecessor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: predecessor \"node-3\" does not exist");
    }


    BOOST_AUTO_TEST_CASE(rule_successor_source_conf_not_object)
    {
        const char* json = "{\n"
                       "    \"source\": [\n"
                       "        {\n"
                       "            \"id\": \"source-1\",\n"
                       "            \"output\": {\n"
                       "                \"conf\": \"\"\n"
                       "            }\n"
                       "        },\n"
                       "        {\n"
                       "            \"id\": \"source-2\",\n"
                       "            \"output\": {\n"
                       "                \"conf\": { \n"
                       "                    \"output_port_num\": 2 \n"
                       "                }\n"
                       "            }\n"
                       "        }\n"
                       "    ],\n"
                       "    \"graph\": [\n"
                       "        {\n"
                       "            \"id\": \"node-1\",\n"
                       "            \"input\": {\n"
                       "                \"pre\": [\n"
                       "                    \"source-1\",\n"
                       "                    \"source-2\"\n"
                       "                ]\n"
                       "            },\n"
                       "            \"output\": {\n"
                       "                \"conf\": { \n"
                       "                    \"output_port_num\": 2 \n"
                       "                }\n"
                       "            }\n"
                       "        }\n"
                       "    ],\n"
                       "    \"sink\": [\n"
                       "        {\n"
                       "            \"id\": \"sink-1\",\n"
                       "            \"input\": {\n"
                       "                \"pre\": [\n"
                       "                    \"node-1\",\n"
                       "                    \"node-2\"\n"
                       "                ]\n"
                       "            }\n"
                       "        }\n"
                       "    ]\n"
                       "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node output conf should be an object");
    }

    BOOST_AUTO_TEST_CASE(rule_successor_source_output_num_not_integer)
    {
        const char* json = "{\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": \"1\"\n"
                           "                }\n"
                           "            }\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"source-2\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"source-1\", \n"
                           "                    \"source-2\"\n"
                           "                ]\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"node-1\",\n"
                           "                    \"node-2\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node output_port_num should be integer");
    }

    BOOST_AUTO_TEST_CASE(rule_successor_source_output_conf_negative)
    {
        const char* json = "{\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": -1 \n"
                           "                }\n"
                           "            }\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"source-2\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"source-1\", \n"
                           "                    \"source-2\"\n"
                           "                ]\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"node-1\",\n"
                           "                    \"node-2\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: source node output_port_num should not be negative");
    }

    BOOST_AUTO_TEST_CASE(rule_successor_graph_output_conf_not_object)
    {
        const char* json = "{\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2\n"
                           "                }\n"
                           "            }\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"source-2\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"source-1\", \n"
                           "                    \"source-2\"\n"
                           "                ]\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"conf\": \"\"\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"node-1\",\n"
                           "                    \"node-2\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node output conf should be an object");
    }

    BOOST_AUTO_TEST_CASE(rule_successor_graph_output_num_not_integer)
    {
        const char* json = "{\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2\n"
                           "                }\n"
                           "            }\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"source-2\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"source-1\", \n"
                           "                    \"source-2\"\n"
                           "                ]\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": \"\"\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"node-1\",\n"
                           "                    \"node-2\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node output_port_num should be integer");
    }

    BOOST_AUTO_TEST_CASE(rule_successor_graph_output_num_negative)
    {
        const char* json = "{\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2\n"
                           "                }\n"
                           "            }\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"source-2\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"source-1\", \n"
                           "                    \"source-2\"\n"
                           "                ]\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": -1\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"node-1\",\n"
                           "                    \"node-2\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: graph node output_port_num should not be negative");
    }

    BOOST_AUTO_TEST_CASE(rule_successor_graph_pre_array_exceed)
    {
        const char* json = "{\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 0\n"
                           "                }\n"
                           "            }\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"source-2\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"source-1\", \n"
                           "                    \"source-2\"\n"
                           "                ]\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"node-1\",\n"
                           "                    \"node-2\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: number of successors of node \"source-1\" exceeds maximum value");
    }

    BOOST_AUTO_TEST_CASE(rule_successor_graph_pre_not_array_exceed)
    {
        const char* json = "{\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 0\n"
                           "                }\n"
                           "            }\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"source-2\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\" \n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"node-1\",\n"
                           "                    \"node-2\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: number of successors of node \"source-1\" exceeds maximum value");
    }

    BOOST_AUTO_TEST_CASE(rule_successor_sink_pre_array_exceed)
    {
        const char* json = "{\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2\n"
                           "                }\n"
                           "            }\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"source-2\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 0\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"node-1\",\n"
                           "                    \"node-2\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: number of successors of node \"node-1\" exceeds maximum value");
    }

    BOOST_AUTO_TEST_CASE(rule_successor_sink_pre_not_array_exceed)
    {
        const char* json = "{\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2\n"
                           "                }\n"
                           "            }\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"source-2\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 2 \n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 0\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \n"
                           "                    \"node-1\"\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 2);
        BOOST_CHECK_EQUAL(res.get_message(), "Error: number of successors of node \"node-1\" exceeds maximum value");
    }

    BOOST_AUTO_TEST_CASE(rule_successor_pass)
    {
        const char* json = "{\n"
                           "    \"source\": [\n"
                           "        {\n"
                           "            \"id\": \"source-1\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 1\n"
                           "                }\n"
                           "            }\n"
                           "        },\n"
                           "        {\n"
                           "            \"id\": \"source-2\",\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 0\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"graph\": [\n"
                           "        {\n"
                           "            \"id\": \"node-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": \"source-1\"\n"
                           "            },\n"
                           "            \"output\": {\n"
                           "                \"conf\": { \n"
                           "                    \"output_port_num\": 1\n"
                           "                }\n"
                           "            }\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"sink-1\",\n"
                           "            \"input\": {\n"
                           "                \"pre\": [\n"
                           "                    \"node-1\"\n"
                           "                ]\n"
                           "            }\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = concrete_rule_successor_check(d);

        BOOST_CHECK_EQUAL(res.get_type(), 0);
        BOOST_CHECK_EQUAL(res.get_message(), "Pass: successors number check");
    }

BOOST_AUTO_TEST_SUITE_END()
