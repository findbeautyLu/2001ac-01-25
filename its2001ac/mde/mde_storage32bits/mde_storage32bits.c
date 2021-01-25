//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\mde_Storage32bits.h"
#include ".\depend\bsp_flashMemory.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef struct
{
    sdt_int32u storage_length_w;//字长，32bit为一个word
    sdt_int32u(*passage_flash_memory_read)(block_def in_block,sdt_int32u in_offset);
    void(*passage_flash_memory_write)(block_def in_block,sdt_int32u in_offset,sdt_int32u in_data);
    void(*passage_flash_write_complete)(void);
}block_storage_parameter_def;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//offset = 0 写入次数
//flash存储格式:写入次数(32bit offset=0)+数据+的校验(32bit)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//读取flash的内容，并生产checksum
//-----------------------------------------------------------------------------
static sdt_int32u make_flash_memory_checksum(block_def in_block_num,block_storage_parameter_def* in_block_SP)
{
    sdt_int32u checksum = 0;
    sdt_int32u offset = 0;
    sdt_int32u length_w;
    
    length_w = in_block_SP->storage_length_w + 1;
    while(length_w)
    {
        checksum += in_block_SP->passage_flash_memory_read(in_block_num,offset);
        length_w --;
        offset ++;
    }
    return(checksum ^ 0x34927c5a);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
static sdt_int32u make_ram_memory_checksum(sdt_int32u in_wr_cnt,sdt_int32u* in_pRam,sdt_int32u in_length_w)
{
    sdt_int32u checksum = 0;
    sdt_int32u offset = 0;
    sdt_int32u length_w;
    
    checksum += in_wr_cnt;
    length_w = in_length_w;
    while(length_w)
    {
        checksum += in_pRam[offset];
        length_w --;
        offset ++;
    }
    return(checksum ^ 0x34927c5a);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//数据块错误检查
//入口:数据长度
//出口:错误代码
//-----------------------------------------------------------------------------
#define  ErrOneBit      0x01     //1区错误
#define  ErrTwoBit      0x02     //2区错误
//-----------------------------------------------------------------------------
static sdt_int8u check_block_error(block_storage_parameter_def* in_block_SP)
{
    sdt_int32u read_checksum;
    sdt_int32u make_checksum;
    sdt_int8u ErrorBit;

    ErrorBit=0;
    read_checksum = in_block_SP->passage_flash_memory_read(block_first,(in_block_SP->storage_length_w+1));
    make_checksum = make_flash_memory_checksum(block_first,in_block_SP);
    if(read_checksum != make_checksum)
    {
        ErrorBit |= ErrOneBit;
    }
    
    read_checksum = in_block_SP->passage_flash_memory_read(block_second,(in_block_SP->storage_length_w+1));
    make_checksum = make_flash_memory_checksum(block_second,in_block_SP);
    if(read_checksum != make_checksum)
    {
        ErrorBit|=ErrTwoBit;
    }
    return(ErrorBit);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static sdt_bool read_block_from_flash(sdt_int32u* out_pRam,block_storage_parameter_def* in_block_SP)
{
    sdt_int8u   Error;
    block_def  block_select;
    
    Error = check_block_error(in_block_SP);
    
    if((Error&ErrOneBit)&&(Error&ErrTwoBit))
    {
        return(sdt_false);
    }
    else
    {
        if(Error&ErrOneBit)  //取2段
        {
            block_select = block_second;
        }
        else if(Error&ErrTwoBit) //取1段
        {
            block_select = block_first;
        }
        else
        {
            sdt_int32u rd_cnt_block1,rd_cnt_block2;
            
            rd_cnt_block1 = in_block_SP->passage_flash_memory_read(block_first,0);
            rd_cnt_block2 = in_block_SP->passage_flash_memory_read(block_second,0);
            while((rd_cnt_block1 == 0xffffffff)||(rd_cnt_block2 == 0xffffffff))
            {
                rd_cnt_block1 = rd_cnt_block1 + 1;
                rd_cnt_block2 = rd_cnt_block2 + 1;
            }
            if(rd_cnt_block1 > rd_cnt_block2)
            {
                block_select = block_first;
            }
            else
            {
                block_select = block_second;
            }
        }
        
        sdt_int32u offset = 1;
        sdt_int32u length_w;
        length_w = in_block_SP->storage_length_w;
        while(length_w)
        {
            *out_pRam = in_block_SP->passage_flash_memory_read(block_select,offset);
            out_pRam ++;
            offset ++;
            length_w --;
        }
    }
    return(sdt_true);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void write_block_to_flash(sdt_int32u* in_pRam,block_storage_parameter_def* in_block_SP)
{
    sdt_int8u   Error;
    block_def   block_select;
    sdt_int32u  w_next_cnt;
    
    Error = check_block_error(in_block_SP);
    if((Error&ErrOneBit)&&(Error&ErrTwoBit))
    {
        block_select = block_first;
        w_next_cnt = 0;
    }
    else
    {
        if(Error&ErrOneBit)  //取1段
        {
            block_select = block_first;
            w_next_cnt = in_block_SP->passage_flash_memory_read(block_second,0);
            w_next_cnt += 1;
        }
        else if(Error&ErrTwoBit) //取2段
        {
            block_select = block_second;
            w_next_cnt = in_block_SP->passage_flash_memory_read(block_first,0);
            w_next_cnt += 1;
        }
        else
        {
            sdt_int32u rd_cnt_block1,rd_cnt_block2;
            
            rd_cnt_block1 = in_block_SP->passage_flash_memory_read(block_first,0);
            rd_cnt_block2 = in_block_SP->passage_flash_memory_read(block_second,0);
            while((rd_cnt_block1 == 0xffffffff)||(rd_cnt_block2 == 0xffffffff))
            {
                rd_cnt_block1 = rd_cnt_block1 + 1;
                rd_cnt_block2 = rd_cnt_block2 + 1;
            }
            if(rd_cnt_block1 >rd_cnt_block2)
            {
                block_select = block_second;
                w_next_cnt = rd_cnt_block1 + 1;
            }
            else
            {
                block_select = block_first;
                w_next_cnt = rd_cnt_block2 + 1;
            }
        }
    }
    sdt_int32u checksum;
    sdt_int32u offset;
    sdt_int32u length_w;
//------------------------------------------------------------------------------比较数据是否有改变
    block_def rd_block_s;
    sdt_int32u* pRam_new_data;
    sdt_bool data_changed = sdt_false;
    
    if(block_first == block_select)
    {
        rd_block_s = block_second;
    }
    else
    {
        rd_block_s = block_first;
    }
    offset = 1;
    pRam_new_data = in_pRam;
    length_w = in_block_SP->storage_length_w;
    while(length_w)
    {
        if(Error&(ErrOneBit|ErrTwoBit))
        {
            data_changed = sdt_true;
            break;   
        }
        else if(*pRam_new_data != in_block_SP->passage_flash_memory_read(rd_block_s,offset))
        { 
            data_changed = sdt_true;
            break;
        }
        offset ++;
        pRam_new_data ++;
        length_w --;
    }
//------------------------------------------------------------------------------
    if(data_changed)
    {
        checksum = make_ram_memory_checksum(w_next_cnt,in_pRam,in_block_SP->storage_length_w);
        length_w = in_block_SP->storage_length_w;
        
        in_block_SP->passage_flash_memory_write(block_select,0,w_next_cnt);
        offset = 1;
        while(length_w)
        {
            in_block_SP->passage_flash_memory_write(block_select,offset,*in_pRam);
            offset ++;
            in_pRam ++;
            length_w --;
        }
        in_block_SP->passage_flash_memory_write(block_select,offset,checksum);
        in_block_SP->passage_flash_write_complete();
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//长度设置，要求入口长度32bits对齐
//-----------------------------------------------------------------------------
static void set_storage_length_w(block_storage_parameter_def* in_block_SP,sdt_int32u in_size_8bits)
{
    if(0 != in_size_8bits%4)
    {
        while(1);
    }
    if(in_size_8bits > 2040)
    {
        while(1);
    }
    in_block_SP->storage_length_w = in_size_8bits/4;
}

static sdt_bool cfged = sdt_false;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include ".\sod_Storage32bits.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
sdt_bool mde_read_storage_block(sdt_int8u in_solidNum,sdt_int32u* out_pRam)
{
    if(cfged)
    {     
    }
    else
    {
        block_storage_solid_cfg();
        cfged = sdt_true;
    }

    if(in_solidNum < max_solid)
    {
        sdt_bool success;
        success = read_block_from_flash(out_pRam,&block_storage_solid[in_solidNum]);
        return(success);
    }
    else
    {
        while(1);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mde_write_storage_block(sdt_int8u in_solidNum,sdt_int32u* in_pRam)
{
    if(cfged)
    {     
    }
    else
    {
        block_storage_solid_cfg();
        cfged = sdt_true;
    }
    
    if(in_solidNum < max_solid)
    {
        write_block_to_flash(in_pRam,&block_storage_solid[in_solidNum]);
    }
    else
    {
        while(1);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mde_storage_block_set_length_w(sdt_int8u in_solidNum,sdt_int32u in_length_8bit)
{
    if(cfged)
    {     
    }
    else
    {
        block_storage_solid_cfg();
        cfged = sdt_true;
    }

    if(in_solidNum < max_solid)
    {
        set_storage_length_w(&block_storage_solid[in_solidNum],in_length_8bit);
    }
    else
    {
        while(1);
    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-----------------------------------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
