/*
 * Copyright (c) 2018 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */

#ifndef DIAG_API_H
#define DIAG_API_H

#include <mqueue.h>
#include <pthread.h>

/* This file provides the prototypes for DIAG APIs that can be
 * used by an application to send and receive DIAG messages to/from QCA402X
 * devices. The APIs create a POSIX msg queue link to the iot Daemon, which
 * then interfacts with the QCA402X device via SPI or UART.
 */

/*All APIs (except diag_open) require a handle input parameter. The handle is
 * generated by the diag_open() API*/
typedef struct _handle{
    mqd_t serv_handle;   /*Message queue handle of iotD*/
    mqd_t client_handle; /*Message queue handle of client application*/
    char client_name[32]; /*Client queue name string*/
} handle_t;


/*Posix message queue parameters*/
#define QUEUE_PERMISSIONS              0660
#define MAX_MESSAGES                   10
#define MAX_MSG_SIZE                   2048
#define MSG_BUFFER_SIZE                (MAX_MSG_SIZE + 10)


/*
 * Function: diag_open() - opens the msg q to iot daemon, exchanges handshake 
 *                        messages with the daemon. It returns a handle which 
 *                       must be used in all future communications.
 * Input: name- iot daemon msg q name. Typical value is "/iotdiag"
 * Returns: valid handle if handshake successful
 *          NULL on failure. 
 */
void* diag_open(char* name);

/*
 * Function: diag_send() - sends diag frame to iot daemon via a posix msg q
 * Input: hdl- handle that was created on diag_open
 *        devID - ID of destination QCA402X device, default 0, only applicable 
 *                when more than one QCA402X devices are connected. 
 *        buf- buffer to send
 *        len- buffer size
 * Returns: 0 - success
 *         -1 - failure
 */
int diag_send(void* hdl, uint8_t devId, uint8_t* buf, int len);

/*
 * Function: diag_recv() - wait for specified time to receive a packet. 
 * Input:hdl - valid handle
 *       buf - buffer to recieve the packet
 *       len - size in bytes of provided buffer
 *       msec - wait time in milliseconds, if 0, wait until a message is available 
 * Returns: Number of bytes read on success
            -1 on error
 */
int diag_recv(void* hdl, uint8_t* buf, int len, int msec);

/*
 * Function: diag_close() - closes the connection to the iot daemon.
 * Input: hdl- valid handle
 * Returns: 0 - success
 *           1 - failure
 */
int diag_close(void* hdl);

#endif

