# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I$(INC_DIR) -g
LDFLAGS =

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = obj
BIN_DIR = bin
SHARED_DIR = shared

# Target executables
CLIENT = $(BIN_DIR)/sftp_client
SERVER = $(BIN_DIR)/sftp_server

# Source and object files for client
CLIENT_SRC_FILES = $(wildcard $(SRC_DIR)/client/*.c) $(wildcard $(SRC_DIR)/$(SHARED_DIR)/*.c)
CLIENT_OBJ_FILES = $(patsubst $(SRC_DIR)/client/%.c, $(BUILD_DIR)/client_%.o, $(wildcard $(SRC_DIR)/client/*.c)) \
                   $(patsubst $(SRC_DIR)/$(SHARED_DIR)/%.c, $(BUILD_DIR)/shared_%.o, $(wildcard $(SRC_DIR)/$(SHARED_DIR)/*.c))

# Source and object files for server
SERVER_SRC_FILES = $(wildcard $(SRC_DIR)/server/*.c) $(wildcard $(SRC_DIR)/$(SHARED_DIR)/*.c)
SERVER_OBJ_FILES = $(patsubst $(SRC_DIR)/server/%.c, $(BUILD_DIR)/server_%.o, $(wildcard $(SRC_DIR)/server/*.c)) \
                   $(patsubst $(SRC_DIR)/$(SHARED_DIR)/%.c, $(BUILD_DIR)/shared_%.o, $(wildcard $(SRC_DIR)/$(SHARED_DIR)/*.c))

# Header files for client, server, and shared
CLIENT_HEADERS = $(wildcard $(INC_DIR)/client/*.h) $(wildcard $(INC_DIR)/$(SHARED_DIR)/*.h)
SERVER_HEADERS = $(wildcard $(INC_DIR)/server/*.h) $(wildcard $(INC_DIR)/$(SHARED_DIR)/*.h)

# Default target
all: $(CLIENT) $(SERVER)

# Rule to build the client program
$(CLIENT): $(CLIENT_OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CLIENT_OBJ_FILES) -o $(CLIENT) $(LDFLAGS)

# Rule to build the server program
$(SERVER): $(SERVER_OBJ_FILES) | $(BIN_DIR)
	$(CC) $(SERVER_OBJ_FILES) -o $(SERVER) $(LDFLAGS)

# Rule to build object files for the client
$(BUILD_DIR)/client_%.o: $(SRC_DIR)/client/%.c $(CLIENT_HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR)/client -I$(INC_DIR)/$(SHARED_DIR) -c $< -o $@

# Rule to build object files for the server
$(BUILD_DIR)/server_%.o: $(SRC_DIR)/server/%.c $(SERVER_HEADERS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR)/server -I$(INC_DIR)/$(SHARED_DIR) -c $< -o $@

# Rule to build object files for shared sources
$(BUILD_DIR)/shared_%.o: $(SRC_DIR)/$(SHARED_DIR)/%.c $(wildcard $(INC_DIR)/$(SHARED_DIR)/*.h) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR)/$(SHARED_DIR) -c $< -o $@

# Create directories if they don't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up build and binary files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
