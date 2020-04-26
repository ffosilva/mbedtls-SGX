FROM ffosilva/sgx:sgx_2.9.1

ARG COMPILE_EXAMPLES=YES
ARG SGX_HW=YES
ARG SGX_MODE=Debug

ADD . ./

RUN mkdir build \
 && cd build \
 && cmake .. -DCOMPILE_EXAMPLES=${COMPILE_EXAMPLES} \
             -DSGX_MODE=${SGX_MODE} \
             -DSGX_HW=${SGX_HW} \
 && make -j$(nproc) install
