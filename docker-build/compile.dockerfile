#------Base docker image------
ARG UBUNTU_VERSION=20.04
FROM ubuntu:${UBUNTU_VERSION} as base

ARG DEBIAN_FRONTEND=noninteractive
ONBUILD ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get --yes install apt-utils=2.0.* --no-install-recommends && \
    ln -snf "/usr/share/zoneinfo/$TZ" "/etc/localtime" && echo "Europe/London" > "/etc/timezone" && \
    apt-get update && apt-get --yes install sudo=1.8.* --no-install-recommends && echo "%sudo ALL=(ALL) NOPASSWD: ALL" >> "/etc/sudoers" && \
    apt-get update && apt-get install --yes --no-install-recommends ca-certificates && /usr/sbin/update-ca-certificates && rm -rf /var/lib/apt/lists/*

#------Compile docker image------
FROM base as compile

# Installing all packages required for compilation
RUN apt-get update && apt-get install --yes build-essential=12.* lcov=1.14-2 --no-install-recommends && \
    apt-get update && apt-get install --yes --no-install-recommends \
    git=1:2.* \
    wget=1.20.3-1ubuntu1 \
    curl=7.* \
    unzip=6.* \
    flex=2.6.* \
    bison=2:3.* \
    gperf=3.* \
    python3=3.8.* \
    python3-pip=20.* \
    python3-setuptools=45.* \
    python3-venv=3.8.* \
    cmake=3.16.* \
    ninja-build=1.10.* \
    pkg-config=0.29.* \
    ccache=3.7.* \
    libffi-dev=3.* \
    libssl-dev=1.1.1* \
    dfu-util=0.9-1 \
    libusb-1.0-0=2:1.0.* \
    zip=3.0-11build1 \
    dos2unix \
    clang-12=1:12.0.0-3ubuntu1~20.04.5  \
    clang-tools-12=1:12.0.0-3ubuntu1~20.04.5 \
    clang-tidy-12=1:12.0.0-3ubuntu1~20.04.5 \
    clang-format-12=1:12.0.0-3ubuntu1~20.04.5 && \
    pip install peru==1.3.1 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /modules

# the only purpose of git init is to be able to apply git patches later
RUN git init

COPY peru.yaml /modules
COPY app/misc/ble-nimble-patch-1.patch /modules/app/misc/ble-nimble-patch-1.patch

RUN peru sync && cd app/externals && unzip esp-idf.zip && mv esp-idf-v4.4.3 esp-idf
RUN cd app/externals/esp-idf/components/bt/host/nimble/nimble && git apply /modules/app/misc/ble-nimble-patch-1.patch

# Jenkins user UID
ARG UID
ARG USER_TO_USE_FOR_RUN

RUN useradd --create-home --home-dir=/home/$USER_TO_USE_FOR_RUN --shell /bin/bash --uid ${UID} ${USER_TO_USE_FOR_RUN}

USER ${USER_TO_USE_FOR_RUN}

WORKDIR /modules/app/externals/esp-idf
RUN ./install.sh && rm -rf /var/lib/apt/lists/*
# to avoid detected dubious ownership in repository error
RUN git config --global --add safe.directory /modules/app/externals/esp-idf

WORKDIR /home/${USER_TO_USE_FOR_RUN}

CMD [ "echo", "entry script from docker-compose.yml"]
