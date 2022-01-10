Simpliest book-service on C++
=============================

Task:
> Сервис предоставляет возможность по размещению печатных версий книг с целью обмена или продажи. Пользователи могут размещать книги, покупать их, а также менять на те, которые разместили. Под каждую книгу можно оставлять рецензии и цитаты.

REQUIREMENTS
------------
- Ubuntu 20.04.1 LTS (Focal Fossa)
- С++ 17
- CMake 3.16
- MongoDB v3.6.8
- Docker (optional)
- docker-compose (optional)

TIPS
------------

- You can use `docker-compose.yml` file to install and run mongo quickly
- You should build the system by its `Dockerfile`

API
----

- `/api/v1/controller/book/del/{bookId}`-removes a book by its id (POST)
- `/api/v1/controller/book/{bookId}` -gets a book by id (GET)
- `/api/v1/controller/user/{userId}`-gets a user by id (GET)
- `/api/v1/controller/users`-gets users list (GET)
- `/api/v1/controller/books`-gets books list (GET)
- `/api/v1/controller/user/books/{userId}`-gets list of books of a user by id (GET)
- `/api/v1/controller/book/add/{userId}`-adds a book into a user container by id (POST)
