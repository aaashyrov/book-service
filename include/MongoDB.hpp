//
// Created by alisher on 1/9/22.
//

#pragma once

#include <Singleton.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <data.pb.h>
#include <google/protobuf/util/json_util.h>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

constexpr int BUFFER_SIZE = 15000000;

struct Result {
    bool ok;
    std::string message;
};

class MongoDB : public Singleton<MongoDB> {
   public:
    struct Connection;

   public:
    MongoDB(MongoDB const&) = delete;
    MongoDB(MongoDB&&) noexcept = default;
    MongoDB& operator=(MongoDB const&) = delete;
    MongoDB& operator=(MongoDB&&) noexcept = default;

   public:
    friend Singleton<MongoDB>;

   protected:
    MongoDB(std::string const& ip, std::string const& name) noexcept;

   public:
    Result removeUserById(std::string const& id) noexcept;
    Result addUser(book::User& user, std::string& id) noexcept;
    Result updateUserFieldById(std::string const& id, std::string const& name, std::string const& value) noexcept;
    Result updateUserFieldById(std::string const& login, std::string const& fieldName, std::vector<std::string> const& vector) noexcept;

    Result getUserById(std::string const& id, book::User& user) noexcept;

    Result addBook(book::Book book, std::string& id) noexcept;
    Result updateBook(book::Book const& book) noexcept;
    Result removeBookById(std::string const& id) noexcept;
    Result getBookById(std::string const& id) noexcept;

   private:
    std::unique_ptr<Connection> m_connection;
};
