/******************************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PICmicro(r) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PICmicro Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
********************************************************************/

#if !defined (_DRV_COMMON_H)
#define _DRV_COMMON_H

// *****************************************************************************
/* Device Driver IO Intent

  Summary:
    Identifies the intended usage of the device when it is opened.

  Description:
    This enumeration identifies the intended usage of the device when the
    caller opens the device.  It identifies the desired behavior of the device
    driver for the following:

    * Blocking or non-blocking IO behavior (do IO calls such as read and write
      block until the operation is finished or do they return immediately and
      require the caller to call another routine to check the status of the
      operation.

    * Support reading and/or writing of data from/to the device.

    * Identify the buffering behavior (sometimes called "double buffering" of
      the driver.  Indicates if the driver should maintain its own read/write
      buffers and copy data to/from these buffers to/from the caller's buffers.

    * Identify the DMA behavior of the peripheral.

  Remarks:
    Buffer allocation method is not identified by this enumeration.  Buffers
    can be allocated statically at build time, dynamically at run time, or
    even allocated by the caller and passed to the driver for its own usage if
    a driver-specific routine is provided for such.  This choice is left to
    the design of the individual driver and is considered part of its
    interface.

    These values can be considered "flags".  One selection from each of the
    groups below can be "OR'd" together to create the complete value passed
    to the driver's open routine.
*/

typedef enum
{
    /* Read */
    DRV_IO_INTENT_READ               /*DOM-IGNORE-BEGIN*/ = 1 << 0 /* DOM-IGNORE-END*/,

    /* Write */
    DRV_IO_INTENT_WRITE              /*DOM-IGNORE-BEGIN*/ = 1 << 1 /* DOM-IGNORE-END*/,

    /* Read and Write*/
    DRV_IO_INTENT_READWRITE          /*DOM-IGNORE-BEGIN*/ \
            = DRV_IO_INTENT_READ|DRV_IO_INTENT_WRITE /* DOM-IGNORE-END*/,

    /* The driver will block and will return when the operation is complete */
    DRV_IO_INTENT_BLOCKING           /*DOM-IGNORE-BEGIN*/ = 0 << 2 /* DOM-IGNORE-END*/,

    /* The driver will return immediately */
    DRV_IO_INTENT_NONBLOCKING        /*DOM-IGNORE-BEGIN*/ = 1 << 2 /* DOM-IGNORE-END*/,

    /* The driver will support only one client at a time */
    DRV_IO_INTENT_EXCLUSIVE          /*DOM-IGNORE-BEGIN*/ = 1 << 3 /* DOM-IGNORE-END*/,

    /* The driver will support multiple clients at a time */
    DRV_IO_INTENT_SHARED             /*DOM-IGNORE-BEGIN*/ = 0 << 3 /* DOM-IGNORE-END*/

} DRV_IO_INTENT;


// *****************************************************************************
/* Driver Client Status

  Summary:
    Identifies the current status/state of a client's connection to a driver

  Description:
    This enumeration identifies the current status/state of a client's link to
    a driver.

  Remarks:
    The enumeration used as the return type for the client-level status routines
    defined by each device driver or system module (for example:
    DRV_USART_ClientStatus) must be based on the values in this enumeration.
*/

typedef enum
{
   // Indicates that a non-system defined error has occurred.
    DRV_CLIENT_STATUS_ERROR_EXTENDED   = -10,

   // An un-specified error has occurred.
    DRV_CLIENT_STATUS_ERROR            =  -1,

   // An operation is currently in progress
    DRV_CLIENT_STATUS_CLOSED           =   0,

   // An operation is currently in progress
    DRV_CLIENT_STATUS_BUSY             =   1,

    // Any previous operations have succeeded and the module is ready for
    // additional operations
    DRV_CLIENT_STATUS_READY            =   2,

   // Indicates that the module is in a non-system defined ready/run state.
    DRV_CLIENT_STATUS_READY_EXTENDED   =  10

} DRV_CLIENT_STATUS;


