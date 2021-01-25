//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "stm32f10x.h"
#include "intrinsics.h"
#include ".\BSP_Steppermotor.h"

//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#define FIRST_PHASE_A_DATA    T_BIT20 
//#define FIRST_PHASE_B_DATA    T_BIT21
//#define FIRST_PHASE_C_DATA    T_BIT22
//#define FIRST_PHASE_D_DATA    T_BIT23  
//
//#define SECOND_PHASE_A_DATA    T_BIT16 
//#define SECOND_PHASE_B_DATA    T_BIT17
//#define SECOND_PHASE_C_DATA    T_BIT18 
//#define SECOND_PHASE_D_DATA    T_BIT19   
//
//#define THIRD_PHASE_A_DATA    T_BIT28 
//#define THIRD_PHASE_B_DATA    T_BIT29 
//#define THIRD_PHASE_C_DATA    T_BIT30 
//#define THIRD_PHASE_D_DATA    T_BIT31  
//
//#define FOUR_PHASE_A_DATA    T_BIT24 
//#define FOUR_PHASE_B_DATA    T_BIT25 
//#define FOUR_PHASE_C_DATA    T_BIT26
//#define FOUR_PHASE_D_DATA    T_BIT27  

#define FIRST_PHASE_A_DATA    20 //J33  ×î×ó±ß
#define FIRST_PHASE_B_DATA    21
#define FIRST_PHASE_C_DATA    22
#define FIRST_PHASE_D_DATA    23  

#define SECOND_PHASE_A_DATA    16 
#define SECOND_PHASE_B_DATA    17
#define SECOND_PHASE_C_DATA    18 
#define SECOND_PHASE_D_DATA    19   

#define THIRD_PHASE_A_DATA    28 
#define THIRD_PHASE_B_DATA    29 
#define THIRD_PHASE_C_DATA    30 
#define THIRD_PHASE_D_DATA    31  

#define FOUR_PHASE_A_DATA    24 //j29 ×îÓÒ±ß
#define FOUR_PHASE_B_DATA    25 
#define FOUR_PHASE_C_DATA    26
#define FOUR_PHASE_D_DATA    27  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void BSP_StepperMotor_Configure(void)
{
     
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void firstPhase_a_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= FIRST_PHASE_A_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FIRST_PHASE_A_DATA,true);
}
void firstPhase_b_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= FIRST_PHASE_B_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FIRST_PHASE_B_DATA,true);
}
void firstPhase_c_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= FIRST_PHASE_C_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FIRST_PHASE_C_DATA,true);
}
void firstPhase_d_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= FIRST_PHASE_D_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FIRST_PHASE_D_DATA,true);
}
void firstPhase_a_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~FIRST_PHASE_A_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FIRST_PHASE_A_DATA,false);
}
void firstPhase_b_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~FIRST_PHASE_B_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FIRST_PHASE_B_DATA,false);
}
void firstPhase_c_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~FIRST_PHASE_C_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FIRST_PHASE_C_DATA,false);
}
void firstPhase_d_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~FIRST_PHASE_D_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FIRST_PHASE_D_DATA,false);
}

void secondPhase_a_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= SECOND_PHASE_A_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(SECOND_PHASE_A_DATA,true);
}
void secondPhase_b_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= SECOND_PHASE_B_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(SECOND_PHASE_B_DATA,true);
}
void secondPhase_c_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= SECOND_PHASE_C_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(SECOND_PHASE_C_DATA,true);
}
void secondPhase_d_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= SECOND_PHASE_D_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(SECOND_PHASE_D_DATA,true);
}
void secondPhase_a_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~SECOND_PHASE_A_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(SECOND_PHASE_A_DATA,false);
}
void secondPhase_b_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~SECOND_PHASE_B_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(SECOND_PHASE_B_DATA,false);
}
void secondPhase_c_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~SECOND_PHASE_C_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(SECOND_PHASE_C_DATA,false);
}
void secondPhase_d_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~SECOND_PHASE_D_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(SECOND_PHASE_D_DATA,false);
}


