/*
 * CQueue.c
 *
 *  Created on: Nov 18, 2016
 *      Author: hflw
 */

#include "include\psptypes.h"
#include <PE_Error.h>
#include <PE_Types.h>
#include "CQueue.h"


/*************************************************************************************************************************
** 函数名称: InitBufferQueue
**
** 函数描述: 缓冲队列初始化；并按照初始化长度分配内存空间；
**
**
**
** 输入变量: struct BUFFER_QUEUE *p_buffer; uint8 length;
** 返回值: void;
**
** 使用宏或常量: None;
** 使用全局变量: None;
**
** 调用函数: None;
**
** 创建人:
** 创建日期: 2009-03-07
**------------------------------------------------------------------------------------------------------------------------
** 修订人:
** 修订日期: 2009-08-10
**------------------------------------------------------------------------------------------------------------------------
*************************************************************************************************************************/

uint16_t InitBufferQueue(BUFFER_QUEUE *p_buffer, COMM_MSG *p_item_buffer, int length)
{
	uint16_t result = ERR_OK;
	COMM_MSG * p_free_space = NULL;
	
	if(!p_buffer || !p_item_buffer || !length )
		return ERR_NOTAVAIL;
	SetReg(p_buffer->state, 0);// 初始化状态寄存器
	
	
	if(0x00 == length || length > MAX_QUEUE_LEN)
	{
		SetRegMask(p_buffer->state, ERR_MASK, ERR_VALUE);// 置错误类型标志位
		return ERR_PARAM_SIZE;
	}

	p_buffer->length = length;// 初始化缓冲区长度
	
	
	//p_free_space = malloc(length);// 分配长度为length的RAM空间
	p_free_space = p_item_buffer;//we can't alloc in SoC
	
	
	if (NULL == p_free_space)
	{
		SetRegMask(p_buffer->state, ERR_MASK, ERR_NOTAVAIL);// 置错误类型标志位
		return ERR_NOTAVAIL;
	}
	
	
	p_buffer->init_address = p_free_space;
	
	
	p_buffer->rear = p_free_space;
	p_buffer->front = p_free_space;
	
	
	SetReg(p_buffer->state, INIT_COMPLETE);// 置位初始化完成标志
	
	return result;
}




/*************************************************************************************************************************
** 函数名称: TestEmptyBufferQueue
**
** 函数描述: 测试缓冲队列是否为空，为空则返回TRUE，不为空则返回FALSE。
**
**
**
** 输入变量: struct BUFFER_QUEUE *p_buffer;
** 返回值: void;
**
** 使用宏或常量: None;
** 使用全局变量: None;
**
** 调用函数: None;
**
** 创建人: 
** 创建日期: 2009-03-07
**------------------------------------------------------------------------------------------------------------------------
** 修订人:
** 修订日期:
**------------------------------------------------------------------------------------------------------------------------
*************************************************************************************************************************/
int TestEmptyBufferQueue(BUFFER_QUEUE *p_buffer)
{
	if((p_buffer->state & ERR_MASK) == ERR_OVERRUN)
		return FALSE;
	else if(p_buffer->rear == p_buffer->front)
		return TRUE;
	else
		return FALSE;
}

int TestFullBufferQueue(BUFFER_QUEUE *p_buffer)
{
	if((p_buffer->state & ERR_MASK) == ERR_OVERRUN)
		return TRUE;
	else
		return FALSE;
}


/*************************************************************************************************************************
** 函数名称: AddBufferQueue
**
** 函数描述: 向缓冲队列添加元素，队尾指针rear加1。
**
**
** 输入变量: struct BUFFER_QUEUE *p_buffer; uint8 &item;
** 返回值: void;
**
** 使用宏或常量: None;
** 使用全局变量: None;
**
** 调用函数: None;
**
** 创建日期: 2009-03-07
**------------------------------------------------------------------------------------------------------------------------
** 修订人:
** 修订日期:
**------------------------------------------------------------------------------------------------------------------------
*************************************************************************************************************************/
uint16_t AddBufferQueue(BUFFER_QUEUE *p_buffer, COMM_MSG* item)
{
	uint16_t result = ERR_OK;
	int size = sizeof(COMM_MSG);
	//*p_buffer->rear = item;// 数据入队
	memcpy((p_buffer->rear), item, size );

	if((p_buffer->state & ERR_MASK) == ERR_OVERRUN)
		return ERR_OVERRUN;
	p_buffer->rear++;// 队尾地址＋1
	
	if (p_buffer->rear >= (p_buffer->init_address + p_buffer->length))// 队尾地址超过分配内存边界
	{
		p_buffer->rear = p_buffer->init_address;// 队尾地址回缓冲区首地址
	}
	
	
	if (p_buffer->rear == p_buffer->front)// 队列长度大于缓冲区长度
	{
		SetRegMask(p_buffer->state, ERR_MASK, ERR_OVERRUN);// 置溢出错误标志
	}
	else
		SetRegMask(p_buffer->state, ERR_MASK, ERR_OK);

	return result;
}


