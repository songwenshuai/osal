/**************************************************************************************************
  Filename:       Fifo.h
  Revised:        $Date: 2014-06-16 15:12:16 -0700 (Mon, 16 Jun 2014) $
  Revision:       $Revision: 39036 $

  Description:    This file contains the OSAL Task definition and manipulation functions.
**************************************************************************************************/

#ifndef FIFO_H
#define FIFO_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * TYPEDEFS
 */

/*!
 * \brief fifo, data size is byte
 */
typedef struct bFIFO{
	uint32 size;          /*!< fifo size */
	uint32 length;        /*!< valid data length */
	uint32 intLine;       /*!< interrupt line */
	uint8  *pBuffer;      /*!< fifo buffer */
	uint8  *pWrite;       /*!< pointer to pos for writing */
	uint8  *pRead;        /*!< pointer to pos for reading */
} bFIFO_t;


/*!
 * \brief fifo, data size is word(16bit)
 */
typedef struct wFIFO{
	uint32 size;
	uint32 length;
	uint32 intLine;
	uint16 *pBuffer;
	uint16 *pWrite;
	uint16 *pRead;
} wFIFO_t;

/*********************************************************************
 * MACROS
 */

/* Memory set functions */
#define FIFO_MEMSET                      osal_memset

/* generic error */
#define ERR_SUCCESS	 			(0)         /*!< ok */
#define ERR_UNDEFINE			(-1)        /*!< unknown error */
#define ERR_PARAMETER			(-2)        /*!< invalid parameter */
#define ERR_TIME_OUT			(-3)        /*!< time is out */
#define ERR_PARITY              (-4)        /* */

/* buffer error */
#define ERR_BUFFER_INITIALIZED  (-10)       /*!< buffer is not initalized */
#define ERR_BUFFER_EMPTY        (-11)       /*!< buffer is empty */
#define ERR_BUFFER_FULL         (-12)       /*!< buffer is full */

/* macro definition */
#define NONE_IRQn	(0xff)                      /*!< invalid interrupt line */
#define bFIFO_GetLength(a)	((a)->length)       /*!< get data length in fifo */
#define wFIFO_GetLength(a)	((a)->length)       /*!< get data length in fifo */

/*********************************************************************
 * FUNCTIONS
 */

/*!
 * \brief init the fifo struct. If the interrupt line is valid, enable interrupt
 * \param [in] pfifo - fifo struct
 * \param [in] pbuffer - fifo buffer
 * \param [in] len - buffer size
 * \param [in] intLine - interrupt line. NONE_IRQn is invalid.
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 */
int bFIFO_Init(bFIFO_t *pfifo, uint8 *pbuffer, uint32 len, uint8 intLine );

/*!
 * \brief clear fifo data.
 * \param [in] pfifo - fifo struct
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 */
int bFIFO_Clear(bFIFO_t *pfifo);

/*!
 * \brief put one byte to fifo
 * \param [in] pfifo - fifo struct
 * \param [in] data - data which will be writen to fifo
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 * \retval ERR_BUFFER_FULL fifo is full.
 */
int bFIFO_PutByte(bFIFO_t *pfifo, uint8 data);

/*!
 * \brief get one byte from fifo, fifo is not changed.
 * \param [in] pfifo - fifo struct
 * \param [out] data - data
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 * \retval ERR_BUFFER_EMPTY fifo is empty
 */
int bFIFO_Glance(bFIFO_t *pfifo, uint8 *data);

/*!
 * \brief get one byte from fifo and the data will be delete
 * \param [in] pfifo - fifo struct
 * \param [out] data - data
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 * \retval ERR_BUFFER_EMPTY fifo is empty
 */
int bFIFO_GetByte(bFIFO_t *pfifo, uint8 *data);


/* fifo for word */
/*!
 * \brief init the fifo struct. If the interrupt line is valid, enable interrupt
 * \param [in] pfifo - fifo struct
 * \param [in] pbuffer - fifo buffer
 * \param [in] len - buffer size
 * \param [in] intSouce - interrupt line. NONE_IRQn is invalid.
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 */
int wFIFO_Init(wFIFO_t *pfifo, uint16 *pbuffer, uint32 len, uint8 intSouce);

/*!
 * \brief clear fifo data.
 * \param [in] pfifo - fifo struct
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 */
int wFIFO_Clear(wFIFO_t *pfifo);

/*!
 * \brief put string to fifo
 * \param [in] pfifo - fifo struct
 * \param [in] data - data which will be writen to fifo
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 * \retval ERR_BUFFER_FULL fifo is full.
 */
int wFIFO_PutByte(wFIFO_t *pfifo, uint16 data);

/*!
 * \brief get string from fifo, fifo is not changed.
 * \param [in] pfifo - fifo struct
 * \param [out] data - data
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 * \retval ERR_BUFFER_EMPTY fifo is empty
 */
int wFIFO_Glance(wFIFO_t *pfifo, uint16 *data);

/*!
 * \brief get string from fifo and the data will be delete
 * \param [in] pfifo - fifo struct
 * \param [out] data - data
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 * \retval ERR_BUFFER_EMPTY fifo is empty
 */
int wFIFO_GetByte(wFIFO_t *pfifo, uint16 *data);

/*!
 * \brief put string to fifo
 * \param [in] pfifo - fifo struct
 * \param [in] data - data which will be writen to fifo
 * \retval ERR_SUCCESS ok
 * \retval ERR_PARAMETER parameter is invalid
 * \retval ERR_BUFFER_FULL fifo is full.
 */
int bFIFO_PutString(bFIFO_t *pfifo, uint8 *data, uint16 len);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* FIFO_H */
