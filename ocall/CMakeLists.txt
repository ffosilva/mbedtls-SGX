set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SGX_COMMON_CFLAGS} -fPIC -Wno-attributes")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -std=c++11")

add_library(${PROJECT_NAME}_u STATIC net_sockets_ocalls.c std_ocalls.c)

include_directories(
    ${PROJECT_SOURCE_DIR}/trusted/config
    ${PROJECT_SOURCE_DIR}/trusted/include
    ${MBEDTLS_SRC_DIR}/library
    ${MBEDTLS_SRC_DIR}/include
    ${SGX_PATH}/include)

install(TARGETS ${PROJECT_NAME}_u
    DESTINATION lib
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
