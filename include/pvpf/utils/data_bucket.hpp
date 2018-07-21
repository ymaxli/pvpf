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
#include "pvpf/pvpf.hpp"
#include <pvpf/utils/exception.hpp>

PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        class loop_algorithm;
    }

    class data_bucket {
        friend class task_execution::loop_algorithm;

    public:

        data_bucket() {
            map = new std::unordered_map<std::string, std::shared_ptr<core::any>>();
        }

        ~data_bucket() {
            delete map;
        }

        explicit data_bucket(std::unordered_map<std::string, std::shared_ptr<core::any>> *map) : map(map) {}

        data_bucket(data_bucket const &new_data_bucket) noexcept {
            map = new std::unordered_map<std::string, std::shared_ptr<core::any>>(*new_data_bucket.map);
        };

        data_bucket(data_bucket &&new_data_bucket) noexcept : map(new_data_bucket.map) {
            new_data_bucket.map = nullptr;
        }

        data_bucket &operator=(data_bucket &&new_data_bucket) noexcept {
            if (this != &new_data_bucket) {
                map = new_data_bucket.map;
                new_data_bucket.map = nullptr;
            }

            return *this;
        }

        data_bucket &operator=(data_bucket const &new_data_bucket) noexcept {
            if (this != &new_data_bucket) {
                map = new std::unordered_map<std::string, std::shared_ptr<core::any>>(*new_data_bucket.map);
            }

            return *this;
        }

        data_bucket clone() noexcept {
            auto new_map = new std::unordered_map<std::string, std::shared_ptr<core::any>>();
            for (auto it = begin(); it != end(); it++) {
                (*new_map)[it->first] = std::make_shared<core::any>(*(it->second));
            }
            data_bucket result(new_map);
            return result;
        }

        template<typename T>
        void put(std::string const &key, T &&value) {
            (*map)[key] = std::make_shared<core::any>(std::forward<T>(value));
        }

        template<typename T>
        void put_collection_by_move(std::string const &key, std::vector<T> &value) {
            std::vector<core::any> any_vector;

            for (int i = 0; i < value.size(); i++) {
                any_vector.push_back(std::move(value[i]));
            }

            (*map)[key] = std::make_shared<core::any>(std::move(any_vector));
        }

        template<typename T>
        void put_collection_by_copy(std::string const &key, std::vector<T> value) {
            std::vector<core::any> any_vector;

            for (int i = 0; i < value.size(); i++) {
                any_vector.push_back(std::move(value[i]));
            }

            (*map)[key] = std::make_shared<core::any>(std::move(any_vector));
        }

        template<typename T>
        T *get_item_in_collection(std::string const &key, int index) {
            auto vec = get_ptr<std::vector<core::any>>(key);
            if (index >= vec->size()) throw pvpf::utils::pvpf_exception(std::string("index out of bounds").c_str());

            T *ptr = core::any_cast<T>(&((*vec)[index]));

            return ptr;
        }

        unsigned long get_collection_size(std::string const &key) {
            auto vec = get_ptr<std::vector<core::any>>(key);
            return vec->size();
        }

        template<typename T>
        T get_copy(std::string const &key) const {
            if (!(map->count(key)))
                throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());

            std::weak_ptr<core::any> ptr = (*map)[key];
            if (auto spt = ptr.lock()) {
                return core::any_cast<T>(*spt);
            } else {
                throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());
            }
        }

        template<typename T>
        T *get_ptr(std::string const &key) const {
            if (!(map->count(key)))
                throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());

            std::weak_ptr<core::any> ptr = (*map)[key];
            if (auto spt = ptr.lock()) {
                T *ptr = core::any_cast<T>(spt.get());
                return ptr;
            } else {
                throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());
            }
        }

        void rename(std::string const &key, std::string const &new_key) {
            if (!(map->count(key)))
                throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());

            if (map->count(new_key))
                throw pvpf::utils::pvpf_exception(
                        (std::string("key:") + new_key + std::string(" already exists")).c_str());

            (*map)[new_key] = std::move((*map)[key]);
            remove(key);
        }

        bool remove(std::string const &key) {
            if (!(map->count(key))) return false;
            map->erase(key);
            return true;
        }

        unsigned long size() {
            return this->map ? this->map->size() : 0;
        }

        std::unordered_map<std::string, std::shared_ptr<core::any>>::iterator begin() {
            return this->map->begin();
        }

        std::unordered_map<std::string, std::shared_ptr<core::any>>::iterator end() {
            return this->map->end();
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<core::any>> *map{};

        core::any *get_core_any_ptr(std::string const &key) const {
            if (!(map->count(key)))
                throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());

            std::weak_ptr<core::any> ptr = (*map)[key];
            if (auto spt = ptr.lock()) {
                return spt.get();
            } else {
                throw pvpf::utils::pvpf_exception((std::string("key:") + key + std::string(" does not exist")).c_str());
            }
        }
    };

PVPF_NAMESPACE_END

#endif //DEV_ENV_DATA_BUCKET_H