/*************************************************************************************************************************
** 函数名称: OutBufferQueue
**
** 函数描述: 从队列取出元素，队头指针front加1。当队列为空时返回FALSE，操作成功返回TRUE。
**
**
**
** 输入变量: struct BUFFER_QUEUE *p_buffer; uint8 &item;
** 返回值: uint8;
**
** 使用宏或常量: None;
** 使用全局变量: None;
**
** 调用函数: None;
**
** 创建人: 
** 创建日期: 2009-03-07
**------------------------------------------------------------------------------------------------------------------------
** 修订人:
** 修订日期:
**------------------------------------------------------------------------------------------------------------------------
*************************************************************************************************************************/
uint16_t OutBufferQueue(BUFFER_QUEUE *p_buffer, COMM_MSG *p_item)
{
	uint16_t result = ERR_OK;
	if(TestEmptyBufferQueue(p_buffer))
	{
		return ERR_NOTAVAIL;
	}
	else
	{
		//*p_item = *(p_buffer->front);// 数据出队
		memcpy( (void *)p_item, (void *)(p_buffer->front), sizeof(COMM_MSG) );
		
		
		p_buffer->front++;
		
		
		if(p_buffer->front >= (p_buffer->init_address + p_buffer->length))
		{
			p_buffer->front = p_buffer->init_address;
		}
		
		SetRegMask(p_buffer->state, ERR_MASK, ERR_OK);//clear overrun status if it is
		
		return result;
	}
}




/*************************************************************************************************************************
** 函数名称: CheckBufferQueueLength
**
** 函数描述: 检查缓冲队列长度;
**
** 输入变量: struct BUFFER_QUEUE *p_buffer;
** 返回值: void;
**
** 使用宏或常量: None;
** 使用全局变量: None;
**
** 调用函数: None;
**
** 创建人: 
** 创建日期: 2009-08-19
**------------------------------------------------------------------------------------------------------------------------
** 修订人:
** 修订日期:
**------------------------------------------------------------------------------------------------------------------------
*************************************************************************************************************************/
int CheckBufferQueueLength(BUFFER_QUEUE* p_buffer)
{
	int temp_value = 0;
	
	if((p_buffer->state & ERR_MASK) == ERR_OVERRUN)
		return p_buffer->length;
	
	if (p_buffer->rear >= p_buffer->front)
	{
		temp_value = p_buffer->rear - p_buffer->front;
	}
	else
	{
		temp_value = p_buffer->front - p_buffer->rear;
		temp_value = p_buffer->length - temp_value;
	}
	
	
	return temp_value;
}





/*************************************************************************************************************************
** 函数名称: CheckBufferQueueState
**
** 函数描述: 检查缓冲队列状态，分配内存空间失败、数据溢出等;
**
**
**
** 输入变量: struct BUFFER_QUEUE *p_buffer;
** 返回值: void;
**
** 使用宏或常量: None;
** 使用全局变量: None;
**
** 调用函数: None;
**
** 创建人: 
** 创建日期: 2009-08-19
**------------------------------------------------------------------------------------------------------------------------
** 修订人:
** 修订日期:
**------------------------------------------------------------------------------------------------------------------------
*************************************************************************************************************************/
int CheckBufferQueueState(BUFFER_QUEUE *p_buffer)
{
	return p_buffer->state;
}




/*************************************************************************************************************************
** 函数名称: ClearBufferQueue
**
** 函数描述: 清空队列;
**
**
** 输入变量: struct BUFFER_QUEUE *p_buffer;
** 返回值: void;
**
** 使用宏或常量: None;
** 使用全局变量: None;
**
** 调用函数: None;
**
** 创建人: 
** 创建日期: 2009-08-19
**------------------------------------------------------------------------------------------------------------------------
** 修订人:
** 修订日期:
**------------------------------------------------------------------------------------------------------------------------
*************************************************************************************************************************/
void ClearBufferQueue(BUFFER_QUEUE *p_buffer)
{
	p_buffer->rear = p_buffer->front;// 移动头尾指针


	SetReg(p_buffer->state, 0);// 初始化状态寄存器
	SetReg(p_buffer->state, INIT_COMPLETE);// 置位初始化完成标志
}








/*************************************************************************************************************************
** 文件结束
*************************************************************************************************************************/
