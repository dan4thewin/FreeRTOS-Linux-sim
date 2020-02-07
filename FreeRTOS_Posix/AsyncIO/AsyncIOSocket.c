/*
 * Copyright (C) 2009 William Davy
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
 * AsyncIOSocket.c
 *
 *  Created on: 21 Sep 2009
 *      Author: William Davy
 */

#define _GNU_SOURCE

#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "AsyncIO.h"
#include "AsyncIOSocket.h"
/*---------------------------------------------------------------------------*/

int iSocketOpenUDP( void (*vSocketCallback)( int, void * ), void *pvContext, struct sockaddr_in *pxBindAddress )
{
int iSocket = 0;

	taskENTER_CRITICAL();
	{
		/* Open a new socket. */
		iSocket = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
		if ( 0 != iSocket )
		{
			/* Have we been passed a call back function that will deal with received messages? */
			if ( pdTRUE == lAsyncIORegisterCallback( iSocket, vSocketCallback, pvContext ) )
			{
				/* This is UDP so bind it passed listen address. */
				if ( NULL != pxBindAddress )
				{
					if ( 0 != bind( iSocket, ( struct sockaddr *)pxBindAddress, sizeof( struct sockaddr_in ) ) )
					{
						printf("Bind error: %d\n", errno );
					}
				}
			}
			else
			{
				/* Socket is being used as polled IO or for sending data. */
			}
		}
		else
		{
			printf( "Failed to open socket: %d.\n", errno );
		}
	}
	taskEXIT_CRITICAL();
	return iSocket;
}
/*---------------------------------------------------------------------------*/

void vSocketClose( int iSocket )
{
	close( iSocket );
	vAsyncIOUnregisterCallback( iSocket );
}
/*---------------------------------------------------------------------------*/

int iSocketUDPSendTo( int iSocket, xUDPPacket *pxPacket, struct sockaddr_in *pxSendAddress )
{
int iBytesSent = 0;
	if ( ( 0 != iSocket ) && ( NULL != pxPacket ) )
	{
		taskENTER_CRITICAL();
			iBytesSent = sendto( iSocket, pxPacket, sizeof( xUDPPacket ), 0, ( struct sockaddr *)pxSendAddress, sizeof( struct sockaddr_in ) );
		taskEXIT_CRITICAL();
	}
	return iBytesSent;
}
/*---------------------------------------------------------------------------*/

int iSocketUDPReceiveISR( int iSocket, xUDPPacket *pxPacket, struct sockaddr_in *pxReceiveAddress )
{
int iBytesReceived = 0;
socklen_t xSocketLength = sizeof( struct sockaddr_in );

	if ( 0 != iSocket )
	{
		iBytesReceived = recvfrom( iSocket, pxPacket, sizeof( xUDPPacket ), 0, ( struct sockaddr *)pxReceiveAddress, &xSocketLength );
		pxPacket->ucNull = 0;
	}
	return iBytesReceived;
}
/*---------------------------------------------------------------------------*/

int iSocketUDPReceiveFrom( int iSocket, xUDPPacket *pxPacket, struct sockaddr_in *pxReceiveAddress )
{
int iBytesReceived = 0;
socklen_t xSocketLength = sizeof( struct sockaddr_in );

	if ( 0 != iSocket )
	{
		taskENTER_CRITICAL();
			iBytesReceived = recvfrom( iSocket, pxPacket, sizeof( xUDPPacket ), 0, ( struct sockaddr *)pxReceiveAddress, &xSocketLength );
			pxPacket->ucNull = 0;
		taskEXIT_CRITICAL();
	}
	return iBytesReceived;
}
/*---------------------------------------------------------------------------*/

void vUDPReceiveAndDeliverCallback( int iSocket, void *pvContext )
{
BaseType_t xHigherTaskWoken = pdFALSE;
static xUDPPacket xPacket;
struct sockaddr_in xReceiveAddress;

	if ( sizeof( xUDPPacket ) == iSocketUDPReceiveISR( iSocket, &xPacket, &xReceiveAddress ) )
	{
		if ( pdPASS != xQueueSendFromISR( (QueueHandle_t)pvContext, &xPacket, &xHigherTaskWoken ) )
		{
			printf( "UDP Rx failed\n" );
		}
	}
	else
	{
		printf( "UDP Rx failed: %d\n", errno );
	}
	portEND_SWITCHING_ISR( xHigherTaskWoken );
}
/*-----------------------------------------------------------*/
