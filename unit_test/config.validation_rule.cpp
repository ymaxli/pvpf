//
// Created by Icay on 23/06/2018.
//


#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE ConfigValidatorTests
#endif


#include <boost/test/unit_test.hpp>
#include <pvpf/config/validation_rule.h>
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

        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: valid JSON file");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: invalid JSON file");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: lack \"source\" field");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: \"source\" field has to have at least one child");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: \"source\" field has to have at least one child");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: the content of \"source\" field is not an array");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: node has to have an id");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node id should be nonempty string");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node id should be nonempty string");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node has to have a task field");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node task should be an object");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node task should have a dylib field");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node task dylib should have location and func");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node task dylib and func should be nonempty string");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node has to have an output field");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node output should be an object");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node output should have a data field");
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

        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: source field check");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: lack \"graph\" field");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: \"graph\" field has to have at least one child");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: the content of \"graph\" field is not an array");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: node has to have an id");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: graph node has to have a task field");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: graph node has to have an input field");
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

        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: graph field check");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: lack \"sink\" field");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: \"sink\" field has to have at least one child");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: the content of \"sink\" field is not an array");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: node has to have an id");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: sink node has to have a task field");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: sink node has to have an input field");
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

        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: sink field check");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: detect duplicate node id \"1\"");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: detect duplicate node id \"1\"");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: detect duplicate node id \"1\"");
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

        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: no duplicate node id");
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

        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: predecessors exist");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: predecessor \"source-2\" does not exist");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: predecessor \"source-2\" does not exist");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: predecessor \"node-3\" does not exist");
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

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: predecessor \"node-3\" does not exist");
    }

BOOST_AUTO_TEST_SUITE_END()
