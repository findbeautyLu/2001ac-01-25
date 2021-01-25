//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#define MAX_MODBUS_NUM    2
void app_modbus_scheduler_task(void)
{
    mde_rtu_master_slave_task();
   // app_modbus_syn_task();     //同步任务
    app_modbus_regedit_task(); //注册任务 
    ms_link_comm_data_def  pReceiveData;
    sdt_bool ReceiveOneMessageFlag;
    uint8_t i = 0;
    for(i = 0; i < MAX_MODBUS_NUM;i++)
    {
        pReceiveData = mde_pull_msRtu_onemessage(i,&ReceiveOneMessageFlag);
        if(ReceiveOneMessageFlag)
        {//收到一帧数据
            if(pReceiveData.ProcotolType == PROTOL_SYN) 
            {//同步帧
                app_master_slave_dispose_syn_data(i,&pReceiveData);
            }   
            else if(pReceiveData.ProcotolType == PROTOL_REGEDIT) 
            {//注册帧
                app_master_slave_dispose_regedit_data(i,&pReceiveData);
            }                             
        } 
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++