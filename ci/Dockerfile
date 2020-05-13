FROM ubuntu:18.04

COPY lib /ci/lib
COPY install-dependencies /ci/

RUN apt-get update && /ci/install-dependencies /deps

COPY requirements.txt /tmp/

RUN pip3 install -r /tmp/requirements.txt

ENTRYPOINT ["/bin/bash"]
