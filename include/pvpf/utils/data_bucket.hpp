//
// Created by Jiabei He on 23/06/2018.
//

#ifndef DEV_ENV_DATA_BUCKET_H
#define DEV_ENV_DATA_BUCKET_H
#pragma once

#ifdef DEV_MODE
#include <iostream>
#endif

#include <core/any.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include "pvpf/pvpf.hpp"
#include <pvpf/utils/exception.hpp>

PVPF_NAMESPACE_BEGIN
namespace task_execution
{
class loop_algorithm;

template <size_t input_size>
struct node_body;

template <size_t input_size>
struct sink_body;
} // namespace task_execution

class data_bucket
{
    friend class task_execution::loop_algorithm;

    template <size_t input_size>
    friend struct task_execution::node_body;

    template <size_t input_size>
    friend struct task_execution::sink_body;

  public:
    data_bucket()
    {
        map = new std::unordered_map<std::string, std::shared_ptr<core::any>>();
    }

    ~data_bucket()
    {
        delete map;
    }

    explicit data_bucket(std::unordered_map<std::string, std::shared_ptr<core::any>> *map) : map(map) {}

    data_bucket(data_bucket const &new_data_bucket) noexcept
    {
        map = new std::unordered_map<std::string, std::shared_ptr<core::any>>(*new_data_bucket.map);
    }

    data_bucket(data_bucket &&new_data_bucket) noexcept : map(new_data_bucket.map)
    {
        new_data_bucket.map = nullptr;
    }

    data_bucket &operator=(data_bucket &&new_data_bucket) noexcept
    {
        if (this != &new_data_bucket)
        {
            map = new_data_bucket.map;
            new_data_bucket.map = nullptr;
        }

        return *this;
    }

    data_bucket &operator=(data_bucket const &new_data_bucket) noexcept
    {
        if (this != &new_data_bucket)
        {
            map = new std::unordered_map<std::string, std::shared_ptr<core::any>>(*new_data_bucket.map);
        }

        return *this;
    }

    data_bucket clone() noexcept
    {
        auto new_map = new std::unordered_map<std::string, std::shared_ptr<core::any>>();
        for (auto it = begin(); it != end(); it++)
        {
            (*new_map)[it->first] = std::make_shared<core::any>(*(it->second));
        }
        data_bucket result(new_map);
        return result;
    }

    template <typename T>
    void put(std::string const &key, T &&value)
    {
        (*map)[key] = std::make_shared<core::any>(std::forward<T>(value));
    }

    template <typename T>
    void put_collection_by_move(std::string const &key, std::vector<T> &value)
    {
        std::vector<core::any> any_vector;

        for (int i = 0; i < value.size(); i++)
        {
            any_vector.push_back(std::move(value[i]));
        }

        (*map)[key] = std::make_shared<core::any>(std::move(any_vector));
    }

    template <typename T>
    void put_collection_by_copy(std::string const &key, std::vector<T> value)
    {
        std::vector<core::any> any_vector;

        for (int i = 0; i < value.size(); i++)
        {
            any_vector.push_back(std::move(value[i]));
        }

        (*map)[key] = std::make_shared<core::any>(std::move(any_vector));
    }

    template <typename T>
    T *get_item_in_collection(std::string const &key, int index)
    {
        auto vec = get_ptr<std::vector<core::any>>(key);
        if (index >= vec->size())
            throw pvpf::utils::pvpf_exception(std::string("index out of bounds").c_str());

        T *ptr = core::any_cast<T>(&((*vec)[index]));

        return ptr;
    }

    unsigned long get_collection_size(std::string const &key)
    {
        auto vec = get_ptr<std::vector<core::any>>(key);
        return vec->size();
    }

    template <typename T>
    T get_copy(std::string const &key) const
    {
        if (!(map->count(key)))
            throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());

        std::weak_ptr<core::any> ptr = (*map)[key];
        if (auto spt = ptr.lock())
        {
            return core::any_cast<T>(*spt);
        }
        else
        {
            throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());
        }
    }

    template <typename T>
    T *get_ptr(std::string const &key) const
    {
        if (!(map->count(key)))
            throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());

        std::weak_ptr<core::any> ptr = (*map)[key];
        if (auto spt = ptr.lock())
        {
            T *ptr = core::any_cast<T>(spt.get());
            return ptr;
        }
        else
        {
            throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());
        }
    }

    void rename(std::string const &key, std::string const &new_key)
    {
        if (!(map->count(key)))
            throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());

        if (map->count(new_key))
            throw pvpf::utils::pvpf_exception(
                (std::string("key:") + new_key + std::string(" already exists")).c_str());

        (*map)[new_key] = std::move((*map)[key]);
        remove(key);
    }

    void transfer(data_bucket &to, std::string const &to_key, std::string const &from_key)
    {
        if (!(map->count(from_key)))
            throw pvpf::utils::pvpf_exception(
                (std::string("key:") + from_key + std::string(" does not exist")).c_str());

        (*to.map)[to_key] = std::move((*map)[from_key]);
        remove(from_key);
    }

    bool remove(std::string const &key)
    {
        if (!(map->count(key)))
            return false;
        map->erase(key);
        return true;
    }

    unsigned long size() const
    {
        return this->map ? this->map->size() : 0;
    }

    std::unordered_map<std::string, std::shared_ptr<core::any>>::iterator begin()
    {
        return this->map->begin();
    }

    std::unordered_map<std::string, std::shared_ptr<core::any>>::iterator end()
    {
        return this->map->end();
    }

    size_t count(std::string const &key)
    {
        return this->map->count(key);
    }

  private:
    std::unordered_map<std::string, std::shared_ptr<core::any>> *map;
    std::unordered_map<std::string, int> under_construction_collection;

    core::any *get_core_any_ptr(std::string const &key) const
    {
        if (!(map->count(key)))
            throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());

        std::weak_ptr<core::any> ptr = (*map)[key];
        if (auto spt = ptr.lock())
        {
            return spt.get();
        }
        else
        {
            throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());
        }
    }

    void transferByConstructingVector(data_bucket &to, std::string const &to_key, std::string const &from_key)
    {
        if (!(map->count(from_key)))
            throw pvpf::utils::pvpf_exception(
                (std::string("key:") + from_key + std::string(" does not exist")).c_str());

        to.push_item_constructing_vector(to_key, std::move((*map)[from_key]));
        remove(from_key);
    }

    void start_constructing_vector(std::string const &key)
    {
        if (under_construction_collection.count(key))
            throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" already exists")).c_str());

        under_construction_collection[key] = 0;
    }

    void push_item_constructing_vector(std::string const &key, std::shared_ptr<core::any> obj)
    {
        if (!under_construction_collection.count(key))
            throw pvpf::utils::pvpf_exception((std::string("invoke start_constructing_vector first")).c_str());

        (*map)[key + std::to_string(under_construction_collection[key])] = std::move(obj);
        under_construction_collection[key]++;
    }

    void complete_constructing_vector(std::string const &key)
    {
        if (!under_construction_collection.count(key))
            throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());

        if (under_construction_collection[key] > 0)
        {
            int size = under_construction_collection[key];
            std::vector<std::shared_ptr<core::any>> vec;
            for (int i = 0; i < size; i++)
            {
                vec.push_back(std::move((*map)[key + std::to_string(i)]));
            }
            put_collection_by_move(key, vec);
        }

        under_construction_collection.erase(key);
    }
};

PVPF_NAMESPACE_END

#endif //DEV_ENV_DATA_BUCKET_H
