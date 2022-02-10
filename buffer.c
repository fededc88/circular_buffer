/*
 * file: buffer.c
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

#include "buffer.h"

#include <string.h>

/* local functions prototype declaration */
static void forward_pointer(circular_buffer_handler pcbh);
static void rewind_pointer(circular_buffer_handler pcbh);

/**
 * forward_pointer() - Forward the buffer head pointer
 */
static void forward_pointer(circular_buffer_handler pcbh)
{
	assert(pcbh);

	if ( ++(pcbh->head) == pcbh->size)
	    pcbh->head = 0;

	if (pcbh->head == pcbh->tail)
	{
	    if(++(pcbh->tail) == pcbh->size)
		pcbh->tail = 0;
	}
}

/**
 * rewind_pointer() - rewind the tail pointer
 */
static void rewind_pointer(circular_buffer_handler pcbh)
{
	assert(pcbh);

	if ( ++(pcbh->tail) == pcbh->size)
	    pcbh->tail = 0;

}

/**
 * @circular_buffer_init() - Allocate & Initialize a circular_buffer instance
 */
circular_buffer_handler circular_buffer_init(void *pBuffer, size_t width,
                                             uint16_t size)
{
    circular_buffer_handler pcbh;

    assert(pBuffer && size);

    pcbh = malloc(sizeof(circular_buffer));

    assert(pcbh);

    circular_buffer_instance_init(pcbh, pBuffer, width, size);

    return pcbh;
}

/**
 * circular_buffer_free() - Free a circular buffer structure instance
 */
void * circular_buffer_free(circular_buffer_handler *ppcbh)
{
    assert(ppcbh);

    free(*ppcbh);
    *ppcbh = NULL; 

    return *ppcbh;
}

/**
 * circular_buffer_instance_init() - Initialize a circular_buffer instance
 */
void circular_buffer_instance_init(circular_buffer *pcbinst, void *pBuffer,
        size_t width, uint16_t size)
{
    assert(pcbinst && pBuffer);

    if(pcbinst->pBuffer != NULL)
	return; // circular buffer instance is already initialized

    pcbinst->pBuffer = pBuffer;
    pcbinst->width = width;
    pcbinst->size = size;
    circular_buffer_reset(pcbinst);
}

/**
 * circular_buffer_instance_free() - Frees the circular_buffer instance data 
 *  				     buffer pointer.
 */
void circular_buffer_instance_free(circular_buffer* pcbinst)
{
    assert(pcbinst);

    pcbinst->pBuffer = NULL;
    pcbinst->width = 0;
    pcbinst->size = 0;
    circular_buffer_reset(pcbinst);
}

/**
 * circular_buffer_reset() - Reset te circular buffer to initial state,
 *                           head = tail = 0
 */
void circular_buffer_reset(circular_buffer_handler pcbh)
{
    pcbh->head = 0;
    pcbh->tail = 0;

}

/**
 * circular_buffer_dump() - Dump received data and empty the circular buffer,
 *                          head == tail
 */
void circular_buffer_dump(circular_buffer_handler pcbh)
{
    assert(pcbh);

    pcbh->head = pcbh->tail;
}

/**
 * circular_buffer_push() - Push data to the circular buffer.
 */
void circular_buffer_push(circular_buffer_handler pcbh, void *data)
{
    void *s1;

    assert(pcbh && pcbh->pBuffer && data);
 
    s1 = (pcbh->pBuffer + (pcbh->head * pcbh->width));
    memcpy(s1, data, pcbh->width);

    forward_pointer(pcbh);
}

/**
 * circular_buffer_put() - Put data in the circular buffer.
 */
int16_t circular_buffer_put(circular_buffer_handler pcbh, void *data)
{
    int16_t rc = -1;

    assert(pcbh && pcbh->pBuffer && data);

    // Check if buffer is full
    if( circular_buffer_count(pcbh) < (pcbh->size-1))
    {
        // Buffer still have space
        rc = 0;
        circular_buffer_push(pcbh, data);
    }

    return rc;
}

/**
 * circular_buffer_pop() - Retrieve data from the circular buffer.
 */
int16_t circular_buffer_pop(circular_buffer_handler pcbh, void *data)
{
    void *s2;
    int16_t rc = -1;

    assert(pcbh && data && pcbh->pBuffer && data);

    if(!circular_buffer_empty(pcbh))
    {
        rc = 0;
        s2 = pcbh->pBuffer + (pcbh->tail * pcbh->width);
        memcpy(data, s2, pcbh->width);
        rewind_pointer(pcbh);
    }

    return rc;
}
/**
 * circular_buffer_drop() - Retrieve data from the circular buffer until it got
 *                          empty.
 */
int16_t circular_buffer_drop(circular_buffer_handler pcbh, void *data,
                             uint16_t data_length)
{
    int16_t rc = -1;

    assert(pcbh && data && pcbh->pBuffer);

    if (circular_buffer_count(pcbh) <= data_length)
    {
        rc = 0;
        while(!circular_buffer_empty(pcbh))
        {
            circular_buffer_pop(pcbh, data);
            data += pcbh->width;
        }
    }

    return rc;
}

/**
 * circular_buffer_empty() - Checks if the circular buffer is empty
 */
uint16_t circular_buffer_empty(circular_buffer_handler pcbh)
{
    uint16_t rc = 0;

    assert(pcbh);

    if (pcbh->head == pcbh->tail)
       rc = 1;

    return rc;
}

/**
 * circular_buffer_count() - Counts the actual number of elemnts in the buffer
 */
uint16_t circular_buffer_count(circular_buffer_handler pcbh)
{
    uint16_t count = 0;

    assert(pcbh);

    if (circular_buffer_empty(pcbh))
        return count;

    if(pcbh->head > pcbh->tail)
        count = (pcbh->head - pcbh->tail);
    else
        count = (pcbh->size + pcbh->head - pcbh->tail);

    return count;
} 

/**
 * circular_buffer_size() - Returns the maximum capacity of the buffer
 */
uint16_t circular_buffer_size(circular_buffer_handler pcbf)
{
    return pcbf->size;
}

//
// End of file.
//
