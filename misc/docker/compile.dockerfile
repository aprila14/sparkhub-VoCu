ARG UBUNTU_VERSION=18.04
ARG IMAGE_PREFIX="wizzdev"
FROM ${IMAGE_PREFIX}_base:${UBUNTU_VERSION}
ARG UID=1000
ARG USER_TO_USE_FOR_RUN=user

# Installing all packages required for compilation
RUN apt-get update && apt-get install --yes build-essential lcov
RUN apt-get update && apt-get install --yes \
git \
wget \
flex bison \
gperf \
python python-pip \
python3 python3-pip python3-setuptools python3-venv \
cmake ninja-build \
pkg-config \
ccache \
libffi-dev \
libssl-dev \
dfu-util \
libusb-1.0-0 \
zip \
libsystemd-dev \
clang-10 clang-tools-10 clang-tidy-10 clang-format-10


# Copying the source of the ESP32 IDF to build the compiler
COPY .git/ /modules/.git/
COPY app/externals/esp-idf/ /modules/app/externals/esp-idf/
# COPY app/misc/esp-idf-patch-1.patch /modules/app/misc/

# Fix one dependency - see details https://github.com/espressif/esp-idf/issues/6262
RUN sed -i '1ipython-socketio<5; python_version<="2.7"' /modules/app/externals/esp-idf/requirements.txt

# Applying esp-idf patch...
# RUN cd /modules/app/externals/esp-idf && git apply ../../misc/esp-idf-patch-1.patch

# ... create the compiler as user ${USER_TO_USE_FOR_RUN}
RUN useradd --create-home --home-dir=/home/$USER_TO_USE_FOR_RUN --shell /bin/bash --uid ${UID} ${USER_TO_USE_FOR_RUN}
RUN sudo --user=${USER_TO_USE_FOR_RUN} --set-home /modules/app/externals/esp-idf/install.sh esp32s3

RUN rm -rf /var/lib/apt/lists/*
