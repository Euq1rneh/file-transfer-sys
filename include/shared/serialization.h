#include <stdint.h>
#include <stdio.h>

/// @brief Serializes an int
/// @param value int to serialize
/// @return the serialized value
uint8_t serialize_int(int value);


/// @brief Deserializes an int 
/// @param in the value to deserialize
/// @return the deserialized int value
int deserialize_int(uint8_t *in);