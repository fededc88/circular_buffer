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

circular_buffer_handler circular_buffer_init(uint8_t *pBuffer, uint16_t size){

    circular_buffer_handler pcbh;

    assert(pBuffer && size);

    pcbh = malloc(sizeof(circular_buffer_t));

    assert(pcbh);

    circular_buffer_struct_init(pcbh, uint8_t* buffer, size);

    circular_bufer_reser(pcbh);

    return pcbh;

}

void circular_buffer_free(circular_buffer_handler pcbh)
{
    free(pcbh);
}

// User provides struct
void circular_buffer_struct_init(circular_buffer* pcbuff, uint8_t *pBuffer, uint16_t size);
{
    	pcbuff->buffer = pBuffer;
	pcbuff->size = size;
	circular_buffer_reset(pcbuff);
}


// User provides struct
void circular_buffer_struct_free(circular_buffer* pcbuff, uint8_t *pBuffer, uint16_t size);
{
    	pcbuff->buffer = NULL;
	pcbuff->size = 0;
	circular_buffer_reset(pcbuff);
}

void circular_buffer_reset(circular_buffer_handler pcbh)
{
    pcbh->head = 0;
    pcbh->tail = 0;

}

void circular_buffer_dump(circular_buffer_handler pcbh)
{
    assert(pcbh);

    pcbh->head = pcbh->tail;
}


/// Put version 1 continues to add data if the buffer is full
/// Old data is overwritten
void circular_buffer_push(circular_buffer_handler pcbh, uint8_t data)
{
    assert(pcbh && pcbh->pBuffer);

    pcbh->pBuffer[pcbh->head] = data;

    forward_pointer(cbuf);

}

/// Put Version 2 rejects new data if the buffer is full
/// Returns 0 on success, -1 if buffer is full
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


/// Retrieve a value from the buffer & removes it from buffer
/// Returns 0 on success, -1 if the buffer is empty
int8_t circular_buf_pop(circular_buffer_handler pcbh, uint8_t * data)
{
    int8_t rc;

    assert(pcbh && data && pcbh->buffer);

    if(!circular_buffer_empty(cbuf))
    {
	rc = 0;
        *data = pcbh->pBuffer[pcbh->tail];
        rewind_pointer(cbuf);
    }

    return rc;
}

/// Retrive all the buffer elementes from the buffer & removes it from buffer
/// Returns 0 on success, -1 if the buffer is empty
int8_t circular_buf_drop(circular_buffer_handler pcbh, uint8_t * data, uint16_t data_length){

    int8_t rc = -1;

    assert(pcbh && data && pcbh->buffer);

    if (circular_buffer_count(pcbh) <= data_length)
    {
	rc = 0;
	while(!circular_buffer_empty(pcbh)){
	    circular_buffer_pop(pcbh, data++);
	}
    }

    return rc;
}

/// Returns true if the buffer is empty
uint8_t circular_buffer_empty(cbufr_buffer_handler pcbh)
{
    uint8_t rc = -1;

    assert(pcbh);

    if (pcbh->head == pcbh->tail)
	return 1;
    else
	return 0;
 
}


// Returns the actual number of element in the buffer
uint16_t circular_buffer_count(circular_buffer_hablder pcbh)
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

/// Returns the maximum capacity of the buffer
uint16_t circular_buffer_size(circular_buffer_handler pcbf)
{
    return pcbf->size;
}


