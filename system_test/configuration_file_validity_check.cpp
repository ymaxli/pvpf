//
// Created by Jiabei He on 28/06/2018.
//

#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE ConfigReaderTests
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/config/config_reader.h>
#include <rapidjson/document.h>
#include <pvpf/config/config_validator.h>

using namespace std;
using namespace rapidjson;
using namespace pvpf::config;

BOOST_AUTO_TEST_SUITE(fr01_configuration_file_validity_check_test)
    BOOST_AUTO_TEST_CASE(invalid_json_format)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/invalid_json.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "invalid JSON file");
    }

    BOOST_AUTO_TEST_CASE(lack_source_field)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/omit_source.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "lack \"source\" field");
    }

    BOOST_AUTO_TEST_CASE(empty_source_field)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/empty_source.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "\"source\" field has to have at least one child");
    }

    BOOST_AUTO_TEST_CASE(omit_graph_field)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/omit_graph.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "lack \"graph\" field");
    }

    BOOST_AUTO_TEST_CASE(empty_graph_field)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/empty_graph.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "\"graph\" field has to have at least one child");
    }

    BOOST_AUTO_TEST_CASE(omit_sink_field)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/omit_sink.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "lack \"sink\" field");
    }

    BOOST_AUTO_TEST_CASE(empty_sink_field)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/empty_sink.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "\"sink\" field has to have at least one child");
    }

    BOOST_AUTO_TEST_CASE(lack_node_id)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/lack_node_id.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "node has to have an id");
    }

    BOOST_AUTO_TEST_CASE(duplicate_node_id)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/duplicate_node_id.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "detect duplicate node id");
    }

    BOOST_AUTO_TEST_CASE(source_missing_task)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/source_missing_task_field.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "source node has to have a task field");
    }

    BOOST_AUTO_TEST_CASE(sink_missing_task)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/sink_missing_task.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "sink node has to have a task field");
    }

    BOOST_AUTO_TEST_CASE(source_missing_output)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/source_missing_output.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "source node has to have an output field");
    }

    BOOST_AUTO_TEST_CASE(sink_missing_input)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/sink_missing_input.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "sink node has to have an input field");
    }

    BOOST_AUTO_TEST_CASE(graph_missing_input)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/graph_missing_input.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "graph node has to have an input field");
    }

    BOOST_AUTO_TEST_CASE(graph_missing_task)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/graph_missing_task.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "graph node has to have a task field");
    }

    BOOST_AUTO_TEST_CASE(graph_missing_algorithm)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/graph_missing_algorithm.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "algorithm not found");
    }

    BOOST_AUTO_TEST_CASE(predecessor_not_found)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/predecessor_not_found.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "predecessor does not exist");
    }
    BOOST_AUTO_TEST_CASE(number_of_successor_exceed)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/number_of_successor_exceed.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "number of successors of one node exceeds maximum value");
    }
    BOOST_AUTO_TEST_CASE(mapping_error)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/mapping_error.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "mapping error");
    }
    BOOST_AUTO_TEST_CASE(unfit_node)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/unfit_node.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "two nodes are unfit");
    }
    BOOST_AUTO_TEST_CASE(invalid_data_type)
    {
        config_reader cr;
        string jsonfile = "../../system_test/test_json/framework_configuration/invalid_data_type.json";
        Document d = cr.load_json_conf(jsonfile);
        config_validator cv;
        vector<validation_result const> res = cv.validate(d);
        BOOST_CHECK_EQUAL(res[0].type, 2);
        BOOST_CHECK_EQUAL(res[0].message, "invalid data type");
    }
BOOST_AUTO_TEST_SUITE_END()