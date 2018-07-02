//
// Created by Max on 2018/6/23.
//

#ifndef DEV_ENV_BUFFER_H
#define DEV_ENV_BUFFER_H
#pragma once

#include "pvpf/pvpf.h"
#include <mutex>
#include <thread>
#include <chrono>
#include "pvpf/utils/data_bucket.h"

PVPF_NAMESPACE_BEGIN

    namespace data_io {
        class buffer {
        public:
            buffer(int buffer_size, bool blocking) : buffer_size(buffer_size), blocking(blocking) {
                write_index = 0;
                read_index = 0;
                write_complete = false;
                buffer_array = new data_bucket[buffer_size];
                buffer_array_read_available = new bool[buffer_size];
                for (int i = 0; i < buffer_size; i++) buffer_array_read_available[i] = false;
            }

            ~buffer() {
                delete[] buffer_array;
                delete[] buffer_array_read_available;
            }

            void write(data_bucket data) {
                if(write_complete) return;

                if (blocking) {
                    while (buffer_array_read_available[write_index]) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(BLOCK_INTERVAL));
                    }
                }

                if (write_index == read_index) {
                    mutex.try_lock();
                    write_to_current_position(std::move(data));
                    mutex.unlock();
                } else {
                    write_to_current_position(std::move(data));
                }

            }

            data_bucket read() {
                while (!buffer_array_read_available[read_index]) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(BLOCK_INTERVAL));
                }

                data_bucket result;

                if (write_index == read_index) {
                    mutex.try_lock();
                    result = read_from_current_position();
                    mutex.unlock();
                } else {
                    result = read_from_current_position();
                }

                return result;
            }

            bool is_empty() {
                return write_complete && !buffer_array_read_available[read_index];
            }


            void stop_writing() {
                write_complete = true;
            }

        private:
            const int BLOCK_INTERVAL = 100;
            int buffer_size;
            bool blocking;
            int write_index, read_index;
            std::mutex mutex;
            data_bucket *buffer_array; // use a dynamically-allocated array to contain buffer
            bool *buffer_array_read_available; // flag for buffer read availability
            bool write_complete;
            void write_to_current_position(data_bucket data) {
                buffer_array[write_index] = std::move(data);
                buffer_array_read_available[write_index] = true;
                if (!blocking) read_index = write_index;
                write_index = (write_index + 1) % buffer_size;
            }

            data_bucket read_from_current_position() {
                buffer_array_read_available[read_index] = false;
                data_bucket result = std::move(buffer_array[read_index]);
                read_index = (read_index + 1) % buffer_size;

                return result;
            }
        };
    }

PVPF_NAMESPACE_END

#endif //DEV_ENV_BUFFER_H

