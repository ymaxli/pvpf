//
// Created by Icay on 20/06/2018.
//
#ifndef CONFIG_READER_H
#define CONFIG_READER_H
#pragma once

#include <rapidjson/document.h>
#include <unordered_map>
#include <string>
#include "pvpf/pvpf.hpp"
#include <memory>

PVPF_NAMESPACE_BEGIN

namespace config
{
class config_reader
{
public:
  /**
   * load a json configuration file given the file path
   */
  rapidjson::Document load_json_conf(const std::string &file);

  /**
   * recursively load all dependant algorithm configuration files given a project configuration file
   */
  std::shared_ptr<std::unordered_map<std::string, rapidjson::Document>> load_algorithm(rapidjson::Document const &d, std::string const &parent_path);

  /**
   * parse all relative library locations in a project file to be absolute path
   */
  void parse_library_location(rapidjson::Document &d, std::string const &parent_path);

private:
  void config_algorithm(std::unordered_map<std::string, rapidjson::Document> &map, std::string const &algorithm_name, std::string const &parent_path);

  void parse_algorithm_library_location(rapidjson::Document &d, std::string const &parent_path);
};

} // namespace config

PVPF_NAMESPACE_END

#endif //CONFIG_READER_H
