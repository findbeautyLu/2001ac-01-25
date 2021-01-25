#include "..\app_cfg.h"
#include "stdlib.h"

typedef enum
{
    LINK_REQUEST_MAIN_STATUS_IDLE    = 0x00, //·¢ËÍ½á¹ûÊ§°Ü
    LINK_REQUEST_MAIN_STATUS_SILENCE ,       //¾²Ä¬
    LINK_REQUEST_MAIN_STATUS_READ_RESULLT ,       
}linkRequestMainStatus_t;


typedef struct
{    
    
    linkRequestMainStatus_t   main_status;     
    timerClock_def            silence_time; 
    timerClock_def            timeout;   
    transmitResult_t          transmitResult;
}linkRequest_t;

linkRequest_t linkRequest[MAX_MODBUS_NUM];

void debug_task(void);
bool app_link_request_transmit_activation_request(uint8_t solidNum)
{
    if((LINK_REQUEST_MAIN_STATUS_IDLE == linkRequest[solidNum].main_status) &&\
      (!pull_bough_link_is_busy(solidNum)))
    {
        linkRequest[solidNum].transmitResult = TRANSMIT_RESULT_DOING;
        linkRequest[solidNum].main_status = LINK_REQUEST_MAIN_STATUS_READ_RESULLT;
        pbc_reload_timerClock(&linkRequest[solidNum].timeout,1000);
        bgk_comm_buff_def *pt = pull_bough_message_pBuff(solidNum);    
        pt->ProcotolType = 0xD020;
        pbc_int16uToArray_bigEndian(rand(),&pt->LinkDstAddr[0]);
        pbc_int16uToArray_bigEndian(rand(),&pt->LinkDstAddr[2]);
        pbc_int16uToArray_bigEndian(rand(),&pt->LinkDstAddr[4]); 
        
        uint8_t *local_addr;
        local_addr = app_pull_local_id();
        pt->LinkSrcAddr[0] = local_addr[0];
        pt->LinkSrcAddr[1] = local_addr[1];
        pt->LinkSrcAddr[2] = local_addr[2];
        pt->LinkSrcAddr[3] = local_addr[3];
        pt->LinkSrcAddr[4] = local_addr[4];
        pt->LinkSrcAddr[5] = local_addr[5];                       
        pt->PayloadLength = 0;
        push_active_one_message_transmit(solidNum,true);
        return true;
    }
    return false;
}

transmitResult_t app_link_request_get_transmit_result(uint8_t solidNum)
{
    return linkRequest[solidNum].transmitResult;   
}

void app_link_request_reset_silence_time(uint8_t solidNum)
{
    pbc_reload_timerClock(&linkRequest[solidNum].silence_time,10); 
    linkRequest[solidNum].main_status = LINK_REQUEST_MAIN_STATUS_SILENCE;
}

void app_link_request_task(void)
{
    static bool cfg[MAX_MODBUS_NUM] = false;
    uint8_t i = 0;   
 //   debug_task();
    for(;i < MAX_MODBUS_NUM; i++)
    {
        if(cfg[i])
        {
            pbc_timerMillRun_task(&linkRequest[i].silence_time);     
            pbc_timerMillRun_task(&linkRequest[i].timeout);  
            switch(linkRequest[i].main_status)
            {
                case LINK_REQUEST_MAIN_STATUS_IDLE:break;                  
                case LINK_REQUEST_MAIN_STATUS_SILENCE:
                {
                    pbc_timerMillRun_task(&linkRequest[i].silence_time);
                    if(pbc_pull_timerIsCompleted(&linkRequest[i].silence_time))
                    {
                        linkRequest[i].main_status = LINK_REQUEST_MAIN_STATUS_IDLE;                          
                    }  
                    break;
                }
                case LINK_REQUEST_MAIN_STATUS_READ_RESULLT:
                {
                    bgk_trans_mon_def newStatus = pull_bough_transmit_monitor(i);
                    if(bgk_trans_mon_none != newStatus)
                    {
                        if(bgk_trans_mon_complete == newStatus)
                        {
                            linkRequest[i].transmitResult = TRANSMIT_RESULT_SUCCESS;
                            linkRequest[i].main_status    = LINK_REQUEST_MAIN_STATUS_IDLE;
                        }
                        else if(bgk_trans_mon_error == newStatus)
                        {
                            linkRequest[i].transmitResult = TRANSMIT_RESULT_FAIL;
                            linkRequest[i].main_status    = LINK_REQUEST_MAIN_STATUS_IDLE;
                        }
                        else if(bgk_trans_mon_conflict == newStatus)
                        {
                            linkRequest[i].transmitResult = TRANSMIT_RESULT_FAIL;
                            app_link_request_reset_silence_time(i);                             
                        }                          
                    }            
                    if(pbc_pull_timerIsCompleted(&linkRequest[i].timeout))
                    {
                        linkRequest[i].transmitResult = TRANSMIT_RESULT_FAIL;
                        linkRequest[i].main_status = LINK_REQUEST_MAIN_STATUS_IDLE;
                    }                      
                    break;
                }
                default:break;
            }
        }   
        else
        {
            cfg[i] = true;  
            linkRequest[i].silence_time.timStatusBits  = timerType_millisecond;
            linkRequest[i].timeout.timStatusBits       = timerType_millisecond;
            linkRequest[i].main_status = LINK_REQUEST_MAIN_STATUS_IDLE;            
        }
    }    
}

void debug_task(void)
{
    macro_createTimer(debugTime,timStatusBits_typeMillsecond,500);
    pbc_timerClockRun_task(&debugTime);
    if(pbc_pull_timerIsCompleted(&debugTime))
    {
        pbc_reload_timerClock(&debugTime,10000);
        if(app_link_request_transmit_activation_request(0))
        {
            
        }
    }  
}



