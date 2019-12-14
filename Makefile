MBEDTLS_VERSION=2.18.1
DOCKER_IMAGE_NAME=mbedtls-compat-sgx-$(MBEDTLS_VERSION)
DOCKER_CONTAINER_NAME=$(DOCKER_IMAGE_NAME)-container
LIB_BIN_DIR=mbedtls_SGX-$(MBEDTLS_VERSION)
LIB_INSTALL_PATH=/usr/src/app/build/$(LIB_BIN_DIR)
LOCAL_INSTALL_PATH=./build

image:
	@docker build -t $(DOCKER_IMAGE_NAME) .
lib: image
	@docker rm -f $(DOCKER_CONTAINER_NAME) || true
	@docker run --name $(DOCKER_CONTAINER_NAME) $(DOCKER_IMAGE_NAME)
	@mkdir -p $(LOCAL_INSTALL_PATH) && rm -rf $(LOCAL_INSTALL_PATH)/$(LIB_BIN_DIR) && docker cp $(DOCKER_CONTAINER_NAME):$(LIB_INSTALL_PATH) $(LOCAL_INSTALL_PATH)
clean:
	@docker rm -f $(DOCKER_CONTAINER_NAME)
	@docker rmi $(DOCKER_IMAGE_NAME)
	@rm -rf $(LOCAL_INSTALL_PATH)/$(LIB_BIN_DIR)
