//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _APP_BOUGHUPGRADEPROTOCOL_H
#define _APP_BOUGHUPGRADEPROTOCOL_H
//------------------------------------------------------------------------------
#define  PROTOTOL_UPGRADE 0XD002
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Task
void app_upgrade_easy_task(void);
//------------------------------------------------------------------------------
//Event
//------------------------------------------------------------------------------
//Status
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Function
void Bough_EasyUpgrade_Protocol(bgk_comm_buff_def* _In_pReceiveData,sdt_int8u* _In_TheAddr);
//------------------------------------------------------------------------------
#endif
//++++++++++++++++++++++++++++++++++End+++++++++++++++++++++++++++++++++++++++++