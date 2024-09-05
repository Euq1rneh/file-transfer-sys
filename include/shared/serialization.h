#include <stdint.h>
#include <stdio.h>

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
/// @return the serialized value
uint8_t serialize_int(int value, uint8_t *buffer);


/// @brief Deserializes an int 
/// @param in the value to deserialize
/// @return the deserialized int value
int deserialize_int(uint8_t *in);

size_t serialize_message();

size_t deserialize_message();

/// @brief Serializes a file chunk
/// @param chunk chunk to serialize
/// @param buffer output buffer
/// @return returns the size of the output buffer
size_t serialize_chunk(Chunk *chunk, unsigned char *buffer);

/// @brief Deserializes a file chunk
/// @param buffer buffer to deserialize
/// @param chunk output chunk
/// @return returns the size of the deserialized data
size_t deserialize_chunk(unsigned char *buffer, Chunk chunk);