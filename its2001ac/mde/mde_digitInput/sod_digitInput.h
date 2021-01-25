//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _SOD_SWITCHIN_H
#define _SOD_SWITCHIN_H
//-----------------------------------------------------------------------------
#include ".\depend\bsp_digitInput.h"
#include <stdint.h>
#include <stdbool.h>
//-----------------------------------------------------------------------------
#define max_solid         4

digit_parameter_def  digitInut[max_solid];

void mde_digit_input_solid_cfg(void)
{
//-----------------------------------------------------------------------------
    bsp_switch_in_config();
    digitInut[0].phyReadInputStatsu = BSP_switch1_pull_statsu;
    digitInut[1].phyReadInputStatsu = BSP_switch2_pull_statsu;
    digitInut[2].phyReadInputStatsu = BSP_switch3_pull_statsu;
    digitInut[3].phyReadInputStatsu = BSP_switch4_pull_statsu;
//-----------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
#endif