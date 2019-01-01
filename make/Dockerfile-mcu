FROM ubuntu:18.10
ARG ESPIDF_VERSION=v3.3-beta1
ARG TOOLCHAIN_VERSION=1.22.0-80-g6c4433a-5.2.0
ENV ESPIDF_VERSION ${ESPIDF_VERSION}
ENV TOOLCHAIN_VERSION ${TOOLCHAIN_VERSION}
RUN apt-get update && \ 
    apt-get install -y --no-install-recommends \
      gcc \
      git \
      make \
      wget \
      libncurses-dev \
      flex \
      bison \
      gperf \
      python-pip \
      python-setuptools \
      python-serial \
      python-cryptography \
      python-future \
      python-pyparsing \
      ca-certificates && \
    apt-get autoremove -yqq && \
    apt-get autoclean -yqq && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* /var/cache/apt && \
    mkdir -p /esp && \
    cd /esp && \
    wget https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-$TOOLCHAIN_VERSION.tar.gz && \
    tar -xzf xtensa-esp32-elf-linux64-$TOOLCHAIN_VERSION.tar.gz && \
    rm xtensa-esp32-elf-linux64-$TOOLCHAIN_VERSION.tar.gz && \
    git clone --depth 1 --recursive -b $ESPIDF_VERSION https://github.com/espressif/esp-idf.git && \
    python -m pip install --user -r /esp/esp-idf/requirements.txt && \
    rm -rf esp-idf/.git
ENV PATH /esp/xtensa-esp32-elf/bin:$PATH
ENV IDF_PATH /esp/esp-idf