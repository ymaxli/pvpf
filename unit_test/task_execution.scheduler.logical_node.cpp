//
// Created by Max on 2018/7/22.
//

#ifdef STAND_ALONE
#define BOOST_TEST_MODULE TaskExecutionSchedulerLogicalNode
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/task_execution/scheduler.hpp>
#include <pvpf/task_execution/scheduler.logical_node.hpp>
#include <rapidjson/document.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <tbb/flow_graph.h>

using namespace std;
using namespace pvpf;
using namespace rapidjson;
using namespace pvpf::task_execution;
using namespace tbb::flow;

BOOST_AUTO_TEST_SUITE(task_execution_scheduler_logical_node_suite)

// BOOST_AUTO_TEST_CASE(normal)
// {
//     graph g;
//     auto cont = std::make_shared<context>("id", vector<string>(), vector<string>(),
//                                           unordered_map<string, vector<pair<int, string>>>(),
//                                           unordered_map<string, string>(),
//                                           unordered_map<string, string>(), vector<bool>(),
//                                           vector<bool>(), false);
//     auto join = std::make_unique<join_node<array<data_bucket, 1>>>(g);
//     auto func = std::make_unique<function_node<array<data_bucket, 1>, data_bucket>>(g, unlimited,
//                                                                                     node_body<1>(
//                                                                                         cont,
//                                                                                         make_shared<normal_algorithm>(),
//                                                                                         make_shared<data_bucket>()));

//     logical_graph_node node = {};
//     node.wrap.size_1 = {std::move(cont), std::move(func), std::move(join)};

//     BOOST_TEST(node.wrap.size_1.cont->node_id == "id");

//     auto func2 = std::make_unique<function_node<array<data_bucket, 2>, data_bucket>>(g, unlimited,
//                                                                                      node_body<2>(
//                                                                                          cont,
//                                                                                          make_shared<normal_algorithm>(),
//                                                                                          make_shared<data_bucket>()));
//     auto join2 = std::make_unique<join_node<array<data_bucket, 2>>>(g);
//     auto cont2 = std::make_shared<context>("id222", vector<string>(), vector<string>(),
//                                            unordered_map<string, vector<pair<int, string>>>(),
//                                            unordered_map<string, string>(),
//                                            unordered_map<string, string>(), vector<bool>(),
//                                            vector<bool>(), false);

//     node.wrap.size_2 = {std::move(cont2), std::move(func2), std::move(join2)};

//     BOOST_TEST(node.wrap.size_2.cont->node_id == "id222");
// }

BOOST_AUTO_TEST_SUITE_END()