FROM ffosilva/sgx:sgx_2.11

RUN apt-get update && \
    apt-get install -y \
    cmake \
    gcc \
    g++

ADD . ./

ARG COMPILE_EXAMPLES=YES
ARG SGX_HW=YES
ARG SGX_MODE=PreRelease

RUN mkdir build \
 && cd build \
 && cmake .. -DCOMPILE_EXAMPLES=${COMPILE_EXAMPLES} \
             -DSGX_MODE=${SGX_MODE} \
             -DSGX_HW=${SGX_HW} \
 && make -j$(nproc) install