void thirdPhase_a_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= THIRD_PHASE_A_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(THIRD_PHASE_A_DATA,true);
}
void thirdPhase_b_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= THIRD_PHASE_B_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(THIRD_PHASE_B_DATA,true);
}
void thirdPhase_c_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= THIRD_PHASE_C_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(THIRD_PHASE_C_DATA,true);
}
void thirdPhase_d_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= THIRD_PHASE_D_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(THIRD_PHASE_D_DATA,true);
}
void thirdPhase_a_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~THIRD_PHASE_A_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(THIRD_PHASE_A_DATA,false);
}
void thirdPhase_b_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~THIRD_PHASE_B_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(THIRD_PHASE_B_DATA,false);
}
void thirdPhase_c_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~THIRD_PHASE_C_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(THIRD_PHASE_C_DATA,false);
}
void thirdPhase_d_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~THIRD_PHASE_D_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(THIRD_PHASE_D_DATA,false);
}

void fourPhase_a_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= FOUR_PHASE_A_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FOUR_PHASE_A_DATA,true);
}
void fourPhase_b_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= FOUR_PHASE_B_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FOUR_PHASE_B_DATA,true);
}
void fourPhase_c_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= FOUR_PHASE_C_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FOUR_PHASE_C_DATA,true);
}
void fourPhase_d_enable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData |= FOUR_PHASE_D_DATA;
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FOUR_PHASE_D_DATA,true);
}
void fourPhase_a_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~FOUR_PHASE_A_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FOUR_PHASE_A_DATA,false);
}
void fourPhase_b_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~FOUR_PHASE_B_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FOUR_PHASE_B_DATA,false);
}
void fourPhase_c_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~FOUR_PHASE_C_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FOUR_PHASE_C_DATA,false);
}
void fourPhase_d_disable(void)
{
//    uint32_t tempData = BSP_pll_hc595_data();
//    tempData &= (~FOUR_PHASE_D_DATA);
//    BSP_push_hc595_data(tempData);
    mde_hc59c_push_status(FOUR_PHASE_D_DATA,false);
}
void BSP_FirstMotor_Output(StepperOutLevel_Def _In_OutLevel)
{
    switch(_In_OutLevel)
    {
        case StepM_Stop:
        {
            firstPhase_a_disable();
            firstPhase_b_disable();
            firstPhase_c_disable();
            firstPhase_d_disable();
            break;
        }
        case StepM_00:
        {
            firstPhase_a_enable();
            firstPhase_b_disable();
            firstPhase_c_disable();
            firstPhase_d_enable();
            break;
        }
        case StepM_01:
        {
            firstPhase_a_disable();
            firstPhase_b_disable();
            firstPhase_c_disable();
            firstPhase_d_enable();
            break;
        }
        case StepM_02:
        {
            firstPhase_a_disable();
            firstPhase_b_disable();
            firstPhase_c_enable();
            firstPhase_d_enable();
            break;
        }
        case StepM_03:
        {
            firstPhase_a_disable();
            firstPhase_b_disable();
            firstPhase_c_enable();
            firstPhase_d_disable();
            break;
        }
        case StepM_04:
        {
            firstPhase_a_disable();
            firstPhase_b_enable();
            firstPhase_c_enable();
            firstPhase_d_disable();
            break;
        }
        case StepM_05:
        {
            firstPhase_a_disable();
            firstPhase_b_enable();
            firstPhase_c_disable();
            firstPhase_d_disable();
            break;
        }
        case StepM_06:
        {
            firstPhase_a_enable();
            firstPhase_b_enable();
            firstPhase_c_disable();
            firstPhase_d_disable();
            break;
        }
        case StepM_07:
        {
            firstPhase_a_enable();
            firstPhase_b_disable();
            firstPhase_c_disable();
            firstPhase_d_disable();
            break;
        }
        default:
        {
            break;
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void BSP_SecondMotor_Output(StepperOutLevel_Def _In_OutLevel)
{
    switch(_In_OutLevel)
    {
        case StepM_Stop:
        {
            secondPhase_a_disable();
            secondPhase_b_disable();
            secondPhase_c_disable();
            secondPhase_d_disable();

            break;
        }
        case StepM_00:
        {
            secondPhase_a_enable();
            secondPhase_b_disable();
            secondPhase_c_disable();
            secondPhase_d_enable();
            break;
        }
        case StepM_01:
        {
            secondPhase_a_disable();
            secondPhase_b_disable();
            secondPhase_c_disable();
            secondPhase_d_enable();
          
            break;
        }
        case StepM_02:
        {
            secondPhase_a_disable();
            secondPhase_b_disable();
            secondPhase_c_enable();
            secondPhase_d_enable();
          
            break;
        }
        case StepM_03:
        {
            secondPhase_a_disable();
            secondPhase_b_disable();
            secondPhase_c_enable();
            secondPhase_d_disable();
            
            break;
        }
        case StepM_04:
        {
            secondPhase_a_disable();
            secondPhase_b_enable();
            secondPhase_c_enable();
            secondPhase_d_disable();
            break;
        }
        case StepM_05:
        {
            secondPhase_a_disable();
            secondPhase_b_enable();
            secondPhase_c_disable();
            secondPhase_d_disable();
            break;
        }
        case StepM_06:
        {
            secondPhase_a_enable();
            secondPhase_b_enable();
            secondPhase_c_disable();
            secondPhase_d_disable();
            break;
        }
        case StepM_07:
        {
            secondPhase_a_enable();
            secondPhase_b_disable();
            secondPhase_c_disable();
            secondPhase_d_disable();
            break;
        }
        default:
        {
            break;
        }
    }
}

void BSP_ThirdMotor_Output(StepperOutLevel_Def _In_OutLevel)
{
    switch(_In_OutLevel)
    {
        case StepM_Stop:
        {
            thirdPhase_a_disable();
            thirdPhase_b_disable();
            thirdPhase_c_disable();
            thirdPhase_d_disable();
            break;
        }
        case StepM_00:
        {
            thirdPhase_a_enable();
            thirdPhase_b_disable();
            thirdPhase_c_disable();
            thirdPhase_d_enable();
            break;
        }
        case StepM_01:
        {
            thirdPhase_a_disable();
            thirdPhase_b_disable();
            thirdPhase_c_disable();
            thirdPhase_d_enable();       
            break;
        }
        case StepM_02:
        {
            thirdPhase_a_disable();
            thirdPhase_b_disable();
            thirdPhase_c_enable();
            thirdPhase_d_enable();
          
            break;
        }
        case StepM_03:
        {
            thirdPhase_a_disable();
            thirdPhase_b_disable();
            thirdPhase_c_enable();
            thirdPhase_d_disable();
            
            break;
        }
        case StepM_04:
        {
            thirdPhase_a_disable();
            thirdPhase_b_enable();
            thirdPhase_c_enable();
            thirdPhase_d_disable();
            break;
        }
        case StepM_05:
        {
            thirdPhase_a_disable();
            thirdPhase_b_enable();
            thirdPhase_c_disable();
            thirdPhase_d_disable();
            break;
        }
        case StepM_06:
        {
            thirdPhase_a_enable();
            thirdPhase_b_enable();
            thirdPhase_c_disable();
            thirdPhase_d_disable();
            break;
        }
        case StepM_07:
        {
            thirdPhase_a_enable();
            thirdPhase_b_disable();
            thirdPhase_c_disable();
            thirdPhase_d_disable();
            break;
        }
        default:
        {
            break;
        }
    }
}

void BSP_FourMotor_Output(StepperOutLevel_Def _In_OutLevel)
{
    switch(_In_OutLevel)
    {
        case StepM_Stop:
        {
            fourPhase_a_disable();
            fourPhase_b_disable();
            fourPhase_c_disable();
            fourPhase_d_disable();

            break;
        }
        case StepM_00:
        {
            fourPhase_a_enable();
            fourPhase_b_disable();
            fourPhase_c_disable();
            fourPhase_d_enable();
            break;
        }
        case StepM_01:
        {
            fourPhase_a_disable();
            fourPhase_b_disable();
            fourPhase_c_disable();
            fourPhase_d_enable();
          
            break;
        }
        case StepM_02:
        {
            fourPhase_a_disable();
            fourPhase_b_disable();
            fourPhase_c_enable();
            fourPhase_d_enable();
          
            break;
        }
        case StepM_03:
        {
            fourPhase_a_disable();
            fourPhase_b_disable();
            fourPhase_c_enable();
            fourPhase_d_disable();
            
            break;
        }
        case StepM_04:
        {
            fourPhase_a_disable();
            fourPhase_b_enable();
            fourPhase_c_enable();
            fourPhase_d_disable();
            break;
        }
        case StepM_05:
        {
            fourPhase_a_disable();
            fourPhase_b_enable();
            fourPhase_c_disable();
            fourPhase_d_disable();
            break;
        }
        case StepM_06:
        {
            fourPhase_a_enable();
            fourPhase_b_enable();
            fourPhase_c_disable();
            fourPhase_d_disable();
            break;
        }
        case StepM_07:
        {
            fourPhase_a_enable();
            fourPhase_b_disable();
            fourPhase_c_disable();
            fourPhase_d_disable();
            break;
        }
        default:
        {
            break;
        }
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++