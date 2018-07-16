//
// Created by Max on 2018/7/13.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_DYNAMIC_LIB_FUNC_MANAGER_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_DYNAMIC_LIB_FUNC_MANAGER_HPP

#include <pvpf/pvpf.hpp>
#include <pvpf/utils/data_bucket.hpp>
#include <boost/filesystem/path.hpp>
#include <unordered_map>
#include <pvpf/data_io/io_pipe.hpp>

using namespace boost::filesystem;

PVPF_NAMESPACE_BEGIN
    namespace os_agnostic {

        typedef void(*dylib_func_ptr)(data_bucket &, data_bucket const &);

        typedef void(*input_func_ptr)(data_io::source_io_pipe &);

        typedef void(*output_func_ptr)(data_io::sink_io_pipe &);

        // singleton
        class dynamic_lib_func_manager {
        public:

            ~dynamic_lib_func_manager() {
                release_all();
            }

            dynamic_lib_func_manager(dynamic_lib_func_manager const &) = delete;

            dynamic_lib_func_manager &operator=(dynamic_lib_func_manager const &) = delete;

            static dynamic_lib_func_manager &get_instance() {
                static dynamic_lib_func_manager instance;

                return instance;
            }

            dylib_func_ptr load_algorithm(path const &lib_location, std::string const &func_name);

            int load_input_func(path const &lib_location, std::string const &func_name);

            int load_output_func(path const &lib_location, std::string const &func_name);

            void invoke_io_func(int io_func_id, data_io::source_io_pipe &pipe);

            void invoke_io_func(int io_func_id, data_io::sink_io_pipe &pipe);

        private:
            struct dynamic_library {
                void *handle;
                std::unordered_map<std::string, dylib_func_ptr> func_ptrs;
            };
            struct io_library {
                void *handle;
                std::unordered_map<std::string, int> input_func_ptrs;
                std::unordered_map<std::string, int> output_func_ptrs;
            };

            std::unordered_map<std::string, dynamic_library> algorithm_map;
            std::unordered_map<std::string, io_library> io_map;
            std::vector<input_func_ptr> input_func_vector;
            std::vector<output_func_ptr> output_func_vector;

            dynamic_lib_func_manager() = default;

            dylib_func_ptr load_linux_dylib_func_ptr(void *handle, std::string const &func_name);

            input_func_ptr load_linux_input_func_ptr(void *handle, std::string const &func_name);

            output_func_ptr load_linux_output_func_ptr(void *handle, std::string const &func_name);

            void *open_linux_dylib_handle(std::string const &lib_location);

            dylib_func_ptr load_windows_dylib_func_ptr(void *handle, std::string const &func_name);

            input_func_ptr load_windows_input_func_ptr(void *handle, std::string const &func_name);

            output_func_ptr load_windows_output_func_ptr(void *handle, std::string const &func_name);

            void release_all();
        };
    }
PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_DYNAMIC_LIB_FUNC_MANAGER_HPP
