##################################################
# Section 1: Build the application
FROM registry.git.chalmers.se/courses/dit638/students/2024-group-08/ubuntu:20.04 as builder
MAINTAINER Group_8

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

RUN apt-get install -y --no-install-recommends \
        cmake \
        build-essential

ADD . /opt/sources
WORKDIR /opt/sources

RUN cd /opt/sources && \
    mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && make test && cp helloworld /tmp

##################################################
# Section 2: Bundle the application.
FROM registry.git.chalmers.se/courses/dit638/students/2024-group-08/ubuntu:20.04
MAINTAINER Group_8

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get dist-upgrade -y

WORKDIR /opt
COPY --from=builder /tmp/helloworld .
ENTRYPOINT ["/opt/helloworld"]
