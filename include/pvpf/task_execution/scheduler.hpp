//
// Created by jiabei on 2018/7/5.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP

#include "pvpf/pvpf.hpp"
#include "body.hpp"
#include <rapidjson/document.h>
#include <tbb/flow_graph.h>
#include <unordered_map>
#include <string>

PVPF_NAMESPACE_BEGIN
    namespace scheduler {
        class scheduler {
        public:
            void build_graph(rapidjson::Document &conf);

            void run();

        private:
            tbb::flow::graph graph;

            void source_node_list(std::unordered_map<std::string, tbb::flow::graph_node> &nodes,
                                  tbb::flow::graph &graph, const rapidjson::Value &conf);

            void graph_node_list(std::unordered_map<std::string, tbb::flow::graph_node> &nodes,
                                 tbb::flow::graph &graph, const rapidjson::Value &conf);

            void sink_node_list(std::unordered_map<std::string, tbb::flow::graph_node> &nodes,
                                tbb::flow::graph &graph, const rapidjson::Value &conf);

            std::unique_ptr<tbb::flow::source_node>
            generate_source_node(tbb::flow::graph &graph, const rapidjson::Value &conf, context &context);

            std::unique_ptr<tbb::flow::function_node>
            generate_graph_node(tbb::flow::graph &graph, const rapidjson::Value &conf, context &context);

            std::unique_ptr<tbb::flow::function_node>
            generate_sink_node(tbb::flow::graph &graph, const rapidjson::Value &conf, context &context);

            std::shared_ptr<context>
            create_context(const rapidjson::Value &obj, rapidjson::Document &conf);
        };


    }

PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP
