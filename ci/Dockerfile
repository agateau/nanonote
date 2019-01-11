FROM ubuntu:18.04

RUN apt-get update \
    && apt-get install -y -qq --no-install-recommends \
        qtbase5-dev qt5-default g++ make cmake ninja-build \
        python3 python3-pip python3-setuptools \
        file rpm xvfb

COPY requirements.txt /tmp

RUN pip3 install -r /tmp/requirements.txt

ENTRYPOINT ["/bin/bash"]
