//
// Created by Max on 2018/7/13.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_EXCEPTION_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_EXCEPTION_HPP

#include <pvpf/pvpf.hpp>
#include <stdexcept>

PVPF_NAMESPACE_BEGIN
    namespace utils {
        struct pvpf_exception : public std::logic_error {
            pvpf_exception(const char *msg) : std::logic_error(msg) {}
        };
    }
PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_EXCEPTION_HPP
