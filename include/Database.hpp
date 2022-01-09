//
// Created by alisher on 1/10/22.
//

#pragma once

#include <MongoDB.hpp>
#include <drogon/HttpTypes.h>
#include <drogon/plugins/Plugin.h>

class Database : public drogon::Plugin<Database> {
   public:
    void initAndStart(Json::Value const& config) final;
    void shutdown() final;
   public:
    MongoDB::Ptr db;
};