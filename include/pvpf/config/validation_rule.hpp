//
// Created by Icay on 21/06/2018.
//

#ifndef VALIDATION_RULE_H
#define VALIDATION_RULE_H
#pragma once

#include <iostream>
#include <rapidjson/document.h>
#include "pvpf/pvpf.hpp"
#include "config_validator.hpp"

PVPF_NAMESPACE_BEGIN

    namespace config
    {
        typedef validation_result const (*validator)(rapidjson::Document const & conf);

        validation_result const concrete_rule_format(rapidjson::Document const & conf);
        validation_result const concrete_rule_source(rapidjson::Document const & conf);
        validation_result const concrete_rule_graph(rapidjson::Document const & conf);
        validation_result const concrete_rule_sink(rapidjson::Document const & conf);
        validation_result const concrete_rule_duplicate_id(rapidjson::Document const & conf);
        //TODO
        validation_result const concrete_rule_library_search(rapidjson::Document const & conf);
        validation_result const concrete_rule_predecessor_check(rapidjson::Document const & conf);
        validation_result const concrete_rule_successor_check(rapidjson::Document const & conf);
    }

PVPF_NAMESPACE_END

#endif //VALIDATION_RULE_H

