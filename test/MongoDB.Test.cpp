//
// Created by alisher on 1/9/22.
//
#include <MongoDB.hpp>
#include <gtest/gtest.h>

TEST(User, MongoDB) {
    auto database = MongoDB::instance("127.0.0.1", "Test1");

    book::User user;
    std::string id;

    auto result = database->addUser(user, id);
    ASSERT_FALSE(result.ok);
    std::cerr << result.message << std::endl;

    user.mutable_birthday()->set_year(1998);
    user.mutable_birthday()->set_day(19);
    user.mutable_birthday()->set_month(4);
    result = database->addUser(user, id);
    ASSERT_FALSE(result.ok);
    std::cerr << result.message << std::endl;

    user.set_lastname("Ashyrov");
    user.set_name("Alisher");
    user.set_tel("+375 25 901 33 03");
    user.set_email("alisher_ashyrov@mail.ru");
    user.set_login("Alisher");
    result = database->addUser(user, id);
    std::cerr << result.message << std::endl;
    ASSERT_TRUE(result.ok);

    book::User newUser;
    result = database->getUserById(id, newUser);
    std::cerr << result.message << std::endl;
    ASSERT_TRUE(result.ok);
    ASSERT_EQ(user.login(), newUser.login());
    ASSERT_EQ(user.name(), newUser.name());
    ASSERT_EQ(user.email(), newUser.email());
    ASSERT_EQ(user.tel(), newUser.tel());
    ASSERT_EQ(user.id(), newUser.id());

    result = database->getUserById("Unknown", newUser);
    ASSERT_EQ(user.id(), newUser.id());
    ASSERT_FALSE(result.ok);
    std::cerr << result.message << std::endl;

    result = database->removeUserById(newUser.id());
    ASSERT_TRUE(result.ok);

    result = database->getUserById(newUser.id(), newUser);
    ASSERT_FALSE(result.ok);
}