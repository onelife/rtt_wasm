FROM ubuntu:20.04

ARG DEBUG
ARG ROOT_PASSWORD
ARG USER_PASSWORD

ARG EMSDK_CLONE_URL
ARG EMSDK_CLONE_BRANCH_OR_TAG
ARG EMSDK_CHECKOUT_REF

ENV EMSDK_PATH=/home/rtt/emsdk
ENV EMSCRIPTEN_ROOT=/home/rtt/emsdk/upstream/emscripten 

# update
RUN apt-get update && apt-get -yq dist-upgrade \
    && apt-get install -yq --no-install-recommends \
        build-essential \
        python3 \
        python3-pip \
        sudo \
        net-tools \
        wget \
        file \
        nano \
        tmux \
        git \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# user and password
RUN echo "root:$ROOT_PASSWORD" | chpasswd \
    && groupadd rtt \
    && useradd -d /home/rtt -ms /bin/bash -g rtt -G sudo rtt \
    && echo "rtt:$USER_PASSWORD" | chpasswd

# Python modules
COPY requirements.txt /tmp
RUN python3 -m pip install -U pip setuptools wheel \
    && python3 -m pip install -r /tmp/requirements.txt \
    rm -f /tmp/requirements.txt

# change user
USER rtt
WORKDIR /home/rtt

# emsdk
RUN echo EMSDK_CHECKOUT_REF=$EMSDK_CHECKOUT_REF EMSDK_CLONE_BRANCH_OR_TAG=$EMSDK_CLONE_BRANCH_OR_TAG && \
    git clone --recursive \
      ${EMSDK_CLONE_BRANCH_OR_TAG:+-b $EMSDK_CLONE_BRANCH_OR_TAG} \
      $EMSDK_CLONE_URL $EMSDK_PATH && \
    if [ -n "$EMSDK_CHECKOUT_REF" ]; then \
      cd $EMSDK_PATH && \
      git checkout $EMSDK_CHECKOUT_REF; \
    fi

RUN $EMSDK_PATH/emsdk install latest && \
    $EMSDK_PATH/emsdk activate latest
#RUN source "$EMSDK_PATH/emsdk_env.sh"
