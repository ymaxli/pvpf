//
// Created by jiabei on 2018/7/5.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP

#include "pvpf/pvpf.hpp"
#include "pvpf/task_execution/body.hpp"
#include <rapidjson/document.h>
#include <tbb/flow_graph.h>
#include <unordered_map>
#include <string>
#include <pvpf/utils/data_bucket.hpp>


PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        struct logical_node {
            std::shared_ptr<context> cont;
            tbb::flow::graph_node *node;
            tbb::flow::split_node<data_bucket> *split;
            tbb::flow::join_node<data_bucket> *join;
        };

        class scheduler {
        public:
            void build_graph(rapidjson::Document &conf);

            void run();

        private:
            tbb::flow::graph graph;

            void source_node_list(std::unordered_map<std::string, tbb::flow::graph_node> &nodes,
                                  tbb::flow::graph &graph, rapidjson::Value const &conf);

            void graph_node_list(std::unordered_map<std::string, tbb::flow::graph_node> &nodes,
                                 tbb::flow::graph &graph, rapidjson::Value const &conf);

            void sink_node_list(std::unordered_map<std::string, tbb::flow::graph_node> &nodes,
                                tbb::flow::graph &graph, rapidjson::Value const &conf);

            std::unique_ptr<tbb::flow::source_node<data_bucket>>
            generate_source_node(tbb::flow::graph &graph, rapidjson::Value const &conf, context &context);

            std::unique_ptr<tbb::flow::function_node<data_bucket>>
            generate_graph_node(tbb::flow::graph &graph, rapidjson::Value const &conf, context &context);

            std::unique_ptr<tbb::flow::function_node<data_bucket>>
            generate_sink_node(tbb::flow::graph &graph, rapidjson::Value const &conf, context &context);

            std::shared_ptr<context>
            create_context(rapidjson::Value const &obj, rapidjson::Document &conf);

            std::vector<std::pair<int, std::string>> analyze_mapping_value(std::string value, int size);
        };


    }

PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP
