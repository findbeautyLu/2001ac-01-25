//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int main (void)
{
    mde_IWDG_Configure();
    mde_systemClock_cfg();
    pbc_createTask_one(app_general_task,0);
//-----------------------------------------------------------------------------
    pbc_sysTickTaskProcess();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++