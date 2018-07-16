//
// Created by jiabei on 2018/7/5.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_BODY_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_BODY_HPP

#include <vector>
#include <unordered_map>
#include <pvpf/pvpf.hpp>
#include <string>
#include <pvpf/data_io/io_pipe.hpp>
#include <pvpf/task_execution/executable.hpp>

PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        struct context {
        public:
            std::string const node_id;
            std::vector<std::string> pre;
            std::vector<std::string> succ;
            std::unordered_map<std::string, std::vector<std::pair<int, std::string> >> input;
            std::unordered_map<std::string, std::string> output;
            std::unordered_map<std::string, std::string> data;

            std::vector<bool> read_only;
            std::vector<bool> pre_is_cpu;
            bool is_cpu;

            context(std::string id, std::vector<std::string> pre, std::vector<std::string> succ,
                    std::unordered_map<std::string, std::vector<std::pair<int, std::string> >> input,
                    std::unordered_map<std::string, std::string> output,
                    std::unordered_map<std::string, std::string> data, std::vector<bool> read_only,
                    std::vector<bool> pre_is_cpu, bool is_cpu)
                    : node_id(id), pre(std::move(pre)), succ(std::move(succ)), input(std::move(input)), output(std::move(output)),
                      data(std::move(data)), read_only(std::move(read_only)), pre_is_cpu(std::move(pre_is_cpu)),
                      is_cpu(is_cpu) {
            }
        };

        struct io_body {
            std::shared_ptr<context> cont;
            data_io::io_pipe_for_source_node pipe;

            io_body(std::shared_ptr<context> context, data_io::io_pipe_for_source_node pipe) : cont(context),
                                                                                               pipe(std::move(pipe)) {};

            void operator()();
        };

        struct body {
        public:
            std::shared_ptr<context> cont;
            std::unique_ptr<executable> exec;

            body(std::shared_ptr<context> context, std::unique_ptr<executable> exec) : cont(context),
                                                                                       exec(std::move(exec)) {};

            body(const body &b) : exec(b.exec.get()) {
                this->cont = b.cont;
            };

            void operator()(const data_bucket &db) {
                return;
            };
        };

    }

PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_BODY_HPP
