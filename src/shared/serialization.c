#include "serialization.h"

void *deserialize(void *data, StructTypes expected_Type)
{
    switch (expected_Type)
    {
    case ST_CHUNK:
        Chunk *chunk;
        deserialize_chunk(data, chunk);

        return chunk;
        break;
    case ST_MESSAGE:
        deserialize_message();
        break;
    }
}

void serialize_int(int value, uint8_t *buffer)
{
    buffer[0] = (value >> 24) & 0xFF;
    buffer[1] = (value >> 16) & 0xFF;
    buffer[2] = (value >> 8) & 0xFF;
    buffer[3] = value & 0xFF;
}

size_t serialize_message()
{
    return -1;
}

size_t serialize_chunk(Chunk *chunk, unsigned char *buffer)
{
    // STEPS FOR SERIALIZATION
    // 1. serialize each of the fields separatly (for Chunks: serialize 2 integer like values, the chunk of data itself is already in bytes)
    // 2. join them all in a buffer

    // serialize index
    uint8_t s_index;
    // serialize size
    uint8_t s_size[8];

    serialize_int(chunk->index, s_index);
    serialize_uint64(chunk->size, s_size);
    // allocate memory for buffer (size will the the sizeof index + size + size of chunk array(which contains size elements))

    buffer = malloc(sizeof(uint8_t) * 9 + sizeof(unsigned char) * chunk->size); // space for the serialized index and size values and for the serialized chunk
    if (buffer == -1)
    {
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }
}

void serialize_uint64(uint64_t value, uint8_t *buffer)
{
    buffer[0] = (value >> 56) & 0xFF;
    buffer[1] = (value >> 48) & 0xFF;
    buffer[2] = (value >> 40) & 0xFF;
    buffer[3] = (value >> 32) & 0xFF;
    buffer[4] = (value >> 24) & 0xFF;
    buffer[5] = (value >> 16) & 0xFF;
    buffer[6] = (value >> 8) & 0xFF;
    buffer[7] = value & 0xFF;
}

uint64_t deserialize_uint64(const uint8_t *buffer, int start_index)
{
    uint64_t value = 0;

    value |= ((uint64_t)buffer[start_index] << 56);
    value |= ((uint64_t)buffer[start_index + 1] << 48);
    value |= ((uint64_t)buffer[start_index + 2] << 40);
    value |= ((uint64_t)buffer[start_index + 3] << 32);
    value |= ((uint64_t)buffer[start_index + 4] << 24);
    value |= ((uint64_t)buffer[start_index + 5] << 16);
    value |= ((uint64_t)buffer[start_index + 6] << 8);
    value |= (uint64_t)buffer[start_index + 7];
    return value;
}

int deserialize_int(uint8_t *in)
{
    return (in[0] << 24) |
           (in[1] << 16) |
           (in[2] << 8) |
           in[3];
}

size_t deserialize_message()
{
    return -1;
}

size_t deserialize_chunk(unsigned char *buffer, Chunk *chunk)
{
    //buffer 
    //malloc(sizeof(uint8_t) * 9 + sizeof(unsigned char) * chunk->size);
    chunk = malloc(sizeof(Chunk));

    if(chunk == -1){
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }

    int start_index = 0;
    chunk->index = deserialize_int(buffer);
    start_index +=4;
    chunk->size = deserialize_uint64(buffer, start_index);
    start_index +=8; // 12
    unsigned char *c = malloc(sizeof(unsigned char) * chunk->size);

    if (c == -1){
        fprintf(stderr, "Error allocating memory\n");
        exit(-1);
    }

    if(copy_to_from(c, buffer, start_index, chunk->size) == -1){
        fprintf(stderr, "Error retrueving chunk from serialized message\n");
        return -1;
    }
    // return value is it chunk->size or the size of the buffer?????
    return chunk->size;
}
