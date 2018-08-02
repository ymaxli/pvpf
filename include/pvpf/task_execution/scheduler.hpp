//
// Created by jiabei on 2018/7/5.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP

#include "pvpf/pvpf.hpp"
#include "pvpf/task_execution/body.hpp"
#include "scheduler.logical_node.hpp"
#include <rapidjson/document.h>
#include <tbb/flow_graph.h>
#include <unordered_map>
#include <string>
#include <pvpf/utils/data_bucket.hpp>
#include <map>
#include <utility>

PVPF_NAMESPACE_BEGIN
    namespace task_execution {


        struct logical_source_node {
            std::shared_ptr<context> cont;
            std::unique_ptr<tbb::flow::source_node<data_bucket>> source;

            logical_source_node(std::unique_ptr<tbb::flow::source_node<data_bucket>> source,
                                std::shared_ptr<context> cont) : cont(
                    std::move(
                            cont)), source(std::move(source)) {};
        };

        class scheduler {
        public:
            std::map<std::string, rapidjson::Value *> json_object_map;

            void build_graph(rapidjson::Document &conf);

            void run();

        private:


            tbb::flow::graph graph;

            std::unordered_map<std::string, std::unique_ptr<logical_node>> node_map;

            std::unordered_map<std::string, std::unique_ptr<logical_source_node>> source_node_map;

            std::unordered_map<int, std::unique_ptr<data_io::source_io_pipe>> source_pipe_map;

            std::unordered_map<int, std::unique_ptr<data_io::sink_io_pipe>> sink_pipe_map;

            std::vector<std::thread> thread_vector;

            void source_node_list(tbb::flow::graph &graph, rapidjson::Value const &conf);

//            std::unique_ptr<executable> graph_node_list(std::unordered_map<std::string, logical_node> &nodes,
//                                                        tbb::flow::graph &graph, rapidjson::Value const &conf);

            void sink_node_list(std::unordered_map<std::string, logical_node> &nodes,
                                tbb::flow::graph &graph, rapidjson::Value const &conf);

            std::unique_ptr<pvpf::task_execution::logical_source_node>
            generate_source_node(const rapidjson::Value &obj, std::shared_ptr<context> cont);

//            std::unique_ptr<logical_node>
//            generate_graph_node(tbb::flow::graph &graph, rapidjson::Value const &conf,
//                                std::shared_ptr<context> context);
//
            std::unique_ptr<pvpf::task_execution::logical_node>
            generate_sink_node(tbb::flow::graph &graph, rapidjson::Value const &conf,
                               std::shared_ptr<context> context);

            std::unique_ptr<executable> generate_executable(rapidjson::Value const &obj);

            void start_source_functions();

            void stop_io_threads();

            void start_sink_functions();
        };

        void run_source_func(int id, std::unique_ptr<pvpf::data_io::source_io_pipe> pipe);

        void run_sink_func(int id, std::unique_ptr<pvpf::data_io::sink_io_pipe> pipe);
    }

PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP
