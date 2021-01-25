#include ".\app_cfg.h"

void app_logic_realy_task(void)
{
   if(StoRunParameter.DeviceStatusWord&Power_ON)
   {
       mde_relay_on(RELAY_1,20);
   }
   else
   {
       mde_relay_off(RELAY_1,0);
   }
   mde_relay_task();
}
//------------------------------E N D-------------------------------------------




