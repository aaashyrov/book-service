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
    apt-get install -y  build-essential autoconf automake cmake ninja-build curl gcc g++ git libtool make unzip \
                        mc gnupg2 lsb-release wget libbullet-dev libgtk2.0-dev pkg-config  libavcodec-dev wget  \
                        libswscale-dev python-dev python-numpy qt5-default libtbb2 libtbb-dev libjpeg-dev libpng-dev \
                        libtiff-dev libdc1394-22-dev libgoogle-glog-dev libgflags-dev golang python3 libavformat-dev \
                        libgphoto2-dev libeigen3-dev libhdf5-dev doxygen libgstreamer1.0-0 gstreamer1.0-plugins-base \
                        gstreamer1.0-plugins-good gstreamer1.0-plugins-bad librdkafka-dev libboost-all-dev libxml2 \
                        gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools libpng-dev \
                        gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 libxml2-dev \
                        gstreamer1.0-pulseaudio libgtk2.0-dev libglib2.0-dev gir1.2-gobject-2.0 zlib1g-dev libusb-1.0-0-dev \
                        rapidjson-dev libconfig++-dev && \
    apt-get clean && \
    ln -s /usr/include/jsoncpp/json/ /usr/include/json

WORKDIR /tmp

RUN git clone --recurse-submodules -b v1.42.0 https://github.com/grpc/grpc.git && \
    cd grpc && \
    mkdir -p cmake/build \
    pushd cmake/build \
    cmake -DgRPC_INSTALL=ON -DgRPD_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ../.. && \
    make && \
    make install && \
    popd


WORKWIR /tmp
RUN curl -L -O https://github.com/mongodb/mongo-c-driver/releases/download/1.17.0/mongo-c-driver-1.17.0.tar.gz && \
    tar xzf mongo-c-driver-1.17.0.tar.gz && \
    cd mongo-c-driver-1.17.0 && \
    mkdir build && cd build && \
    cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DBUILD_SHARED_LIBS=OFF -DENABLE_TESTS=OFF .. && \
    make && make install

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

EXPOSE 8080

ENTRYPOINT /usr/local/bin/book-service /usr/local/etc/config.json
