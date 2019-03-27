/****************************************************************************
**
**  Name:       hidh_api.h
**
**  Function:   this file contains HID HOST side API definitions
**
**  Copyright (c) 2002-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef HIDH_API_H
#define HIDH_API_H

#include "hiddefs.h"
#include "sdp_api.h"

/*****************************************************************************
**  Constants
*****************************************************************************/

enum {
    HID_SDP_NO_SERV_UUID = (SDP_ILLEGAL_PARAMETER+1),
    HID_SDP_MANDATORY_MISSING
};

/* Attributes mask values to be used in HID_HostAddDev API */
#define HID_VIRTUAL_CABLE        0x0001
#define HID_NORMALLY_CONNECTABLE 0x0002
#define HID_RECONN_INIT          0x0004
#define HID_SDP_DISABLE          0x0008
#define HID_BATTERY_POWER        0x0010
#define HID_REMOTE_WAKE          0x0020
#define HID_SUP_TOUT_AVLBL       0x0040
#define HID_SSR_MAX_LATENCY      0x0080
#define HID_SSR_MIN_TOUT         0x0100

#define HID_UCD_CONN             0x4000
#define HID_SEC_REQUIRED         0x8000
#define HID_ATTR_MASK_IGNORE     0

#ifndef HID_HOST_ACPT_UNKNOWN_CONN
#define HID_HOST_ACPT_UNKNOWN_CONN      FALSE
#endif

/* by default use security channel for unknown device incomming connection 
   when  HID_HOST_ACPT_UNKNOWN_CONN is enabled */
#ifndef HIDH_UNKNOWN_DEV_ATTR_MASK
#define HIDH_UNKNOWN_DEV_ATTR_MASK     HID_SEC_REQUIRED
#endif

/*****************************************************************************
**  Type Definitions
*****************************************************************************/

typedef void (tHID_HOST_SDP_CALLBACK) (UINT16 result, UINT16 attr_mask,
                                       tHID_DEV_SDP_INFO *sdp_rec );

/* HID-HOST returns the events in the following table to the application via tHID_HOST_DEV_CALLBACK
HID_HDEV_EVT_OPEN   Connected to device with Interrupt and Control Channels in OPEN state.
                                                        Data = NA
HID_HDEV_EVT_CLOSE  Connection with device is closed.   Data=reason code.
HID_HDEV_EVT_RETRYING   Lost connection is being re-connected.
                                                        Data=Retrial number
HID_HDEV_EVT_IN_REPORT  Device sent an input report     Data=Report Type pdata= pointer to BT_HDR
                                                        (GKI buffer having report data.)
HID_HDEV_EVT_HANDSHAKE  Device sent SET_REPORT          Data=Result-code pdata=NA.
HID_HDEV_EVT_VC_UNPLUG  Device sent Virtual Unplug      Data=NA. pdata=NA.
*/

enum
{
    HID_HDEV_EVT_OPEN,
    HID_HDEV_EVT_CLOSE,
    HID_HDEV_EVT_RETRYING,
    HID_HDEV_EVT_INTR_DATA,
    HID_HDEV_EVT_INTR_DATC,
    HID_HDEV_EVT_CTRL_DATA,
    HID_HDEV_EVT_CTRL_DATC,
    HID_HDEV_EVT_HANDSHAKE,
    HID_HDEV_EVT_VC_UNPLUG
};
typedef void (tHID_HOST_DEV_CALLBACK) (UINT8 dev_handle,
                                       BD_ADDR addr,
                                       UINT8 event, /* Event from HID-DEVICE. */
                                       UINT32 data, /* Integer data corresponding to the event.*/
                                       BT_HDR *p_buf ); /* Pointer data corresponding to the event. */


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**
** Function         HID_HostGetSDPRecord
**
** Description      This function reads the device SDP record.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_HostGetSDPRecord (BD_ADDR addr,
                                                 tSDP_DISCOVERY_DB *p_db,
                                                 UINT32 db_len,
                                                 tHID_HOST_SDP_CALLBACK *sdp_cback );

/*******************************************************************************
**
** Function         HID_HostRegister
**
** Description      This function registers HID-Host with lower layers.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_HostRegister (tHID_HOST_DEV_CALLBACK *dev_cback);

/*******************************************************************************
**
** Function         HID_HostDeregister
**
** Description      This function is called when the host is about power down.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_HostDeregister(void);

/*******************************************************************************
**
** Function         HID_HostFindDevHdl
**
** Description      This is called for getting device handle.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_UCDFindDevHdl ( BD_ADDR addr, UINT8 *handle);

/*******************************************************************************
**
** Function         HID_HostAddDev
**
** Description      This is called so HID-host may manage this device.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_HostAddDev (BD_ADDR addr, UINT16 attr_mask,
                                           UINT8 *handle );

/*******************************************************************************
**
** Function         HID_HostRemoveDev
**
** Description      This removes the device from list devices that host has to manage.
**
** Returns          tHID_STATUS
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_HostRemoveDev (UINT8 dev_handle );

/*******************************************************************************
**
** Function         HID_HostOpenDev
**
** Description      This function is called when the user wants to initiate a
**                  connection attempt to a device.
**
** Returns          void
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_HostOpenDev (UINT8 dev_handle );

/*******************************************************************************
**
** Function         HID_HostWriteDev
**
** Description      This function is called when the host has a report to send.
**
** Returns          void
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_HostWriteDev(UINT8 dev_handle, UINT8 t_type,
                                            UINT8 param, UINT16 data,
                                            UINT8 report_id, BT_HDR *pbuf);

/*******************************************************************************
**
** Function         HID_HostCloseDev
**
** Description      This function disconnects the device.
**
** Returns          void
**
*******************************************************************************/
HID_API extern tHID_STATUS HID_HostCloseDev(UINT8 dev_handle );

/*******************************************************************************
** Function         HID_HostInit
**
** Description      This function initializes the control block and trace variable
**
** Returns          void
*******************************************************************************/
HID_API extern void HID_HostInit(void);

/*******************************************************************************
** Function        HID_HostSetSecurityLevel
**
** Description     This function sets the security level for the devices which
**                 are marked by application as requiring security
**
** Returns         tHID_STATUS
*******************************************************************************/
HID_API extern tHID_STATUS HID_HostSetSecurityLevel( char serv_name[], UINT8 sec_lvl );

/*******************************************************************************
**
** Function         HID_HostSetTraceLevel
**
** Description      This function sets the trace level for HID Host. If called with
**                  a value of 0xFF, it simply reads the current trace level.
**
** Returns          the new (current) trace level
**
*******************************************************************************/
HID_API extern UINT8 HID_HostSetTraceLevel (UINT8 new_level);


/*******************************************************************************
**
** Function         hidh_configure_qos_settings
**
** Description      This function is a conformance test command to configure the
**                  QoS settings on the outgoing traffic on HID host control channel
**                  and interrupt channel
**
** Returns          none.
**
*******************************************************************************/
HID_API extern void hidh_configure_qos_settings(FLOW_SPEC *p_ctrl_fs, FLOW_SPEC *p_int_fs);

#ifdef __cplusplus
}
#endif

#endif  /* HIDH_API_H */
