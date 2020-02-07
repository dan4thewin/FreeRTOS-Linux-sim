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
 * AsyncIO.h
 *
 *  Created on: 9 Apr 2010
 *      Author: William Davy
 */

#ifndef ASYNCIO_H_
#define ASYNCIO_H_

/**
 * Used to register a callback function for notification when an asynchronous IO action is completed.
 * @param iFileDescriptor The File descriptor used for the asynchronous IO.
 * @param pvFunction The callback function that receives the file descriptor and pvContext when the IO completes.
 * @param pvContext A caller supplied parameter for the pvFunction.
 * @return TRUE iff there is a valid pvFunction to register.
 */
long lAsyncIORegisterCallback( int iFileDescriptor, void (*pvFunction)(int,void*), void *pvContext );

/**
 * Removes the registered call back from the list.
 * @param iFileDescriptor The file descriptor for an already closed file handle.
 */
void vAsyncIOUnregisterCallback( int iFileDescriptor );


#endif /* ASYNCIO_H_ */
