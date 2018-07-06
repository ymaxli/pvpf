//
// Created by jiabei on 2018/7/5.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_BODY_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_BODY_HPP

#endif //POINT_VISION_PIPELINE_FRAMEWORK_BODY_HPP

#include <pvpf/pvpf.hpp>
using namespace std;

PVPF_NAMESPACE_BEGIN
namespace scheduler{
    struct context{
    public:
        string const node_id;
        string pre[];
        string succ[];
        unordered_map<string, vector< tuple<int, string> >> input;
        unordered_map<string, string> output;
        context(string id, string pre[], string succ[],
                unordered_map<string, vector< tuple<int, string> >> input, unordered_map<string, string> output)
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