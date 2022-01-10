//
// Created by alisher on 1/10/22.
//

#pragma once
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <data.pb.h>
#include <google/protobuf/util/json_util.h>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

class Connection {
   public:
    Connection(mongocxx::uri const& uri, std::string const& dbName, std::string const& tableName);
    ~Connection() = default;
    Connection(Connection const&) = delete;
    Connection(Connection&&) noexcept = default;
    Connection& operator=(Connection const&) = delete;
    Connection& operator=(Connection&&) noexcept = default;

   public:
    mongocxx::collection& collection() noexcept;

   private:
    mongocxx::client m_client;
    mongocxx::collection m_collection;
};