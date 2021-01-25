//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "..\app_cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void app_link_scheduler_task(void)
{
    app_link_syn_task();     //同步任务
    mde_bough_link_task();
    app_link_log_task();//注册任务 
    app_link_request_task();//激活任务
	app_upgrade_easy_task();//升级任务
    uint8_t i = 0;
    for(; i < MAX_MODBUS_NUM; i++)
    {
        if(pull_bough_recevie_one_message(i))
        {
            bgk_comm_buff_def *ptRev;
            ptRev = pull_bough_message_pBuff(i);
            app_link_request_reset_silence_time(i);
            switch(ptRev->ProcotolType)
            {
                case PROTOL_CHECK_CONFICT:
				{
					break;
				}
                case PROTOL_SYN:
                {
                    app_link_syn_push_receive_data(i,ptRev);
                    break;
                }                 
                case PROTOL_REGEDIT:
                {
                    app_link_log_push_receive_data(i,ptRev);
                    break;
                }  
				case PROTOTOL_UPGRADE:
                {
                    Bough_EasyUpgrade_Protocol(ptRev,app_pull_local_id());
                    break;
                }  
                default:break;
            }
        }
    }     
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++