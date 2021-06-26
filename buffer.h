
struct circular_buffer_t {
    char *pBuffer;
    uint16_t head;
    uint16_t tail;
    uint16_t size;
};

// Opaque circular buffer structure
typedef struct circular_buffer_t circular_buffer;
