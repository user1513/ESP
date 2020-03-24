#ifndef __SOFTAP_STATION_H
#define __SOFTAP_STATION_H

#include "driver/inc.h"
//设置AP ,SAT, AP + SAT
bool softap_station_scan(uint8_t _set_wifi_mode, uint8_t _select);

//获取AP模式下参数
bool My_Get_softap_Status(uint8_t _select);

//设置AP模式下的参数
bool My_Set_softap_Status(uint8_t* ssid, uint8_t* password,uint8_t _select);

//获取AP模式下终端的信息
void Get_station_info_from_AP(void);

//查询本地端口信息
bool Get_localhost_info(uint8_t mode);
#endif
