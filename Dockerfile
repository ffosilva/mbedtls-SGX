FROM sebvaucher/sgx-base:sgx_2.5

ARG COMPILE_EXAMPLES=YES
ARG SGX_MODE=HW

ADD . ./

RUN mkdir build \
 && cd build \
 && cmake .. -DCOMPILE_EXAMPLES=${COMPILE_EXAMPLES} -DSGX_MODE=${SGX_MODE} \
 && make -j$(nproc)
