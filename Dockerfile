FROM sebvaucher/sgx-base:sgx_2.5

ARG COMPILE_EXAMPLES=YES
ARG SGX_MODE=HW
ARG SGX_BUILD=DEBUG

ADD . ./

RUN mkdir build \
 && cd build \
 && cmake .. -DCOMPILE_EXAMPLES=${COMPILE_EXAMPLES} \
             -DSGX_MODE=${SGX_MODE} \
             -DSGX_BUILD=${SGX_BUILD} \
 && make -j$(nproc) install
