MBEDTLS_VERSION=2.16.5
DOCKER_IMAGE_NAME=mbedtls-compat-sgx-$(MBEDTLS_VERSION)
LIB_BIN_DIR=mbedtls_SGX-$(MBEDTLS_VERSION)
LIB_INSTALL_PATH=/usr/src/app/build/$(LIB_BIN_DIR)
LOCAL_INSTALL_PATH=./build

image:
	@echo "Building container $(DOCKER_IMAGE_NAME)..."
	@docker build -t $(DOCKER_IMAGE_NAME) .
	@echo "Done!"
lib: image
	@$(eval CONTAINER_ID=$(shell docker run -d $(DOCKER_IMAGE_NAME)))
	@echo "Saving library on path: $(LOCAL_INSTALL_PATH)/$(LIB_BIN_DIR)"
	@mkdir -p $(LOCAL_INSTALL_PATH) && rm -rf $(LOCAL_INSTALL_PATH)/$(LIB_BIN_DIR) && docker cp $(CONTAINER_ID):$(LIB_INSTALL_PATH) $(LOCAL_INSTALL_PATH)
	@docker stop $(CONTAINER_ID) > /dev/null
	@docker rm $(CONTAINER_ID) > /dev/null
clean:
	@docker rmi $(DOCKER_IMAGE_NAME)
	@rm -rf $(LOCAL_INSTALL_PATH)/$(LIB_BIN_DIR)
