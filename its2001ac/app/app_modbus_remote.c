//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\app_cfg.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//name: 应用task
//-----------------------------------------------------------------------------
void app_modbus_remote_task(void)
{
  
    static sdt_bool cfged = sdt_false;
    
    if(cfged)
    {
        mde_mRtu_task();
        
        sdt_int16u reg_addr,reg_length;
        mRtu_status_def rd_stauts = mRtuS_none;
        sdt_int16u reg_detailes;
        
        rd_stauts = pull_mRtu_register(MODBUS_REMOTE,&reg_addr,&reg_length);
        if(mRtuS_read == rd_stauts)
        {
            while(reg_length)
            {
                reg_detailes = app_modbus_read_reg_data(reg_addr);           
                if(push_mRtu_readReg(MODBUS_REMOTE,reg_addr,reg_detailes))
                {
                    reg_addr++;
                    reg_length--;
                }
                else
                {
                    break;
                }
            }
            mRtu_answer_event(MODBUS_REMOTE);
        }
        else if(mRtuS_write == rd_stauts)
        {
            while(reg_length)
            {
                sdt_int16u rd_wReg_details;

                if(pull_mRtu_writeReg(MODBUS_REMOTE,reg_addr,&rd_wReg_details))
                {
                    app_modebus_write_reg_data(reg_addr,rd_wReg_details);
                    reg_addr++;
                    reg_length--;
                }
                else
                {
                    break;
                }
            }
            mRtu_answer_event(MODBUS_REMOTE);
        }
        else if(mRtuS_rwBoth == rd_stauts)
        {
            sdt_int16u w_reg_addr,w_reg_length;
            pull_mRtu_register_wb(MODBUS_REMOTE,&w_reg_addr,&w_reg_length);

            while(w_reg_length)
            {
                sdt_int16u rd_wReg_details;
                if(pull_mRtu_writeReg(MODBUS_REMOTE,w_reg_addr,&rd_wReg_details))
                {
                    app_modebus_write_reg_data(reg_addr,rd_wReg_details);
                    w_reg_addr++;
                    w_reg_length--;
                }
                else
                {
                    break;
                }
            }
            while(reg_length)
            {
                reg_detailes = app_modbus_read_reg_data(reg_addr);
                if(push_mRtu_readReg(MODBUS_REMOTE,reg_addr,reg_detailes))
                {             
                    reg_addr++;
                    reg_length --;
                }
                else
                {
                    break;
                } 
            }
            mRtu_answer_event(MODBUS_REMOTE);
        }      
    }
    else
    {
        cfged = sdt_true;
        mRtu_parameter_def parameter;
        parameter.mRtu_address = 0x01;
        parameter.mRtu_baudrate = 9600;
        parameter.mRtu_parity = mRtu_parity_none;
        parameter.mRtu_stopBits = mRtu_stopBits_one;
        parameter.mRtu_sysFrequency = 72000000;
        set_mRtu_parameter(MODBUS_REMOTE,&parameter);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++