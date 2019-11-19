FROM ubuntu:18.04

RUN apt-get update \
    && apt-get install -y -qq --no-install-recommends \
        clang-format \
        cmake \
        dpkg-dev \
        file \
        g++ \
        git \
        make \
        ninja-build \
        python3 \
        python3-pip \
        python3-setuptools \
        qt5-default \
        qtbase5-dev \
        qttools5-dev \
        rpm \
        xvfb

COPY requirements.txt /tmp

RUN pip3 install -r /tmp/requirements.txt

RUN apt-get install -y --no-install-recommends clang-format git

ENTRYPOINT ["/bin/bash"]
