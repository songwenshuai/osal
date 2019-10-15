/**************************************************************************************************
  Filename:       Fifo.c
  Revised:        $Date: 2014-11-04 15:36:27 -0800 (Tue, 04 Nov 2014) $
  Revision:       $Revision: 40989 $

  Description:    
**************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"

#include "Fifo.h"

/*********************************************************************
 * LOCAL VARIABLES
 */
static halIntState_t intState;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/* enable the interrupt */
static void FIFO_IntEnable(uint8 intLine)
{
    if(intLine != NONE_IRQn )
    { 
		HAL_EXIT_CRITICAL_SECTION( intState );
    }   
}

/* disable the interrupt */
static void FIFO_IntDisable(uint8 intLine)
{
    if(intLine != NONE_IRQn)
    {
		HAL_ENTER_CRITICAL_SECTION( intState );
    }   
}

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

int bFIFO_Init(bFIFO_t *pfifo, uint8 *pbuffer, uint32 len, uint8 intLine)
{
	if((pfifo == NULL) || (pbuffer == NULL) || (len == 0))
	{
		return ERR_PARAMETER;
	}
	
    /* initialize the fifo */	
	pfifo->size = len;		
	pfifo->pBuffer = pbuffer;
	pfifo->intLine = intLine;
	pfifo->length = 0;	/* there is no valid data length */
	FIFO_MEMSET(pfifo->pBuffer, 0, pfifo->size);
	pfifo->pWrite = pfifo->pRead = pfifo->pBuffer; /* write head = read head = buffer head on no valid data */
	
	return ERR_SUCCESS;
}

int bFIFO_Clear(bFIFO_t *pfifo)
{
	if(pfifo == NULL)
	{
		return ERR_PARAMETER;
	}
			
	FIFO_IntDisable(pfifo->intLine);/* disable the interrupt */

    /* clear the buffer */
	pfifo->length = 0;	
	FIFO_MEMSET(pfifo->pBuffer, 0, pfifo->size);
	pfifo->pWrite = pfifo->pRead = pfifo->pBuffer;

	FIFO_IntEnable(pfifo->intLine);/* enable the interrupt */

	return ERR_SUCCESS;
}

int bFIFO_Glance(bFIFO_t *pfifo, uint8 *data)
{	
	if((pfifo == NULL) || (data == NULL))
	{
		return ERR_PARAMETER;
	}
    
    FIFO_IntDisable(pfifo->intLine); /* disable the interrupt */

    if(pfifo->length == 0) /* fifo is empty */
	{
        FIFO_IntEnable(pfifo->intLine);
		return ERR_BUFFER_EMPTY;
	}
	
	*data = *(pfifo->pRead); /* get the data, but the read head pointer is kept */
	FIFO_IntEnable(pfifo->intLine); /* enable the interrupt */

	return ERR_SUCCESS;
}

int bFIFO_GetByte(bFIFO_t *pfifo, uint8 *data)
{	
	if((pfifo == NULL) || (data == NULL))
	{
		return ERR_PARAMETER;
	}    

	FIFO_IntDisable(pfifo->intLine); /* disable the interrupt */

	if(pfifo->length == 0) /* fifo is empty */
	{
		FIFO_IntEnable(pfifo->intLine);	
		return ERR_BUFFER_EMPTY;
	}

	*data = *(pfifo->pRead);/* get the data */

	(pfifo->length)--;
	(pfifo->pRead)++;

	if(pfifo->pRead >= (pfifo->pBuffer + pfifo->size)) /* roll back */
	{
		pfifo->pRead = pfifo->pBuffer;	
	}

	FIFO_IntEnable(pfifo->intLine);/* enable the interrupt */

	return ERR_SUCCESS;
}

int bFIFO_PutByte(bFIFO_t *pfifo, uint8 data)
{
	if(pfifo == NULL)
	{
		return ERR_PARAMETER;
	}

	FIFO_IntDisable(pfifo->intLine); /* disable the interrupt */

	if(pfifo->length == pfifo->size) /* fifo is empty */
	{
		FIFO_IntEnable(pfifo->intLine);
		return ERR_BUFFER_FULL;
	}
	
	*(pfifo->pWrite) = data;

	(pfifo->length)++;
	(pfifo->pWrite)++;

	if(pfifo->pWrite >= (pfifo->pBuffer + pfifo->size)) /* buffer roll back */
	{
		pfifo->pWrite = pfifo->pBuffer;	
	}

	FIFO_IntEnable(pfifo->intLine); /* enable the interrupt */

	return ERR_SUCCESS;
}

