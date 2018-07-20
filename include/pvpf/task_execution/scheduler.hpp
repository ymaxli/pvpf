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
#include <map>


PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        struct logical_node {
            int join_size = 0;
            int join_ind = 0;
            int split_size = 0;
            int split_ind = 0;
            std::shared_ptr<context> cont;

            std::unique_ptr<tbb::flow::function_node<data_bucket>> node;
            std::unique_ptr<tbb::flow::join_node<std::array<data_bucket, 1>>> join;

            logical_node(tbb::flow::function_node<data_bucket> *node, std::shared_ptr<context> cont) : cont(cont),
                                                                                                       node(node),
                                                                                                       join() {};
        };

        struct logical_source_node {
            std::shared_ptr<context> cont;
            std::unique_ptr<tbb::flow::source_node<data_bucket>> source;

            logical_source_node(tbb::flow::source_node<data_bucket> *source, std::shared_ptr<context> cont) : cont(
                    cont), source(source){};
        };

        class scheduler {
        public:
            void build_graph(rapidjson::Document &conf);

            void run();

        private:
            std::map<std::string, rapidjson::Value *> json_object_map;

            tbb::flow::graph graph;

            std::unordered_map<std::string, std::unique_ptr<logical_node>> node_map;

            std::unordered_map<std::string, std::unique_ptr<logical_source_node>> source_node_map;

            std::unordered_map<int, std::unique_ptr<data_io::source_io_pipe>> source_pipe_map;

            std::vector<std::thread> thread_vector;

            void source_node_list(tbb::flow::graph &graph, rapidjson::Value const &conf);

            std::unique_ptr<executable> graph_node_list(std::unordered_map<std::string, logical_node> &nodes,
                                                        tbb::flow::graph &graph, rapidjson::Value const &conf);

            void sink_node_list(std::unordered_map<std::string, logical_node> &nodes,
                                tbb::flow::graph &graph, rapidjson::Value const &conf);

            std::unique_ptr<pvpf::task_execution::logical_source_node>
            generate_source_node(const rapidjson::Value &obj, std::shared_ptr<context> cont);

            std::unique_ptr<logical_node>
            generate_graph_node(tbb::flow::graph &graph, rapidjson::Value const &conf,
                                std::shared_ptr<context> context);

            std::unique_ptr<logical_node>
            generate_sink_node(tbb::flow::graph &graph, rapidjson::Value const &conf,
                               std::shared_ptr<context> context);

            std::shared_ptr<context>
            create_context(rapidjson::Value const &obj);

            std::vector<std::pair<int, std::string>> analyze_mapping_value(std::string value, int size);

            std::unique_ptr<tbb::flow::join_node<std::array<data_bucket, 1>>>
            create_join_node(tbb::flow::graph &graph, int size);

            bool is_cpu(std::string algorithm_name);

            void figure_out_json_object(rapidjson::Document &conf);

            std::unique_ptr<executable> generate_executable(rapidjson::Value const &obj);

            void start_source_functions();
        };
    }

PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP
