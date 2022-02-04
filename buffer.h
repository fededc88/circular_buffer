/*
 * file: buffer.h
 *
 * A general purpose circular buffer implementation.
 * 
 * author: Federico D. Ceccarelli 
 * 
 * Do you find this project interesting or useful for your work? Please let me
 * know (fededc88@gmail.com, github:@fededc88).
 *
 * Any kind of submission are welcome!
 */

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

/*
 * Circular Buffer structure definition.
 *
 * NOTE: Structure could be moved and defined in the .c source file to prevent
 * API users to edit it and encourage the API funtions use. 
 */
struct circular_buffer_t {
    void *pBuffer;
    size_t width;
    uint16_t head;
    uint16_t tail;
    uint16_t size;
};

// Opaque circular buffer structure
typedef struct circular_buffer_t circular_buffer;

// Handle type, the way users interact with the API
typedef circular_buffer* circular_buffer_handler;

/**
 * @fn circular_buffer_init()
 *
 * @brief Allocate & Initialize a circular_buffer instance
 *
 * @param [in] pBuffer Pointer to a uint16_t type array.
 * @param [in] width Size of one element of the array
 * @param [in] size Number of elements of the pBuffer array
 *
 * @return circular_buffer_handler Pointer to a circular buffer struct instance
 *
 * The function create a circular buffer instance allocating memory dinamically
 * and return the ponter to it.
 * Users should provide the underlying char array pointer and the array size,
 * and it returns a circular buffer handle to them. This allows you to
 * statically allocated buffer.
 *
 * Systems which cannot use dynamic memory simply need to statically allocate a
 * buffer struct instance and take a look to circular_buffer_struct_init()
 * function to initialize it.
 */
circular_buffer_handler circular_buffer_init(void *pBuffer, size_t width,
                                             uint16_t size);

/**
 * @fn circular_buffer_free()
 *
 * @brief Free a circular buffer structure instance.
 * 
 * @param [in] ppcbh pointer to a circular buffer handler instance.
 * 
 * The functión free the dynamic memory space allocated for a circular buffer
 * instance. Function should only be called if the instance was inited with
 * circular_buffer_init(). Funtion does not free data buffer form memory, usser
 * is responsible for that.
 */
void * circular_buffer_free(circular_buffer_handler *ppcbh);

/**
 * @fn circular_buffer_instance_init()
 *
 * @brief Initialize a circular_buffer instance
 *
 * @param [in] pcbinst Pointer to a circular_buffer struct instance
 * @param [in] pBuffer Pointer to a uint16_t type array.
 * @param [in] width Size of one element of the array
 * @param [in] size Size of the pBuffer array
 *
 * The function initialize an statically decleared circular buffer instance 
 *
 * Users are responsable for statically allocate a buffer struct instance and
 * provide it whithin the underlying char array pointer and the array size.
 * This allows you not only statically allocated buffer but also the circular
 * buffer struct instance..
 *
 * NOTE: On embedded system is recomended to initialize the API using this
 * function.
 */
void circular_buffer_instance_init(circular_buffer *pcbinst, void *pBuffer,
                                   size_t width, uint16_t size);

/**
 * @fn circular_buffer_instance_free()
 *
 * @brief Frees the circular_buffer instance data buffer pointer.
 *
 * @param [in] pcbinst Pointer to a circular_buffer struct instance.
 *
 * Function free a circular_buffer struct instance, reset it and makes it
 * available to be re initialized with circular_buffer_instance_init(). Funtion
 * does not free data buffer from memory, usser is responsible for that.
 */
void circular_buffer_instance_free(circular_buffer* pcbinst);

/**
 * @fn circular_buffer_reset()
 *
 * @brief Reset the circular buffer to initial state, head = tail = 0
 *
 * @param [in] pcbh Pointer to a circular buffer struct instance
 */
void circular_buffer_reset(circular_buffer_handler pcbh);

/**
 * @fn circular_buffer_dump()
 *
 * @brief Dump received data and empty the circular buffer, head == tail
 *
 * @param [in] pcbh Pointer to a circular buffer struct instance
 */
void circular_buffer_dump(circular_buffer_handler pcbh);

/**
 * @fn circular_buffer_push()
 *
 * @brief Push data to the circular buffer. 
 * 
 * @param [in] pcbh Pointer to a circular buffer struct instance
 * @param [in] data Pointer to data type from where will be copied the data 
 *
 * Function push a new value to te circular buffer. If there is no more space
 * the older data will fall from the buffer. Old data is overwritten if there is
 * no more available space.
 */
void circular_buffer_push(circular_buffer_handler pcbh, void *data);

/**
 * @fn circular_buffer_put()
 *
 * @brief Put data in the circular buffer. 
 * 
 * @param [in] pcbh Pointer to a circular buffer struct instance
 * @param [in] data Pointer to data type from where will be copied the data 
 * 
 * @return 0 on succes, -1 if the buffer is full and fails
 *
 * Buffer can hold up to size-1 elements
 */
int16_t circular_buffer_put(circular_buffer_handler pcbh, void *data);

/**
 * @fn circular_buffer_pop()
 *
 * @brief Retrieve data from the circular buffer. 
 * 
 * @param [in] pcbh Pointer to a circular buffer struct instance
 * @param [in] data Pointer to data type where will be copied the retrieved data
 *
 * @return 0 on succes, -1 if the buffer is empty and fails
 *
 * Function retrieve a value from the buffer, removes it from buffer and copy it
 * to data addres.
 */
int16_t circular_buffer_pop(circular_buffer_handler pcbh, void *data);

/**
 * @fn circular_buffer_drop()
 *
 * @brief Retrieve data from the circular buffer until it got empty.
 * 
 * @param [in] pcbh Pointer to a circular buffer struct instance
 * @param [in] data Pointer to data type where will be copied the retrieved data
 * @param [in] data_length Length of data pointed array. 
 *
 * @return 0 on succes, -1 if the buffer is empty or data array isn't big enough.
 *
 * Function retrieve all values stored in the buffer, removes from it and copies
 * them to data addres.
 * Function checks that the number of element to be copied fits on data array.
 */
int16_t circular_buffer_drop(circular_buffer_handler pcbh, void *data,
                            uint16_t data_length);

/**
 * @fn circular_buffer_empty()
 *
 * @brief Checks if the circular buffer is empty
 *
 * @param [in] pcbh Pointer to a circular buffer struct instance.
 *
 * @return 1 if the buffer is empty, else return 0
 */
uint16_t circular_buffer_empty(circular_buffer_handler pcbh);

/**
 * @fn circular_buffer_count()
 *
 * @brief Counts the actual number of element in the buffer.
 *
 * @param [in] pcbh Pointer to a circular buffer struct instance.
 *
 * @return Returns the actual number of element in the buffer
 */
uint16_t circular_buffer_count(circular_buffer_handler pcbh);

/**
 * @fn circular_buffer_size()
 *
 * @brief Returns the maximum capacity of the circular buffer
 *
 * @param [in] pcbh Pointer to a circular buffer struct instance.
 *
 * @return The size of the circular buffer
 */
uint16_t circular_buffer_size(circular_buffer_handler pcbh);

#endif
//
// End of file.
//
