set(MBEDTLS_SGX_SRC_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbedtls/include)

FILE(GLOB MBEDTLS_SGX_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/mbedtls/library/*.c)
FILE(GLOB MBEDTLS_SGX_SRC_HEADERS ${MBEDTLS_SGX_SRC_INCLUDE_PATH}/mbedtls/*.h)
FILE(GLOB MBEDTLS_SGX_COMPAT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_t.c ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}_t.h
    COMMAND ${SGX_EDGER8R} --trusted ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.edl
    --search-path ${SGX_SDK}/include
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${PROJECT_NAME}_t STATIC ${MBEDTLS_SGX_SRC_FILES} ${PROJECT_NAME}_t.c threading_alt.c mbedtls_compat_sgx.c dummy.c net_sockets_proxy.c)

# Enable network errors handling by error.c
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/mbedtls/library/error.c PROPERTIES COMPILE_FLAGS -DMBEDTLS_NET_C)

list(REMOVE_ITEM MBEDTLS_SGX_SRC_HEADERS ${MBEDTLS_SGX_SRC_INCLUDE_PATH}/mbedtls/config.h)

install(FILES ${MBEDTLS_SGX_SRC_HEADERS}
    DESTINATION include/mbedtls
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

include_directories(
    $(CMAKE_CURRENT_SOURCE_DIR)/config
    $(CMAKE_CURRENT_SOURCE_DIR)/include
    mbedtls/include
    ${SGX_SDK}/include
    ${SGX_SDK}/include/tlibc
    ${SGX_SDK}/include/stlport
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
