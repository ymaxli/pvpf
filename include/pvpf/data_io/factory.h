//
// Created by Max on 2018/6/24.
//

#ifndef DEV_ENV_FACTORY_H
#define DEV_ENV_FACTORY_H
#pragma once

#include "pvpf/pvpf.h"
#include "pvpf/utils/data_bucket.h"
#include "io_pipe.h"
#include "buffer.h"

using namespace std;

PVPF_NAMESPACE_BEGIN

    namespace data_io {
        tuple<unique_ptr<source_io_pipe>, unique_ptr<io_pipe_for_source_node>> create_source(int buffer_size, bool buffer_blocking) {
            auto buffer = make_shared<buffer>(buffer_size, buffer_blocking);
            auto sip = make_unique<source_io_pipe>(buffer);
            auto ipsn = make_unique<io_pipe_for_source_node>(buffer);
            return make_tuple(sip, ipsn);
        };
        tuple<unique_ptr<sink_io_pipe>, unique_ptr<io_pipe_for_sink_node>> create_sink(int buffer_size, bool buffer_blocking) {
            auto buffer = make_shared<buffer>(buffer_size, buffer_blocking);
            auto sip = make_unique<sink_io_pipe>(buffer);
            auto ipsn = make_unique<io_pipe_for_sink_node>(buffer);
            return make_tuple(sip, ipsn);
        };
    }

PVPF_NAMESPACE_END

#endif //DEV_ENV_FACTORY_H
