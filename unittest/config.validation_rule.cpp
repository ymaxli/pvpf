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
        concrete_rule_format crf;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crf.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: valid JSON file");
    }

    BOOST_AUTO_TEST_CASE(rule_invalid_format)
    {
        concrete_rule_format crf;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [,\n"
                           "    \"graph\": ],\n"
                           "    \"sink\": []\n"
                           "";
        Document d;
        d.Parse(json);
        validation_result res = crf.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: invalid JSON file");
    }

    // concrete_rule_source check
    BOOST_AUTO_TEST_CASE(rule_lack_source)
    {
        concrete_rule_source crs;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crs.validate(d);

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: lack “source” field");
    }

    BOOST_AUTO_TEST_CASE(rule_empty_source)
    {
        concrete_rule_source crs;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crs.validate(d);

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: “source” field has to have at least one child");
    }

    BOOST_AUTO_TEST_CASE(rule_source_not_array)
    {
        concrete_rule_source crs;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": 1,\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crs.validate(d);

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: the content of “source” field is not an array");
    }

    BOOST_AUTO_TEST_CASE(rule_source_without_id)
    {
        concrete_rule_source crs;
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
        validation_result res = crs.validate(d);

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: node has to have an id");
    }

    BOOST_AUTO_TEST_CASE(rule_source_without_task)
    {
        concrete_rule_source crs;
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
        validation_result res = crs.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node has to have a task field");
    }

    BOOST_AUTO_TEST_CASE(rule_source_without_output)
    {
        concrete_rule_source crs;
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
        validation_result res = crs.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: source node has to have an output field");
    }

    BOOST_AUTO_TEST_CASE(rule_valid_source)
    {
        concrete_rule_source crs;
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
        validation_result res = crs.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: source field check");
    }

    ///////////////////////graph



    BOOST_AUTO_TEST_CASE(rule_lack_graph)
    {
        concrete_rule_graph crg;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crg.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: lack “graph” field");
    }

    BOOST_AUTO_TEST_CASE(rule_empty_graph)
    {
        concrete_rule_graph crg;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crg.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: “graph” field has to have at least one child");
    }


    BOOST_AUTO_TEST_CASE(rule_graph_not_array)
    {
        concrete_rule_graph crg;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": 1,\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crg.validate(d);

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: the content of “graph” field is not an array");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_without_id)
    {
        concrete_rule_graph crg;
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
        validation_result res = crg.validate(d);

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: node has to have an id");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_without_task)
    {
        concrete_rule_graph crg;
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
        validation_result res = crg.validate(d);

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: graph node has to have a task field");
    }

    BOOST_AUTO_TEST_CASE(rule_graph_without_input)
    {
        concrete_rule_graph crg;
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
        validation_result res = crg.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: graph node has to have an input field");
    }


    BOOST_AUTO_TEST_CASE(rule_valid_graph)
    {
        concrete_rule_graph crg;
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
        validation_result res = crg.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: graph field check");
    }



    /////////////////////sink


    BOOST_AUTO_TEST_CASE(rule_lack_sink)
    {
        concrete_rule_sink crs;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crs.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: lack “sink” field");
    }

    BOOST_AUTO_TEST_CASE(rule_empty_sink)
    {
        concrete_rule_sink crs;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crs.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: “sink” field has to have at least one child");
    }


    BOOST_AUTO_TEST_CASE(rule_sink_not_array)
    {
        concrete_rule_sink crs;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [],\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": 1\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crs.validate(d);

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: the content of “sink” field is not an array");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_without_id)
    {
        concrete_rule_sink crs;
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
        validation_result res = crs.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: node has to have an id");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_without_task)
    {
        concrete_rule_sink crs;
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
        validation_result res = crs.validate(d);

        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: sink node has to have a task field");
    }

    BOOST_AUTO_TEST_CASE(rule_sink_without_input)
    {
        concrete_rule_sink crs;
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
        validation_result res = crs.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: sink node has to have an input field");
    }

    BOOST_AUTO_TEST_CASE(rule_valid_sink)
    {
        concrete_rule_sink crs;
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
        validation_result res = crs.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: sink field check");
    }

    BOOST_AUTO_TEST_CASE(rule_duplicate_id)
    {
        concrete_rule_duplicate_id crd;
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
                           "            \"id\": \"1\"\n"
                           "        }\n"
                           "    ],\n"
                           "    \"sink\": [\n"
                           "        {\n"
                           "            \"id\": \"1\"\n"
                           "        }\n"
                           "    ]\n"
                           "}";
        Document d;
        d.Parse(json);
        validation_result res = crd.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 2);
        BOOST_CHECK_EQUAL(res.message, "Error: detect duplicate node id");
    }

    BOOST_AUTO_TEST_CASE(rule_no_duplicate_id)
    {
        concrete_rule_duplicate_id crd;
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
        validation_result res = crd.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.type, 0);
        BOOST_CHECK_EQUAL(res.message, "Pass: no duplicate node id");
    }

BOOST_AUTO_TEST_SUITE_END()
