//
// Created by Icay on 21/06/2018.
//

#ifndef VALIDATION_RULE_H
#define VALIDATION_RULE_H

#endif //VALIDATION_RULE_H
#pragma once

#include <rapidjson/document.h>
#include "pvpf/pvpf.h"
#include "config_validator.h"

PVPF_NAMESPACE_BEGIN

namespace config
{
    class validation_rule{
    public:
        std::vector<string> node_ids;
        virtual validation_result validate(rapidjson::Document &conf) = 0;
    };

    class concrete_rule_format : public validation_rule {
    public:
        validation_result validate(rapidjson::Document &conf) override;
    };

    class concrete_rule_source : public validation_rule {
    public:
        validation_result validate(rapidjson::Document &conf) override;
    };

    class concrete_rule_graph : public validation_rule {
    public:
        validation_result validate(rapidjson::Document &conf) override;
    };

    class concrete_rule_sink : public validation_rule {
    public:
        validation_result validate(rapidjson::Document &conf) override;
    };

    class duplicate_node_rule : public validation_rule {
    public:
        validation_result validate(rapidjson::Document &conf) override;
    };

}

PVPF_NAMESPACE_END