/*
 * Copyright (C) 2010 William Davy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/*
 * AsyncIOSerial.c
 *
 *  Created on: 9 Apr 2010
 *      Author: William Davy
 */

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "AsyncIOSerial.h"
/*---------------------------------------------------------------------------*/

/* See 'man termios' for more details on configuring the serial port. */

long lAsyncIOSerialOpen( const char *pcDevice, int *piDeviceDescriptor )
{
int iSerialDevice = 0;
struct termios xTerminalSettings;
long lReturn = pdFALSE;

	/* Open the port that was passed in. */
	iSerialDevice = open( pcDevice, O_RDWR | O_NOCTTY | O_NONBLOCK );
	if ( iSerialDevice < 0 )
	{
		lReturn = pdFALSE;
	}
	else
	{
		/* Grab a copy of the current terminal settings. */
		if ( 0 == tcgetattr( iSerialDevice, &xTerminalSettings ) )
		{
			/* Configure the port to the Hard-coded values. */
			cfmakeraw( &xTerminalSettings );
			cfsetspeed( &xTerminalSettings, B38400 );
			/* Set the terminal settings. */
			lReturn = ( 0 == tcsetattr(iSerialDevice, TCSANOW, &xTerminalSettings) );

			/* Pass out the device descriptor for subsequent calls to AsyncIORegisterCallback() */
			*piDeviceDescriptor = iSerialDevice;
		}
	}

	return lReturn;
}
 /*---------------------------------------------------------------------------*/

/* Define a callback function which is called when data is available. */
void vAsyncSerialIODataAvailableISR( int iFileDescriptor, void *pContext )
{
BaseType_t xHigherPriorityTaskWoken = pdFALSE;
ssize_t iReadResult = -1;
unsigned char ucRx;

	/* This handler only processes a single byte/character at a time. */
	iReadResult = read( iFileDescriptor, &ucRx, 1 );
	if ( 1 == iReadResult )
	{
		if ( NULL != pContext )
		{
			/* Send the received byte to the queue. */
			if ( pdTRUE != xQueueSendFromISR( (QueueHandle_t)pContext, &ucRx, &xHigherPriorityTaskWoken ) )
			{
				/* the queue is full. */
			}
		}
	}
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}
/*---------------------------------------------------------------------------*/
