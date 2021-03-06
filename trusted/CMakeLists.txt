set(MBEDTLS_SGX_SRC_INCLUDE_PATH ${MBEDTLS_SRC_DIR}/include)
set(MBEDTLS_SGX_SRC_LIB_PATH ${MBEDTLS_SRC_DIR}/library)

FILE(GLOB MBEDTLS_SGX_SRC_FILES ${MBEDTLS_SRC_DIR}/library/*.c)
FILE(GLOB MBEDTLS_SGX_SRC_HEADERS ${MBEDTLS_SGX_SRC_INCLUDE_PATH}/mbedtls/*.h)
FILE(GLOB MBEDTLS_SGX_COMPAT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_custom_command(
    OUTPUT ${PROJECT_NAME}_t.c ${PROJECT_NAME}_t.h
    COMMAND ${SGX_EDGER8R} --trusted ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.edl
    --search-path ${SGX_PATH}/include
)

# remove default AESNI implementation in favor of alternate one
list(REMOVE_ITEM MBEDTLS_SGX_SRC_FILES ${MBEDTLS_SRC_DIR}/library/aesni.c)

add_library(${PROJECT_NAME}_t STATIC ${MBEDTLS_SGX_SRC_FILES} ${PROJECT_NAME}_t.c aesni_alt.c threading_alt.c mbedtls_compat_sgx.c dummy.c net_sockets_proxy.c)

# Enable network errors handling by error.c
set_source_files_properties(${MBEDTLS_SRC_DIR}/library/error.c PROPERTIES COMPILE_FLAGS -DMBEDTLS_NET_C)

list(REMOVE_ITEM MBEDTLS_SGX_SRC_HEADERS ${MBEDTLS_SGX_SRC_INCLUDE_PATH}/mbedtls/config.h)

install(FILES ${MBEDTLS_SGX_SRC_HEADERS}
    DESTINATION include/mbedtls
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${MBEDTLS_SGX_SRC_INCLUDE_PATH}
    ${MBEDTLS_SGX_SRC_LIB_PATH}
    ${SGX_PATH}/include
    ${SGX_PATH}/include/tlibc
    ${SGX_PATH}/include/stlport
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SGX_COMMON_CFLAGS} -nostdinc -fvisibility=hidden -fpie -fstack-protector")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -std=c++03 -nostdinc++")

target_link_libraries(${PROJECT_NAME}_t "${SGX_COMMON_CFLAGS} \
    -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L${SGX_LIBRARY_PATH} \
    -Wl,--whole-archive -l${SGX_TRTS_LIB} -Wl,--no-whole-archive \
    -Wl,--start-group ${LIB_MBEDTLS_PATH} -lsgx_tstdc -lsgx_tstdcxx -lsgx_tcrypto -l${SGX_TSVC_LIB} -Wl,--end-group \
    -Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
    -Wl,-pie,-eenclave_entry -Wl,--export-dynamic \
    -Wl,--defsym,__ImageBase=0")

install(TARGETS ${PROJECT_NAME}_t
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

install(FILES ${PROJECT_NAME}.edl
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

install(FILES ${MBEDTLS_SGX_COMPAT_HEADERS}
    DESTINATION include/mbedtls
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

install(FILES config/config-sgx.h
    DESTINATION include/mbedtls
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    RENAME config.h)
