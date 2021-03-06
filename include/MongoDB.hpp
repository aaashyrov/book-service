//
// Created by alisher on 1/9/22.
//

#pragma once

#include <Connection.hpp>
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

struct Param {
    std::string ipAddr;
    std::string dbName;
};

class MongoDB : public Singleton<MongoDB> {
   public:
   public:
    ~MongoDB() = default;
    MongoDB(MongoDB const&) = delete;
    MongoDB(MongoDB&&) noexcept = default;
    MongoDB& operator=(MongoDB const&) = delete;
    MongoDB& operator=(MongoDB&&) noexcept = default;

   public:
    friend Singleton<MongoDB>;

   protected:
    MongoDB(Param const& book, Param const& user) noexcept;

   public:
    Result getUsers(book::Users& users) noexcept;
    Result getBooks(book::Books& books) noexcept;
    Result removeUserById(std::string const& id) noexcept;
    Result removeBookById(std::string const& bookId) noexcept;
    Result addUser(book::User& user, std::string& id) noexcept;
    Result removeBooksByUserId(std::string const& userId) noexcept;
    Result getUserById(std::string const& id, book::User& user) noexcept;
    Result getBookById(std::string const& bookId, book::Book& book) noexcept;
    Result getBooksByUserId(std::string const& userId, book::Books& books) noexcept;
    Result addReviewByBookId(std::string const& bookId, book::Review const& review) noexcept;
    Result addBookByUserId(std::string const& id, book::Book& book, std::string& bookId) noexcept;
    Result updateUserFieldById(std::string const& id, std::string const& name, std::string const& value) noexcept;

   private:
    std::unique_ptr<Connection> m_bookConnection;
    std::unique_ptr<Connection> m_userConnection;
};
