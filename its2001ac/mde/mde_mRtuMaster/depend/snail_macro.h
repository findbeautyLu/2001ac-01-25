//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//用户宏定义
//-------------------------------------------------------------------------------------------------
#ifndef  snail_macro_defed
#define  snail_macro_defed
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef snail_data_types
    #include ".\snail_data_types.h"
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//数组清零
//-------------------------------------------------------------------------------------------------
#define macro_clear_array(in_arrayName) \
do\
{\
    sdt_int8u in_arrayName##_index = 0;\
    sdt_int8u in_arrayName##_size = sizeof(in_arrayName);\
    while(in_arrayName##_size)\
    {\
        in_arrayName##[in_arrayName##_index] = 0;\
        in_arrayName##_index++;\
        in_arrayName##_size--;\
    }\
}while(0)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//数组填充
//-------------------------------------------------------------------------------------------------
#define macro_fill_array(in_arrayName,in_newData) \
do\
{\
    sdt_int8u in_arrayName##_index = 0;\
    sdt_int8u in_arrayName##_size = sizeof(in_arrayName);\
    while(in_arrayName##_size)\
    {\
        in_arrayName##[in_arrayName##_index] = in_newData;\
        in_arrayName##_index++;\
        in_arrayName##_size--;\
    }\
}while(0)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//8bit长度的环形队列，中断原子特性
//-------------------------------------------------------------------------------------------------
//创建一个队列
//-------------------------------------------------------------------------------------------------
#define macro_creat_queueBasce(in_queueName,in_queueBytes) \
        volatile static sdt_int8u  in_queueName##_buff[in_queueBytes];\
		volatile static sdt_int8u  in_queueName##_bytes = 0;\
		static const sdt_int8u in_queueName##_maxBytes = in_queueBytes;\
		volatile static sdt_int8u in_queueName##_in_idx = 0;\
		volatile static sdt_int8u in_queueName##_out_idx = 0
//-------------------------------------------------------------------------------------------------
//获取队列的字节数
//-------------------------------------------------------------------------------------------------
#define macro_pull_queueBasce_bytes(in_queueName,out_bytes) \
do\
{\
	sdt_int8u in_queueName##_rd_bytes;\
	in_queueName##_rd_bytes = in_queueName##_bytes;\
	out_bytes = in_queueName##_rd_bytes;\
}while(0)
//-------------------------------------------------------------------------------------------------
//获取队列full的情况
//-------------------------------------------------------------------------------------------------
#define macro_pull_queueBasce_full(in_queueName,in_bytes,out_full) \
do\
{\
    if(in_bytes > in_queueName##_maxBytes)\
	{\
	    in_queueName##_bytes = 0;\
		in_queueName##_in_idx = 0;\
		in_queueName##_out_idx = 0;\
		out_full = sdt_false;\
	}\
	if(in_bytes == in_queueName##_maxBytes)\
    {\
		out_full = sdt_true;\
	}\
	else\
	{\
		out_full = sdt_false;\
	}\
}while(0)
//-------------------------------------------------------------------------------------------------
//从队列中取一个数据
//-------------------------------------------------------------------------------------------------
#define macro_pull_queueBasce_data(in_queueName,in_bytes,out_data) \
do\
{\
    if(0 != in_queueName##_bytes)\
	{\
        sdt_int8u the_out_index;\
        the_out_index = in_queueName##_out_idx;\
        out_data = in_queueName##_buff[the_out_index];\
	    in_queueName##_out_idx = in_queueName##_out_idx + 1;\
		if(in_queueName##_out_idx > (in_queueName##_maxBytes - 1))\
	    {\
			in_queueName##_out_idx = 0;\
		}\
		in_queueName##_bytes = in_queueName##_bytes - 1;\
    }\
}while(0)
//-------------------------------------------------------------------------------------------------
#define macro_push_queueBasce_data(in_queueName,in_bytes,in_data) \
do\
{\
    if(in_bytes < in_queueName##_maxBytes)\
	{\
        sdt_int8u the_index;\
        the_index = in_queueName##_in_idx;\
        in_queueName##_buff[the_index] = in_data;\
	    in_queueName##_in_idx = in_queueName##_in_idx + 1;\
		if(in_queueName##_in_idx > (in_queueName##_maxBytes - 1))\
		{\
			in_queueName##_in_idx = 0;\
		}\
		in_queueName##_bytes = in_queueName##_bytes + 1;\
    }\
}while(0)
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++end+++++++++++++++++++++++++++++++++++++++++++++++++++
//指针边界保护
//BN_TRUE 正常
//---------------------------------------------------------------------
#define _CheckPointerBoundary(_CurrentIndex,_MaxIndex,_NotFail) \
do\
{\
   if((_CurrentIndex)>(_MaxIndex))\
   {\
       _NotFail=BN_FALSE;\
   }\
   else\
   {\
       _NotFail=BN_TRUE;\
   }\
}while(0)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//获取数组最大索引
//---------------------------------------------------------------------
#define _GetArrayMaxIndex(_ArrayName) (sizeof(_ArrayName)-1)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#endif	
//++++++++++++++++++++++++++++++++End++++++++++++++++++++++++++++++++++

