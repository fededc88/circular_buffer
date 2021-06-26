
#include <assert.h>
#include <stdint.h>

// Opaque circular buffer structure
typedef struct circular_buffer_t circular_buffer;

// Handle type, the way users interact with the API
typedef circular_buffer* circular_buffer_handler;

/**
 * @fn circular_buffer_init()
 *
 * @brief Initialize a circular_buffer instance
 *
 * @arg [in] pBuffer Pointer to a uint8_t type array.
 * @arg [in] size Size of the pBuffer array
 *
 * @return circular_buffer_handler Pointer to a circular buffer struct instance
 *
 * The function create a circular buffer instance allocatin memory dinamically
 * and return the ponter to it.
 * Users should provide the underlying char array and array size, and we
 * return a circular buffer handle to them. This allows you to statically allocated buffer.
 *
 * Systems which cannot use dynamic memory simply need to statically allocate a
 * buffer struct instance and take a look to  circular_buffer_struct_init()
 * function.
 */
circular_buffer_handler circular_buffer_init(uint8_t* buffer, size_t size);

// User provides struct
void circular_buffer_struct_init(circular_buffer* cbuf, uint8_t* buffer, 
	size_t size);

/// Free a circular buffer structure.
/// Does not free data buffer; owner is responsible for that
void circular_buffer_free(circular_buffer_handler pcbh);

/// Reset the circular buffer to empty, head = tail = 0
void circular_buffer_reset(circular_buffer_handler pcbh);

// Dump received data and empty, head == tail
void circular_buffer_dump(circular_buffer_handler pcbh);

/// Put version 1 continues to add data if the buffer is full
/// Old data is overwritten
void circular_buffer_push(circular_buffer_hanlder pcbh, uint8_t data);

/// Put Version 2 rejects new data if the buffer is full
/// Returns 0 on success, -1 if buffer is full
uint8_t circular_buf_put(cbuf_handle_t cbuf, uint8_t data);

/// Retrieve a value from the buffer & removes it from buffer
/// Returns 0 on success, -1 if the buffer is empty
int8_t circular_buf_pop(circular_buffer_handler pcbh, uint8_t * data);

/// Retrive al the buffer elementes from the buffer & removes it from buffer
/// Returns 0 on success, -1 if the buffer is empty
int8_t circular_buf_drop(circular_buffer_handler pcbh, uint8_t * data);

/// Returns 0 if the buffer is empty, else return buffer count
uint16_t circular_buferf_empty(cbufr_buffer_handler pcbh);

// Returns the actual number of element in the buffer
uint16_t circular_buffer_count(circular_bufer_handler pcbh);

// Returns the maximum capacity of the buffer
size_t circular_buffer_size(circular_buffer_handler pcbh);
