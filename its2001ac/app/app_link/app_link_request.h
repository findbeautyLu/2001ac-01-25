#ifndef _APP_LINK_REQUEST_H
#define _APP_LINK_REQUEST_H


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//interface list  4
1:扫描任务:调度器使用         
void app_link_request_task(uint8_t solidNum);  

2:发送激活请求:注册和同步模式使用
bool app_link_request_transmit_activation_request(uint8_t solidNum);

3:发送结果获取::注册和同步模式使用 
typedef enum
{
    TRANSMIT_RESULT_FAIL = 0x00, //发送结果失败
    TRANSMIT_RESULT_DOING,       //发送中
    TRANSMIT_RESULT_SUCCESS,     //发送结果成功
}transmitResult_t;
transmitResult_t app_link_request_get_transmit_result(solidNum);

4:复位静默时间:调度器使用
void app_link_request_reset_silence_time((uint8_t solidNum););
--------------------------------E N D-----------------------------------------*/


//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
//序号: 1  
//名称: app_link_request_task
//功能: 链路请求主任务，调度器调用
//入口: 无                           
//出口: 无
void app_link_request_task(void);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
//序号: 2 
//名称: app_link_request_transmit_activation_request
//功能: 发送激活请求
//入口: 无                           
//出口: bool true 激活成功
bool app_link_request_transmit_activation_request(uint8_t solidNum);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
typedef enum
{
    TRANSMIT_RESULT_FAIL = 0x00, //发送结果失败
    TRANSMIT_RESULT_DOING,       //发送中
    TRANSMIT_RESULT_SUCCESS,     //发送结果成功
}transmitResult_t;
//序号: 3
//名称: app_link_get_transmit_result
//功能: 获取发送结果
//入口: 无                           
//出口: 结果状态
transmitResult_t app_link_request_get_transmit_result(uint8_t solidNum);
//------------------------------E N D-------------------------------------------



//++++++++++++++++++++++++++++++start+++++++++++++++++++++++++++++++++++++++++*/
//序号: 4 
//名称: app_link_request_reset_silence_time(void)
//功能: 重置静默时间
//入口: 无                           
//出口: 无
void app_link_request_reset_silence_time(uint8_t solidNum);
//------------------------------E N D-------------------------------------------

#endif



//--------------------------APP.RTC.h--END--------------------------------------