// *****************************************************************************
/* Device Driver Blocking Status Macro

  Summary:
    Returns if the IO intent provided is blocking

  Description:
    Returns if the IO intent provided is blocking

  Remarks:
    None.
*/

#define DRV_IO_ISBLOCKING(intent)          (intent & DRV_IOINTENT_BLOCKING)


// *****************************************************************************
/* Device Driver Non Blocking Status Macro

  Summary:
    Returns if the IO intent provided is non-blocking

  Description:
    Returns if the IO intent provided is non-blocking

  Remarks:
    None.
*/

#define DRV_IO_ISNONBLOCKING(intent)       (intent & DRV_IOINTENT_NONBLOCKING)


// *****************************************************************************
/* Device Driver Exclusive Status Macro

  Summary:
    Returns if the IO intent provided is non-blocking

  Description:
    Returns if the IO intent provided is non-blocking

  Remarks:
    None.
*/

#define DRV_IO_ISEXCLUSIVE(intent)       (intent & DRV_IO_INTENT_EXCLUSIVE)


// *****************************************************************************
/* Device Driver IO Buffer Identifier

  Summary:
    Identifies to which buffer a device operation will apply

  Description:
    This enumeration identifies to which buffer (read, write, both, or neither)
    a device operation will apply.  This is used for "flush" (or similar)
    operations.
*/

typedef enum
{
    // Operation does not apply to any buffer
    DRV_IO_BUFFER_TYPE_NONE      = 0x00,

    // Operation applies to read buffer
    DRV_IO_BUFFER_TYPE_READ      = 0x01,

    // Operation applies to write buffer
    DRV_IO_BUFFER_TYPE_WRITE     = 0x02,

    // Operation applies to both read and write buffers
    DRV_IO_BUFFER_TYPE_RW        = DRV_IO_BUFFER_TYPE_READ|DRV_IO_BUFFER_TYPE_WRITE

} DRV_IO_BUFFER_TYPES;


// *****************************************************************************
/* Device Handle

  Summary:
    Handle to an opened device driver

  Description:
    This handle identifies the an open instance of a device driver.  It must be
    passed to all other driver routines (except the init, deinit, or power
    routines) to identify the caller.

  Remarks:
    Every application or module that wishes to use a driver must first call
    the driver's open routine.  this is the only routine that is absolutely
    required for every driver.

    If a driver is unable to allow an additional module to use it, then it must
    return the special value DRV_HANDLE_INVALID.  Callers should check the
    handle returned for this value to ensure this value was not returned before
    attempting to call any other driver routines using the handle.
*/

typedef unsigned short int DRV_HANDLE;


// *****************************************************************************
/* Invalid Device Handle

 Summary:
    Invalid device handle

 Description:
    If a driver is unable to allow an additional module to use it, then it must
    return the special value DRV_HANDLE_INVALID.  Callers should check the
    handle returned for this value to ensure this value was not returned before
    attempting to call any other driver routines using the handle.

 Remarks:
    None
*/

#define DRV_HANDLE_INVALID  (((unsigned short) -1))


// *****************************************************************************
/* Not supported configuration

  Summary:
    Not supported configuration

  Description:
    If the configuration option is not supported on a instance of the
    peripheral, use this macro to equate to that configuration. This option
    should be listed as a possible value in the description of that
    configuration option.
*/

#define DRV_CONFIG_NOT_SUPPORTED                    (((unsigned short) -1))


// *****************************************************************************
/* Unsupported Attribute Abstraction

  Summary:
    Abstracts the use of the unsupported attribute defined by the compiler

  Description:
    This macro nulls the definition of the _PLIB_UNSUPPORTED macro, to support
	compilation of the drivers for all different variants. 

  Example:
    void _PLIB_UNSUPPORTED PLIB_USART_Enable(USART_MODULE_ID index);

    Will not generates a compiler error, if the interface is not defined for the
    part selected.

 Remarks:
	None
*/
    
#ifndef _PLIB_UNSUPPORTED

    #define _PLIB_UNSUPPORTED   

#endif

#endif      //_DRV_COMMON_H
