#include "buffer.h"

/*
 * Circular Buffer structure definition.
 *
 * NOTE: Stricture is defined here to prevent API users to edit it and encourage
 * the API funtions use. 
 */
struct circular_buffer_t {
    uint8_t *pBuffer;
    uint16_t head;
    uint16_t tail;
    uint16_t size;
};

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
circular_buffer_handler circular_buffer_init(uint8_t *pBuffer, uint16_t size){

    circular_buffer_handler pcbh;

    assert(pBuffer && size);

    pcbh = malloc(sizeof(circular_buffer));

    assert(pcbh);

    circular_buffer_instance_init(pcbh, pBuffer, size);

    circular_buffer_reset(pcbh);

    return pcbh;

}

/**
 * circular_buffer_free() - Free a circular buffer structure instance
 */
void circular_buffer_free(circular_buffer_handler pcbh)
{
    assert(pcbh);

    free(pcbh);
}

/**
 * circular_buffer_instance_init() - Initialize a circular_buffer instance
 */
void circular_buffer_instance_init(circular_buffer *pcbinst, uint8_t *pBuffer, uint16_t size)
{
    assert(pcbinst && pBuffer);

    if(pcbinst->pBuffer != NULL)
	return; // circular buffer instance is already initialized

    pcbinst->pBuffer = pBuffer;
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
void circular_buffer_push(circular_buffer_handler pcbh, uint8_t data)
{
    assert(pcbh && pcbh->pBuffer);

    pcbh->pBuffer[pcbh->head] = data;

    forward_pointer(pcbh);
}
/**
 * circular_buffer_put() - Put data in the circular buffer.
 */
int8_t circular_buffer_put(circular_buffer_handler pcbh, uint8_t data)
{
    int8_t rc = -1;

    assert(pcbh && pcbh->pBuffer);

    // Check if buffer is full
    if( circular_buffer_count(pcbh) < pcbh->size)
    {
	// Buffer still have space
	rc = 0;
	pcbh->pBuffer[pcbh->head] = data;
	forward_pointer(pcbh);
    }

    return rc;

}

/**
 * circular_buffer_pop() - Retrieve data from the circular buffer.
 */
int8_t circular_buf_pop(circular_buffer_handler pcbh, uint8_t * data)
{
    int8_t rc;

    assert(pcbh && data && pcbh->pBuffer);

    if(!circular_buffer_empty(pcbh))
    {
	rc = 0;
        *data = pcbh->pBuffer[pcbh->tail];
        rewind_pointer(pcbh);
    }

    return rc;
}
/**
 * circular_buffer_drop() - Retrieve data from the circular fufer until it got
 *                          empty.
 */
int8_t circular_buffer_drop(circular_buffer_handler pcbh, uint8_t * data, uint16_t data_length){

    int8_t rc = -1;

    assert(pcbh && data && pcbh->pBuffer);

    if (circular_buffer_count(pcbh) <= data_length)
    {
	rc = 0;
	while(!circular_buffer_empty(pcbh)){
	    circular_buffer_pop(pcbh, data++);
	}
    }

    return rc;
}

/**
 * circular_buffer_empty() - Checks if the circular buffer is empty
 */
uint8_t circular_buffer_empty(circular_buffer_handler pcbh)
{
    uint8_t rc = -1;

    assert(pcbh);

    if (pcbh->head == pcbh->tail)
	return 1;
    else
	return 0;
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
