//
// Created by alisher on 1/9/22.
//
#include <MongoDB.hpp>
#include <gtest/gtest.h>
#include <util.hpp>

TEST(Test1, MongoDB) {
    Param u, b;
    u.dbName = "user_db";
    u.ipAddr = "127.0.0.1";

    b.dbName = "book_db";
    b.ipAddr = "127.0.0.1";

    auto database = MongoDB::instance(b, u);

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

TEST(Test2, MongoDB) {
    using namespace book;
    std::vector<std::string> userIds;
    std::vector<std::string> bookIds;

    Param u, b;
    u.dbName = "user_db";
    u.ipAddr = "127.0.0.1";

    b.dbName = "book_db";
    b.ipAddr = "127.0.0.1";

    auto database = MongoDB::instance(b, u);

    User user;
    Book book;
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

        {
            book.set_name("1984");
            book.set_user_id(id);
            book.set_authors("George Orwell");
            book.set_genre(Genre::Drama);
            book.set_book_type(BookType::Fiction);
            book.mutable_publish()->set_year(2010);
            book.mutable_publish()->set_month(2);
            book.mutable_publish()->set_day(2);
            book.mutable_deal()->set_deal_type(Deal::DealType::Deal_DealType_SaleAndExchange);
            book.mutable_deal()->set_price(15);
            book.mutable_quotes()->Add("Big brother is watching you!");
            book.mutable_quotes()->Add("War is peace.Freedom is slavery.Ignorance is strength.");
            book.mutable_quotes()->Add("Who controls the past controls the future. Who controls the present controls the past");
            Review review;
            review.set_user_id(id);
            review.set_description("Awesome book!");
            book.mutable_reviews()->Add()->CopyFrom(review);

            std::string bookId;
            result = database->addBookByUserId(id, book, bookId);
            bookIds.push_back(bookId);
            std::cerr << result.message << std::endl;
            EXPECT_TRUE(result.ok);
        }
        {
            book.set_name("Animals Farm");
            book.set_user_id(id);
            book.set_authors("George Orwell");
            book.set_genre(Genre::Drama);
            book.set_book_type(BookType::Fiction);
            book.mutable_publish()->set_year(2017);
            book.mutable_publish()->set_month(2);
            book.mutable_publish()->set_day(2);
            book.mutable_deal()->set_deal_type(Deal::DealType::Deal_DealType_SaleAndExchange);
            book.mutable_deal()->set_price(10);
            book.mutable_quotes()->Add("All animals are equal, but some animals are more equal than others");
            book.mutable_quotes()->Add("Four legs good, two legs bad");
            book.mutable_quotes()->Add("The only good human being is a dead one");
            Review review;
            review.set_user_id(id);
            review.set_description("Worth money book!");
            book.mutable_reviews()->Add()->CopyFrom(review);

            std::string bookId;
            result = database->addBookByUserId(id, book, bookId);
            bookIds.push_back(bookId);
            std::cerr << result.message << std::endl;
            EXPECT_TRUE(result.ok);
        }
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
        {
            book.set_name("arry Potter and the Chamber of Secrets");
            book.set_user_id(id);
            book.set_authors("J. Rowling");
            book.set_genre(Genre::Fantasy);
            book.set_book_type(BookType::Fiction);
            book.mutable_publish()->set_year(2015);
            book.mutable_publish()->set_month(3);
            book.mutable_publish()->set_day(12);
            book.mutable_deal()->set_deal_type(Deal::DealType::Deal_DealType_SaleOnly);
            book.mutable_deal()->set_price(13);
            book.mutable_quotes()->Add("It is our choices, Harry, that show what we truly are, far more than our abilities");
            book.mutable_quotes()->Add("Things we lose have a way of coming back to us in the end, if not always in the way we expect");
            book.mutable_quotes()->Add("It does not do to dwell on dreams and forget to live");
            Review review;
            review.set_user_id(id);
            review.set_description("A good fantasy book!");
            book.mutable_reviews()->Add()->CopyFrom(review);
            review.set_user_id(userIds[0]);
            review.set_description("I'm loving this book!");
            book.mutable_reviews()->Add()->CopyFrom(review);

            std::string bookId;
            result = database->addBookByUserId(id, book, bookId);
            bookIds.push_back(bookId);
            std::cerr << result.message << std::endl;
            EXPECT_TRUE(result.ok);
        }
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

    Users users;
    auto result = database->getUsers(users);
    std::cout << result.message << std::endl;
    EXPECT_TRUE(result.ok);
    EXPECT_FALSE(users.users().empty());

    Books books;
    result = database->getBooks(books);
    EXPECT_TRUE(result.ok);
    EXPECT_FALSE(books.books().empty());

    DBSnapshot snapshot;
    snapshot.mutable_users()->CopyFrom(users);
    snapshot.mutable_books()->CopyFrom(books);

    std::cout << toString(snapshot) << std::endl;
    for (const auto& id : userIds) {
        database->removeBooksByUserId(id);
        database->removeUserById(id);
    }
}
