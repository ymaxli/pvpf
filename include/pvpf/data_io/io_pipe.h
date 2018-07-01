//
// Created by Max on 2018/6/24.
//

#ifndef DEV_ENV_IO_PIPE_H
#define DEV_ENV_IO_PIPE_H
#pragma once

#include "pvpf/pvpf.h"
#include "pvpf/utils/data_bucket.h"
#include "buffer.h"

using namespace std;

PVPF_NAMESPACE_BEGIN

    namespace data_io {
        class source_io_pipe {
        public:
            source_io_pipe(shared_ptr<buffer> b) : buffer(b) {}

            template<typename T>
            void source_write(const string &key, T &obj) {
                data_bucket bucket;
                bucket.put(key, obj);

                source_write(bucket);
            }

            void source_write(data_bucket data) {
                buffer->write(data);
            }

            void source_complete() {
                buffer->stop_writing();
            }

        private:
            shared_ptr<buffer> buffer;
        };

        class io_pipe_for_source_node {
        public:
            io_pipe_for_source_node(shared_ptr<buffer> b) : buffer(b) {}

            bool is_empty() {
                return buffer->is_empty();
            }

            data_bucket read() {
                return buffer->read();
            }

        private:
            shared_ptr<buffer> buffer;
        };

        class sink_io_pipe {
        public:
            sink_io_pipe(shared_ptr<buffer> b) : buffer(b) {}

            template<typename T>
            T sink_read(const string &key) {
                data_bucket data = buffer->read();
                return boost::any_cast<T>(data.get(key));
            }

            data_bucket sink_read() {
                return buffer->read();
            }

            bool sink_is_complete() {
                return buffer->is_empty();
            }

        private:
            shared_ptr<buffer> buffer;
        };

        class io_pipe_for_sink_node {
        public:
            io_pipe_for_sink_node(shared_ptr<buffer> b) : buffer(b) {}

            void write(data_bucket data) {
                buffer->write(data);
            }

            void stop_writing() {
                buffer->stop_writing();
            }

        private:
            shared_ptr<buffer> buffer;
        };
    }

PVPF_NAMESPACE_END

#endif //DEV_ENV_IO_PIPE_H
