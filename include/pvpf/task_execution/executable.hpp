//
// Created by jiabei on 2018/7/6.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_EXECUTABLE_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_EXECUTABLE_HPP

#include <pvpf/pvpf.hpp>
#include <pvpf/utils/data_bucket.hpp>

PVPF_NAMESPACE_BEGIN
    namespace scheduler {
        class executable {
            virtual void exec() = 0;
        };
    }

PVPF_NAMESPACE_END
#endif //POINT_VISION_PIPELINE_FRAMEWORK_EXECUTABLE_HPP

