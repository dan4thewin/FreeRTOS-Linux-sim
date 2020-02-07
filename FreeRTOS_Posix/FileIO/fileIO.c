/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#include <stdio.h>
#include <string.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo program include files. */
#include "fileIO.h"

void vDisplayMessage( const char * const pcMessageToPrint )
{
	vTaskSuspendAll();
	{
		printf( "%s", pcMessageToPrint );
		fflush( stdout );
	}
	xTaskResumeAll();
}
/*-----------------------------------------------------------*/

void vWriteMessageToDisk( const char * const pcMessage )
{
const portCHAR * const pcFileName = "RTOSlog.txt";
const portCHAR * const pcSeparator = "\r\n-----------------------\r\n";
FILE *pf;

	vTaskSuspendAll();
	{
		pf = fopen( pcFileName, "a" );
		if( pf != NULL )
		{
			fwrite( pcMessage, strlen( pcMessage ), ( unsigned portSHORT ) 1, pf );
			fwrite( pcSeparator, strlen( pcSeparator ), ( unsigned portSHORT ) 1, pf );
			fclose( pf );
		}
	}
	xTaskResumeAll();
}
/*-----------------------------------------------------------*/

void vWriteBufferToDisk( const char * const pcBuffer, unsigned long ulBufferLength )
{
const portCHAR * const pcFileName = "trace.bin";
FILE *pf;

	vTaskSuspendAll();
	{
		pf = fopen( pcFileName, "wb" );
		if( pf )
		{
			fwrite( pcBuffer, ( size_t ) ulBufferLength, ( unsigned portSHORT ) 1, pf );
			fclose( pf );
		}
	}
	xTaskResumeAll();
}