int bFIFO_PutString(bFIFO_t *pfifo, uint8 *data, uint16 len)
{
	if(pfifo == NULL)
	{
		return ERR_PARAMETER;
	}

	FIFO_IntDisable(pfifo->intLine); /* disable the interrupt */

	if(pfifo->length+len > pfifo->size) /* fifo has no enough space */
	{
		FIFO_IntEnable(pfifo->intLine);
		return ERR_BUFFER_FULL;
	}

	while(len--)
	{
		*(pfifo->pWrite) = *data++;

		(pfifo->length)++;
		(pfifo->pWrite)++;

		if(pfifo->pWrite >= (pfifo->pBuffer + pfifo->size)) /* buffer roll back */
		{
			pfifo->pWrite = pfifo->pBuffer;	
		}
	}
	

	FIFO_IntEnable(pfifo->intLine); /* enable the interrupt */

	return ERR_SUCCESS;
}

int wFIFO_Init(wFIFO_t *pfifo, uint16 *pbuffer, uint32 len, uint8 intLine)
{
	if((pfifo == NULL) || (pbuffer == NULL) || (len == 0))
	{
		return ERR_PARAMETER;
	}

	pfifo->size = len;		
	pfifo->pBuffer = pbuffer;
	pfifo->intLine = intLine;
	pfifo->length = 0;	
	FIFO_MEMSET(pfifo->pBuffer, 0, pfifo->size);
	pfifo->pWrite = pfifo->pRead = pfifo->pBuffer;
	
	return ERR_SUCCESS;
}

int wFIFO_Clear(wFIFO_t *pfifo)
{
	if(pfifo == NULL)
	{
		return ERR_PARAMETER;
	}
			
	FIFO_IntDisable(pfifo->intLine);

	pfifo->length = 0;	
	FIFO_MEMSET(pfifo->pBuffer, 0, pfifo->size);
	pfifo->pWrite = pfifo->pRead = pfifo->pBuffer;

	FIFO_IntEnable(pfifo->intLine);

	return ERR_SUCCESS;
}

int wFIFO_Glance(wFIFO_t *pfifo, uint16 *data)
{	
	if((pfifo == NULL) || (data == NULL))
	{
		return ERR_PARAMETER;
	}

	FIFO_IntDisable(pfifo->intLine);

	if(pfifo->length == 0)
	{
		FIFO_IntEnable(pfifo->intLine);	
		return ERR_BUFFER_EMPTY;
	}

	*data = *(pfifo->pRead);

	FIFO_IntEnable(pfifo->intLine);

	return ERR_SUCCESS;
}

int wFIFO_GetByte(wFIFO_t *pfifo, uint16 *data)
{	
	if((pfifo == NULL) || (data == NULL))
	{
		return ERR_PARAMETER;
	}

	FIFO_IntDisable(pfifo->intLine);

	if(pfifo->length == 0)
	{
		FIFO_IntEnable(pfifo->intLine);	
		return ERR_BUFFER_EMPTY;
	}

	*data = *(pfifo->pRead);

	(pfifo->length)--;
	(pfifo->pRead)++;

	if(pfifo->pRead >= (pfifo->pBuffer + pfifo->size))
	{
		pfifo->pRead = pfifo->pBuffer;	
	}

	FIFO_IntEnable(pfifo->intLine);

	return ERR_SUCCESS;
}

int wFIFO_PutByte(wFIFO_t *pfifo, uint16 data)
{
	if(pfifo == NULL)
	{
		return ERR_PARAMETER;
	}

	FIFO_IntDisable(pfifo->intLine);

	if(pfifo->length == pfifo->size)
	{
		FIFO_IntEnable(pfifo->intLine);
		return ERR_BUFFER_FULL;
	}
	
	*(pfifo->pWrite) = data;

	(pfifo->length)++;
	(pfifo->pWrite)++;

	if(pfifo->pWrite >= (pfifo->pBuffer + pfifo->size))
	{
		pfifo->pWrite = pfifo->pBuffer;	
	}

	FIFO_IntEnable(pfifo->intLine);

	return ERR_SUCCESS;
}


