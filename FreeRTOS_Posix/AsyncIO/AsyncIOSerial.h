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
 * AsyncIOSerial.h
 *
 *  Created on: 9 Apr 2010
 *      Author: William Davy
 */

#ifndef ASYNCIOSERIAL_H_
#define ASYNCIOSERIAL_H_

/**
 * Opens and configures a serial device which exists on the path specified by pcDevice.
 * The serial is defined as a 'raw' connection with 38400 baudrate, 8 bit, 1 stopbit, no parity, no flow.
 * @param pcDevice A string path to the serial device, typically, /dev/ttyS0
 * @param piDeviceDescriptor A pointer to an int which is set to the file descriptor for the opened device.
 * @return TRUE iff the device was successfully opened and configured.
 */
long lAsyncIOSerialOpen( const char *pcDevice, int *piDeviceDescriptor );

/**
 * The 'ISR' callback function that is triggered when there is data waiting.
 * This function is intended to be registered using lAsyncIORegisterCallback().
 * It collects as many bytes as possible, delivering them one byte at a time to
 * the ( QueueHandle_t )pContext.
 * @param iFileDescriptor The file descriptor for the serial port.
 * @param pContext A Handle to a queue. Cast to QueueHandle_t.
 */
void vAsyncSerialIODataAvailableISR( int iFileDescriptor, void *pContext );

#endif /* ASYNCIOSERIAL_H_ */
