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
#include <executable.hpp>

using namespace std;

PVPF_NAMESPACE_BEGIN
    namespace scheduler{
        struct context{
        public:
            string const node_id;
            string pre[];
            string succ[];
            unordered_map<string, vector< pair<int, string> >> input;
            unordered_map<string, string> output;
            context(string id, string pre[], string succ[],
                    unordered_map<string, vector< pair<int, string> >> input, unordered_map<string, string> output)
                    : node_id(id), pre(pre), succ(succ), input(input), output(output){}
        };
        struct body{
        public:
            context context;
            executable exec;
            data_io::io_pipe_for_source_node pipe;
            body(context context, executable exec):context(context), exec(exec){};
            body(context context, data_io::io_pipe_for_source_node pipe):context(context), pipe(pipe){};
            void operator();
        };
    }

PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_BODY_HPP
