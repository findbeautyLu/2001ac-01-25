//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "chip_select.h"
#ifndef snail_data_types
    #include ".\snail_data_types.h"
#endif
//------------------------------------------------------------------------------
#if defined(stm32f103rctb)
#include ".\bsp_bough_link_stm32f103rctb.h"
#elif defined(stm8l052r8t6)

#elif defined(hc32l136k8ta)
#include ".\bsp_bough_link_hc32l136k8ta.h"
#elif defined(hc32l176kata)
#include ".\bsp_bough_link_hc32l176kata.h"
#endif
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
