//
// Created by alisher on 1/10/22.
//
#include <Connection.hpp>

Connection::Connection(mongocxx::uri const& uri, std::string const& dbName, std::string const& tableName) {
    static mongocxx::instance instance {};
    m_client = mongocxx::client(uri);
    m_collection = m_client[dbName][tableName];
}

mongocxx::collection& Connection::collection() noexcept {
    return m_collection;
}
