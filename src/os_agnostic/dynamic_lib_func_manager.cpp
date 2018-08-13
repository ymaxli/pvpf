//
// Created by Max on 2018/7/13.
//

#include <pvpf/pvpf.hpp>
#include <pvpf/os_agnostic/dynamic_lib_func_manager.hpp>
#include <dlfcn.h>
#include <pvpf/utils/exception.hpp>
#include <iostream>

using namespace std;
using namespace pvpf;

PVPF_NAMESPACE_BEGIN
namespace os_agnostic
{

dylib_func_ptr
dynamic_lib_func_manager::load_algorithm(path const &lib_location, std::string const &func_name)
{
    if (algorithm_map.count(lib_location.string()))
    { // a library been opened before
        auto &library = algorithm_map[lib_location.string()];

        // find cached function pointers
        auto &func_ptrs = library.func_ptrs;
        if (func_ptrs.count(func_name))
        { // found
            return func_ptrs[func_name];
        }
        else
        { // not found
#ifdef _WIN32

#else
            auto ptr = load_linux_dylib_func_ptr(library.handle, func_name);
#endif

            func_ptrs[func_name] = ptr;
            return ptr;
        }
    }
    else
    { // new library
#ifdef _WIN32

#else
        void *handle = open_linux_dylib_handle(lib_location.string());
        auto ptr = load_linux_dylib_func_ptr(handle, func_name);
#endif

        dynamic_library library;
        library.handle = handle;
        library.func_ptrs[func_name] = ptr;
        algorithm_map[lib_location.string()] = library;

        return ptr;
    }
}

int dynamic_lib_func_manager::load_input_func(path const &lib_location, std::string const &func_name)
{
    if (io_map.count(lib_location.string()))
    { // a library been opened before
        auto &library = io_map[lib_location.string()];

        // find cached function pointers
        auto &func_ptrs = library.input_func_ptrs;
        if (func_ptrs.count(func_name))
        { // found
            return func_ptrs[func_name];
        }
        else
        { // not found
#ifdef _WIN32

#else
            auto ptr = load_linux_input_func_ptr(library.handle, func_name);
#endif

            input_func_vector.push_back(ptr);
            auto id = static_cast<int>(input_func_vector.size() - 1);
            func_ptrs[func_name] = id;
            return id;
        }
    }
    else
    { // new library
#ifdef _WIN32

#else
        void *handle = open_linux_dylib_handle(lib_location.string());
        auto ptr = load_linux_input_func_ptr(handle, func_name);
#endif

        io_library library;
        library.handle = handle;
        input_func_vector.push_back(ptr);
        auto id = static_cast<int>(input_func_vector.size() - 1);
        library.input_func_ptrs[func_name] = id;
        io_map[lib_location.string()] = library;
        return id;
    }
}

int dynamic_lib_func_manager::load_output_func(path const &lib_location, std::string const &func_name)
{
    if (io_map.count(lib_location.string()))
    { // a library been opened before
        auto &library = io_map[lib_location.string()];

        // find cached function pointers
        auto &func_ptrs = library.output_func_ptrs;
        if (func_ptrs.count(func_name))
        { // found
            return func_ptrs[func_name];
        }
        else
        { // not found
#ifdef _WIN32

#else
            auto ptr = load_linux_output_func_ptr(library.handle, func_name);
#endif
            output_func_vector.push_back(ptr);
            auto id = static_cast<int>(output_func_vector.size() - 1);
            func_ptrs[func_name] = id;
            return id;
        }
    }
    else
    { // new library
#ifdef _WIN32

#else
        void *handle = open_linux_dylib_handle(lib_location.string());
        auto ptr = load_linux_output_func_ptr(handle, func_name);
#endif

        io_library library;
        library.handle = handle;
        output_func_vector.push_back(ptr);
        auto id = static_cast<int>(output_func_vector.size() - 1);
        library.output_func_ptrs[func_name] = id;
        io_map[lib_location.string()] = library;
        return id;
    }
}

dylib_func_ptr dynamic_lib_func_manager::load_linux_dylib_func_ptr(void *handle,
                                                                   std::string const &func_name)
{
    char *err = nullptr;

    //clear error info
    dlerror();

    auto pf = (dylib_func_ptr)dlsym(handle, func_name.c_str());

    err = dlerror();

    if (err)
    {
        throw utils::pvpf_exception(
            (string("could not find function: ") + func_name).c_str());
    }
    return pf;
}

input_func_ptr dynamic_lib_func_manager::load_linux_input_func_ptr(void *handle, std::string const &func_name)
{
    char *err = nullptr;

    //clear error info
    dlerror();

    auto pf = (input_func_ptr)dlsym(handle, func_name.c_str());

    err = dlerror();

    if (err)
    {
        throw utils::pvpf_exception(
            (string("could not find function: ") + func_name).c_str());
    }
    return pf;
}

output_func_ptr
dynamic_lib_func_manager::load_linux_output_func_ptr(void *handle, std::string const &func_name)
{
    char *err = nullptr;

    //clear error info
    dlerror();

    auto pf = (output_func_ptr)dlsym(handle, func_name.c_str());

    err = dlerror();

    if (err)
    {
        throw utils::pvpf_exception(
            (string("could not find function: ") + func_name).c_str());
    }
    return pf;
}

void *dynamic_lib_func_manager::open_linux_dylib_handle(std::string const &lib_location)
{
    void *handle = dlopen(lib_location.c_str(), RTLD_NOW);

    char *err = dlerror();
    if (!handle || err)
    {
        throw utils::pvpf_exception((string("could not open library: ") + lib_location + (err ? string(err) : "")).c_str());
    }

    return handle;
}

void dynamic_lib_func_manager::release_all()
{
    for (auto it : algorithm_map)
    {
        dlclose(it.second.handle);
    }

    for (auto it : io_map)
    {
        dlclose(it.second.handle);
    }
}

dylib_func_ptr
dynamic_lib_func_manager::load_windows_dylib_func_ptr(void *handle, std::string const &func_name)
{
    return nullptr;
}

output_func_ptr
dynamic_lib_func_manager::load_windows_output_func_ptr(void *handle, std::string const &func_name)
{
    return nullptr;
}

input_func_ptr
dynamic_lib_func_manager::load_windows_input_func_ptr(void *handle, std::string const &func_name)
{
    return nullptr;
}

void dynamic_lib_func_manager::invoke_io_func(int io_func_id, data_io::source_io_pipe &pipe)
{
    if (input_func_vector.size() <= io_func_id)
    {
        throw utils::pvpf_exception(
            (string("input library id: ") + to_string(io_func_id) + string(" not found")).c_str());
    }

    auto ptr = input_func_vector[io_func_id];

    ptr(pipe);
}

void dynamic_lib_func_manager::invoke_io_func(int io_func_id, data_io::sink_io_pipe &pipe)
{
    if (output_func_vector.size() <= io_func_id)
    {
        throw utils::pvpf_exception(
            (string("output library id: ") + to_string(io_func_id) + string(" not found")).c_str());
    }

    auto ptr = output_func_vector[io_func_id];

    ptr(pipe);
}
} // namespace os_agnostic
PVPF_NAMESPACE_END
