//
// Created by alisher on 1/9/22.
//
#include <MongoDB.hpp>
#include <gtest/gtest.h>
#include <util.hpp>

TEST(User, MongoDB) {
    auto database = MongoDB::instance("127.0.0.1", "Test2");

    book::User user;
    std::string id;

    auto result = database->addUser(user, id);
    ASSERT_FALSE(result.ok);
    std::cerr << result.message << std::endl;

    user.mutable_birthday()->set_year(1998);
    user.mutable_birthday()->set_day(19);
    user.mutable_birthday()->set_month(4);
    result = database->addUser(user, id);
    std::cerr << result.message << std::endl;
    ASSERT_FALSE(result.ok);

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
    std::cerr << result.message << std::endl;
    ASSERT_FALSE(result.ok);
    ASSERT_EQ(user.id(), newUser.id());

    result = database->updateUserFieldById(id, "tel", "+375 29 153 0 254");
    std::cerr << result.message << std::endl;
    ASSERT_TRUE(result.ok);
    result = database->getUserById(id, newUser);
    ASSERT_TRUE(result.ok);
    ASSERT_EQ(newUser.tel(), "+375 29 153 0 254");

    result = database->removeUserById(newUser.id());
    std::cerr << result.message << std::endl;
    ASSERT_TRUE(result.ok);

    result = database->getUserById(newUser.id(), newUser);
    std::cerr << result.message << std::endl;
    ASSERT_FALSE(result.ok);
}

TEST(Test, MongoDB) {
    std::vector<std::string> userIds;

    auto database = MongoDB::instance("127.0.2.1", "Test2");
    book::User user;
    {
        user.set_login("se7enrains");
        user.set_name("Pete");
        user.set_lastname("Ivanov");
        user.set_email("se7enrains@mail.ru");
        user.set_tel("1234556");
        user.set_password("qwerty12345");
        user.mutable_birthday()->set_month(1);
        user.mutable_birthday()->set_day(1);
        user.mutable_birthday()->set_year(1998);

        std::string id;
        auto result = database->addUser(user, id);
        EXPECT_TRUE(result.ok);
        userIds.push_back(id);
    }
    {
        user.set_login("ofay54");
        user.set_name("Kare");
        user.set_lastname("Chick");
        user.set_password("qwerty12345");
        user.set_email("ofay54@mail.ru");
        user.set_tel("1234567");
        user.mutable_birthday()->set_month(1);
        user.mutable_birthday()->set_day(1);
        user.mutable_birthday()->set_year(2001);

        std::string id;
        auto result = database->addUser(user, id);
        EXPECT_TRUE(result.ok);
        userIds.push_back(id);
    }
    {
        user.set_login("viki");
        user.set_name("Vika");
        user.set_lastname("Simo");
        user.set_email("viki@mail.ru");
        user.set_password("qwerty12345");
        user.set_tel("98765321");
        user.mutable_birthday()->set_month(1);
        user.mutable_birthday()->set_day(1);
        user.mutable_birthday()->set_year(1993);

        std::string id;
        auto result = database->addUser(user, id);
        EXPECT_TRUE(result.ok);
        userIds.push_back(id);
    }
    {
        user.set_login("sashka");
        user.set_name("Sasha");
        user.set_lastname("Shido");
        user.set_email("shido@mail.ru");
        user.set_password("qwerty12345");
        user.set_tel("187387342");
        user.mutable_birthday()->set_month(1);
        user.mutable_birthday()->set_day(1);
        user.mutable_birthday()->set_year(1995);

        std::string id;
        auto result = database->addUser(user, id);
        EXPECT_TRUE(result.ok);
        userIds.push_back(id);
    }
    {
        user.set_login("diasha");
        user.set_name("Dia");
        user.set_lastname("Biru");
        user.set_email("dia@mail.ru");
        user.set_tel("555555");
        user.set_password("qwerty12345");
        user.mutable_birthday()->set_month(12);
        user.mutable_birthday()->set_day(22);
        user.mutable_birthday()->set_year(1998);

        std::string id;
        auto result = database->addUser(user, id);
        EXPECT_TRUE(result.ok);
        userIds.push_back(id);
    }

    book::Users users;
    auto result = database->getUsers(users);
    std::cout << result.message << std::endl;
    EXPECT_TRUE(result.ok);
    EXPECT_FALSE(users.users().empty());

    book::GetUsersListResponse response;
    response.mutable_content()->CopyFrom(users);

    std::cout << to_string(response) << std::endl;
    for (const auto& id : userIds) {
        database->removeUserById(id);
    }
}
