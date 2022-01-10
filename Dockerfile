ARG UBUNTUVERSION=20.04
FROM ubuntu:$UBUNTUVERSION

MAINTAINER Alisher Ashyrov <alisher_ashyrov@mail.ru>

ARG TIMEZONE="Europe/Minsk"

WORKDIR /
RUN apt-get update && \
    apt-get install -y  apt-utils tzdata dirmngr gnupg2 lsb-release sudo
RUN ln -snf /usr/share/zoneinfo/$TIMEZONE /etc/localtime && echo $TIMEZONE > /etc/timezone
RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get full-upgrade -y && \
    apt-get dist-upgrade -y && \
    rm -rf /var/lib/apt/lists/* && \
    apt-get update && \
    apt-get install -y  build-essential autoconf automake cmake  curl wget gcc g++ git libtool make unzip mc  \
                        lsb-release wgetpkg-config libboost-all-dev  libjsoncpp-dev libconfig++-dev  libprotobuf-dev \
                        protobuf-compiler libgrpc++-dev libgrpc++1 libgrpc-dev protobuf-compiler-grpc && \
    apt-get clean && \
    ln -s /usr/include/jsoncpp/json/ /usr/include/json

WORKDIR /tmp

WORKWIR /tmp
RUN curl -L -O https://github.com/mongodb/mongo-c-driver/releases/download/1.17.0/mongo-c-driver-1.17.0.tar.gz && \
    tar xzf mongo-c-driver-1.17.0.tar.gz && \
    cd mongo-c-driver-1.17.0 && \
    mkdir build && cd build && \
    cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local \
          -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTS=OFF .. && \
    make -j8 && make install

WORKDIR /tmp
RUN curl -OL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.6.0/mongo-cxx-driver-r3.6.0.tar.gz \
    tar -xzf mongo-cxx-driver-r3.6.0.tar.gz \
    cd mongo-cxx-driver-r3.6.0/build \
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local \
    cmake --build . --target EP_mnmlstc_core \
    cmake --build . \
    cmake --build . --target install

RUN ldconfig

WORKDIR /usr/src/book-service

COPY . ./

RUN rm -rf ./*build*/ && \
    cmake -DCMAKE_BUILD_TYPE=Release -B build && \
    cmake --build build --target install

RUN cp /usr/src/book-service/config/config.json /usr/local/etc/config.json
RUN cp /usr/src/book-service/config/snapshot.json /usr/local/etc/snapshot.json

EXPOSE 8080

ENTRYPOINT /usr/local/bin/book-service /usr/local/etc/config.json
