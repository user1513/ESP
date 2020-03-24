#include "driver/softap_udp_tcp.h"




void ICACHE_FLASH_ATTR ESP8266_WIFI_recv_callback(void *arg, char *pdata, unsigned short len)
{
    struct espconn* pEspconn = (struct espconn*)arg;
    remot_info * P_port_info = NULL;	// 远端连接信息结构体指针


    if(espconn_get_connection_info(pEspconn, &P_port_info, 0)==ESPCONN_OK)	// 获取远端信息
	{
		pEspconn->proto.udp->remote_port  = P_port_info->remote_port;		// 获取对方端口号
		pEspconn->proto.udp->remote_ip[0] = P_port_info->remote_ip[0];		// 获取对方IP地址
		pEspconn->proto.udp->remote_ip[1] = P_port_info->remote_ip[1];
		pEspconn->proto.udp->remote_ip[2] = P_port_info->remote_ip[2];
		pEspconn->proto.udp->remote_ip[3] = P_port_info->remote_ip[3];

        os_printf("remote_ip:" IPSTR "\n",pEspconn->proto.udp->remote_ip[0],
                    pEspconn->proto.udp->remote_ip[1],
                    pEspconn->proto.udp->remote_ip[2],
                    pEspconn->proto.udp->remote_ip[3]);
	}

        

    espconn_send(pEspconn,"ESP8266_WIFI_Recv_OK",os_strlen("ESP8266_WIFI_Recv_OK"));	// 向对方发送应答

    os_printf("remote_port:%d",pEspconn->proto.udp->remote_port);

    os_printf("pdata = %s\n", pdata);

    if(!strcmp(pdata,"open"))
    {
        os_printf("开灯!!!!\n");
    }
    if(!strcmp(pdata,"close"))
    {
        os_printf("关灯!!!!\n");
    }
    os_printf("\nESP8266_WIFI_Recelve_OK\n");
}

void ICACHE_FLASH_ATTR ESP8266_WIFI_sent_callback(void *arg)
{
    os_printf("\nESP8266_WIFI_Send_OK\n");
}

struct espconn _espconn;// 注：必须定义为全局变量，内核将会使用此变量(内存)
esp_udp _esp_udp;

void Create_Udp_connect(void)
{
    _esp_udp.local_port = 1314;
    _espconn.proto.udp = &_esp_udp;
    _espconn.type = ESPCONN_UDP;

	// ③：注册/定义回调函数
	//-------------------------------------------------------------------------------------------
    //也可以使用
    //_espconn.recv_callback = (espconn_recv_callback)ESP8266_WIFI_recv_callback;
    //_espconn.sent_callback = (espconn_sent_callback)ESP8266_WIFI_sent_callback;

	espconn_regist_sentcb(&_espconn,ESP8266_WIFI_sent_callback);	// 注册网络数据发送成功的回调函数
	espconn_regist_recvcb(&_espconn,ESP8266_WIFI_recv_callback);	// 注册网络数据接收成功的回调函数
    espconn_create(&_espconn);
}

