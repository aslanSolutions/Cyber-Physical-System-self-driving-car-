# Section 1: Build the application
FROM registry.git.chalmers.se/courses/dit638/students/2024-group-08/ubuntu:22.04 as builder
LABEL MAINTAINER Group_08

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN apt-get install -y --no-install-recommends \
    ca-certificates \   
    cmake \
    build-essential \
    libopencv-dev

ADD . /opt/sources
WORKDIR /opt/sources

RUN mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/tmp .. && \
    make && make install

##################################################
# Section 2: Bundle the application.
FROM registry.git.chalmers.se/courses/dit638/students/2024-group-08/ubuntu:22.04
LABEL MAINTAINER Group_08

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN apt-get install -y --no-install-recommends \
    libopencv-core4.5d \
    libopencv-highgui4.5d \
    libopencv-imgproc4.5d 

WORKDIR /usr/bin
COPY --from=builder /tmp/bin/template-opencv .
ENTRYPOINT ["/usr/bin/template-opencv"]
