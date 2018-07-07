//
// Created by jiabei on 2018/7/5.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP

#include "pvpf/pvpf.hpp"
#include <rapidjson/document.h>
#include <tbb/flow_graph.h>

PVPF_NAMESPACE_BEGIN
namespace scheduler{
    class scheduler{
    public:
        void build_graph(rapidjson::Document& conf);
        void run();
    private:
        tbb::flow::graph graph;
    };


}

PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_SCHEDULER_HPP
