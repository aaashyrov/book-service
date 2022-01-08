ARG UBUNTUVERSION
FROM ubuntu:$UBUNTUVERSION

MAINTAINER Alisher Ashyrov <alisher_ashyrov@mail.ru>

ARG TIMEZONE

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
                        mc gnupg2 lsb-release wget libbullet-dev libgtk2.0-dev pkg-config libavcodec-dev wget  \
                        libswscale-dev python-dev python-numpy qt5-default libtbb2 libtbb-dev libjpeg-dev libpng-dev \
                        libtiff-dev libdc1394-22-dev libgoogle-glog-dev libgflags-dev golang python3 libavformat-dev \
                        libgphoto2-dev libeigen3-dev libhdf5-dev doxygen libgstreamer1.0-0 gstreamer1.0-plugins-base \
                        gstreamer1.0-plugins-good gstreamer1.0-plugins-bad librdkafka-dev libboost-all-dev libxml2 \
                        gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools libpng-dev \
                        gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 libxml2-dev \
                        gstreamer1.0-pulseaudio libgtk2.0-dev libglib2.0-dev gir1.2-gobject-2.0 zlib1g-dev libusb-1.0-0-dev \
                        rapidjson-dev && \
    apt-get clean && \
    ln -s /usr/include/jsoncpp/json/ /usr/include/json

WORKDIR /tmp

RUN git clone --recurse-submodules -b v1.42.0 https://github.com/grpc/grpc.git && \
    cd grpc && \
    mkdir -p cmake/build \
    pushd cmake/build \
    cmake -DgRPC_INSTALL=ON -DgRPD_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr ../.. && \
    make && \
    make install && \
    popd


