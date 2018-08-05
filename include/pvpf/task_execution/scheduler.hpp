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

using namespace tbb::flow;

PVPF_NAMESPACE_BEGIN
    namespace task_execution {

        class scheduler {
        public:
            std::map<std::string, rapidjson::Value *> json_object_map;

            void build_graph(rapidjson::Document &conf);

            void run();

        private:
            tbb::flow::graph graph;

            std::unordered_map<std::string, std::unique_ptr<logical_graph_node>> node_map;

            std::unordered_map<std::string, std::unique_ptr<logical_source_node>> source_node_map;

            std::unordered_map<int, std::unique_ptr<data_io::source_io_pipe>> source_pipe_map;

            std::unordered_map<int, std::unique_ptr<data_io::sink_io_pipe>> sink_pipe_map;

            std::vector<std::thread> thread_vector;

            void source_node_list(rapidjson::Value const &conf);

            void graph_node_map(rapidjson::Value const &conf);

            void sink_node_list(rapidjson::Value const &conf);

            void save_source_node(rapidjson::Value const &obj, std::shared_ptr<context> cont,
                                  std::unique_ptr<data_io::io_pipe_for_source_node> io_pipe);

            void save_source_pipe(rapidjson::Value const &obj, std::unique_ptr<data_io::source_io_pipe> io_pipe);

            void save_graph_node(rapidjson::Value const &conf, std::shared_ptr<context> cont);

            void save_sink_node(rapidjson::Value const &conf, std::shared_ptr<context> cont,
                                std::unique_ptr<data_io::io_pipe_for_sink_node> io_pipe);

            void save_sink_pipe(rapidjson::Value const &obj, std::unique_ptr<data_io::sink_io_pipe> io_pipe);

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
