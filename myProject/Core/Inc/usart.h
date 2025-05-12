#ifndef _USART_H_
#define _USART_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


/**
 * Initializes the USART interface.
 */
void USART_Init(void);

/**
 * Appends a single character to the USART buffer and enables the
 * TXE interrupt so the data can be sent away.
 *
 * @param[in] c character to add
 * @return true if the character was added successfully, false otherwise
*/
bool USART_PutChar(char c);

/**
 * Appends contents of a data buffer to the USART transmit buffer and triggers transmission.
 *
 * @param[in] data pointer to the source memory buffer
 * @param[in] dataSize size (in bytes) of the data buffer
 * @return number of bytes successfully written to the USART transmit buffer
*/
size_t USART_WriteData(const void *data, size_t dataSize);

/**
 * Appends a null-terminated string to the USART transmit buffer and triggers transmission.
 *
 * @param[in] string pointer to null-terminated string to be transmitted
 * @return number of characters successfully written to the USART transmit buffer
*/
size_t USART_WriteString(const char *string);

/**
 * Pulls out a single character from the USART receive buffer.
 *
 * @param c pointer to a variable, where the read character will be stored
 * @return true if the character was pulled out successfully, false otherwise
*/
bool USART_GetChar(char *c);

/**
 * Pulls out characters from the USART buffer and stores them into a destination buffer.
 *
 * @param[out] data pointer to memory where the read characters will be stored
 * @param[in] maxSize maximum numbers of characters that can be read
 * @return number of read characters
*/
size_t USART_ReadData(void *data, size_t maxSize);


#endif // _USART_H_
