#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include "driver/inc.h"

#define Led_Soft_Timer_init(os_timer,func,ms) bsp_Soft_Timer_init(os_timer,func,ms,520,1)


//ע��os_timer������ȫ�ֱ���
void ICACHE_FLASH_ATTR bsp_Soft_Timer_init(os_timer_t* os_timer,
                                            os_timer_func_t* func,
                                            uint32 ms,
                                            uint32 reg,
                                            bool repeat_flag);

//�ر������ʱ��
void ICACHE_FLASH_ATTR bsp_Soft_Timer_close(os_timer_t* os_timer);

//�������������ʱ��(�����������Ķ�ʱʱ��)
void ICACHE_FLASH_ATTR bsp_Soft_Timer_Restart(os_timer_t* os_timer,
                                                uint32 ms,
                                                bool repeat_flag);

//Ӳ����ʱ����ʼ��
void ICACHE_FLASH_ATTR bsp_hw_timer_init(void (* user_hw_timer_cb_set)(void),uint32_t us);

#endif
