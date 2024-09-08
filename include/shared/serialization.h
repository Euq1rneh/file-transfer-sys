#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>


#include "structures.h"
#include "s_utils.h"

/// @brief Deserializes data to its expected type
/// @param data data to deserialize
/// @param expected_type expected type (Message or Chunk)
/// @return deserialized data or NULL in case of error
void *deserialize(void * data, StructTypes expected_type);

/// @brief Serializes an int
/// @param value int to serialize
/// @param buffer buffer to write the serialized value
void serialize_int(int value, uint8_t *buffer);


/// @brief Deserializes an int 
/// @param in the value to deserialize
/// @return the deserialized int value
int deserialize_int(uint8_t *in);

size_t serialize_message(Message *msg, char **buffer, size_t message_size);

size_t deserialize_message(unsigned char *data, Message *msg);

/// @brief Serializes a file chunk
/// @param chunk chunk to serialize
/// @param buffer output buffer
/// @return returns the size of the output buffer
void serialize_chunk(struct Chunk *chunk, char **buffer, size_t *total_size);

/// @brief Deserializes a file chunk
/// @param buffer buffer to deserialize
/// @param chunk output chunk
/// @return returns the size of the deserialized data
size_t deserialize_chunk(struct Chunk *chunk, char *buffer);