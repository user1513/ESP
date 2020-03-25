#include "driver/softap_udp_tcp.h"


static int ICACHE_FLASH_ATTR My_Pow(int x, int y);
//������ΪXX.XX.XX.XX��IP�ַ���ת����uint8_t[4]������
static bool ICACHE_FLASH_ATTR IpStr2Tab(uint8_t* ip_table,const char * ip_str);



void ICACHE_FLASH_ATTR ESP8266_WIFI_recv_callback(void *arg, char *pdata, unsigned short len)
{
    struct espconn* pEspconn = (struct espconn*)arg;
    remot_info * P_port_info = NULL;	// Զ��������Ϣ�ṹ��ָ��


    if(espconn_get_connection_info(pEspconn, &P_port_info, 0)==ESPCONN_OK)	// ��ȡԶ����Ϣ
	{
		pEspconn->proto.udp->remote_port  = P_port_info->remote_port;		// ��ȡ�Է��˿ں�
		pEspconn->proto.udp->remote_ip[0] = P_port_info->remote_ip[0];		// ��ȡ�Է�IP��ַ
		pEspconn->proto.udp->remote_ip[1] = P_port_info->remote_ip[1];
		pEspconn->proto.udp->remote_ip[2] = P_port_info->remote_ip[2];
		pEspconn->proto.udp->remote_ip[3] = P_port_info->remote_ip[3];

        os_printf("remote_ip:" IPSTR "\n",pEspconn->proto.udp->remote_ip[0],
                    pEspconn->proto.udp->remote_ip[1],
                    pEspconn->proto.udp->remote_ip[2],
                    pEspconn->proto.udp->remote_ip[3]);
	}

        

    espconn_send(pEspconn,"ESP8266_WIFI_Recv_OK",os_strlen("ESP8266_WIFI_Recv_OK"));	// ��Է�����Ӧ��

    os_printf("remote_port:%d",pEspconn->proto.udp->remote_port);

    os_printf("pdata = %s\n", pdata);

    if(!strcmp(pdata,"open"))
    {
        os_printf("����!!!!\n");
    }
    if(!strcmp(pdata,"close"))
    {
        os_printf("�ص�!!!!\n");
    }
    os_printf("\nESP8266_WIFI_Recelve_OK\n");
}

void ICACHE_FLASH_ATTR ESP8266_WIFI_sent_callback(void *arg)
{
    os_printf("\nESP8266_WIFI_Send_OK\n");
}

struct espconn _espconn;// ע�����붨��Ϊȫ�ֱ������ں˽���ʹ�ô˱���(�ڴ�)
esp_udp _esp_udp;
//����udp������
void Create_Udp_Connect_Server(int local_port)
{
    _esp_udp.local_port = local_port;
    _espconn.proto.udp = &_esp_udp;
    _espconn.type = ESPCONN_UDP;

	// �ۣ�ע��/����ص�����
    //Ҳ����ʹ��
    //_espconn.recv_callback = (espconn_recv_callback)ESP8266_WIFI_recv_callback;
    //_espconn.sent_callback = (espconn_sent_callback)ESP8266_WIFI_sent_callback;
	espconn_regist_sentcb(&_espconn,ESP8266_WIFI_sent_callback);	// ע���������ݷ��ͳɹ��Ļص�����
	espconn_regist_recvcb(&_espconn,ESP8266_WIFI_recv_callback);	// ע���������ݽ��ճɹ��Ļص�����
    espconn_create(&_espconn);
}
//����udp�ͻ���
void Create_Udp_Connect_Client(const char * remote_ip_str, int remote_port)
{
    //_esp_udp.local_port = local_port;
    if(IpStr2Tab(_esp_udp.remote_ip, remote_ip_str) != true) return ;
    os_printf(IPSTR "\n",(uint16)_esp_udp.remote_ip[0],(uint16)_esp_udp.remote_ip[1],(uint16)_esp_udp.remote_ip[2],(uint16)_esp_udp.remote_ip[3]);
    _esp_udp.remote_port = remote_port;
    _espconn.proto.udp = &_esp_udp;
    _espconn.type = ESPCONN_UDP;

	// �ۣ�ע��/����ص�����
	//---------------------------------------------------------------------------------------
    //Ҳ����ʹ��
    //_espconn.recv_callback = (espconn_recv_callback)ESP8266_WIFI_recv_callback;
    //_espconn.sent_callback = (espconn_sent_callback)ESP8266_WIFI_sent_callback;

	espconn_regist_sentcb(&_espconn,ESP8266_WIFI_sent_callback);	// ע���������ݷ��ͳɹ��Ļص�����
	espconn_regist_recvcb(&_espconn,ESP8266_WIFI_recv_callback);	// ע���������ݽ��ճɹ��Ļص�����
    espconn_create(&_espconn);
}
esp_udp _esp_tcp;



void ICACHE_FLASH_ATTR ESP8266_WIFI_connect_callback(void *arg)
{
    struct espconn* pEspconn= (struct espconn*)arg;
    os_printf("TCP���ӳɹ�\n");
    Send_Wifi_Data("hello i am yuan\n");

}
//����tcp������
void Create_Tcp_Connect_Server(int local_port)
{
    _esp_tcp.local_port = local_port;
    _espconn.proto.tcp = &_esp_tcp;
    _espconn.type = ESPCONN_TCP;

	// �ۣ�ע��/����ص�����
    //Ҳ����ʹ��
    //_espconn.recv_callback = (espconn_recv_callback)ESP8266_WIFI_recv_callback;
    //_espconn.sent_callback = (espconn_sent_callback)ESP8266_WIFI_sent_callback;
	espconn_regist_sentcb(&_espconn,ESP8266_WIFI_sent_callback);	// ע���������ݷ��ͳɹ��Ļص�����
	espconn_regist_recvcb(&_espconn,ESP8266_WIFI_recv_callback);	// ע���������ݽ��ճɹ��Ļص�����
    espconn_accept(&_espconn);

    espconn_regist_time(&_espconn, 3000, 0);

    espconn_regist_connectcb(&_espconn,(espconn_connect_callback)ESP8266_WIFI_connect_callback);

}


//����tcp�ͻ���
void Create_Tcp_Connect_Client(const char * remote_ip_str, int remote_port)
{

    _esp_tcp.local_port = espconn_port();
    if(IpStr2Tab(_esp_tcp.remote_ip, remote_ip_str) != true) return ;
    os_printf(IPSTR "\n",_esp_tcp.remote_ip[0],_esp_tcp.remote_ip[1],_esp_tcp.remote_ip[2],_esp_tcp.remote_ip[3]);
    _esp_tcp.remote_port = remote_port;
    _espconn.proto.tcp = &_esp_tcp;
    _espconn.type = ESPCONN_TCP;

	// �ۣ�ע��/����ص�����
	//---------------------------------------------------------------------------------------
    //Ҳ����ʹ��
    //_espconn.recv_callback = (espconn_recv_callback)ESP8266_WIFI_recv_callback;
    //_espconn.sent_callback = (espconn_sent_callback)ESP8266_WIFI_sent_callback;

	espconn_regist_sentcb(&_espconn,ESP8266_WIFI_sent_callback);	// ע���������ݷ��ͳɹ��Ļص�����
	espconn_regist_recvcb(&_espconn,ESP8266_WIFI_recv_callback);	// ע���������ݽ��ճɹ��Ļص�����
    espconn_connect(&_espconn);

    espconn_regist_time(&_espconn, 3000, 0);

    espconn_regist_connectcb(&_espconn,(espconn_connect_callback)ESP8266_WIFI_connect_callback);
    
}






//�l��wifi����
void Send_Wifi_Data(const char* str)
{
    espconn_send(&_espconn,str,os_strlen(str));	
}

// ������ΪXX.XX.XX.XX��IP�ַ���ת����uint8_t[4]������
static bool IpStr2Tab(uint8_t* ip_table,const char * ip_str)
{
    int i = 0;
    uint8_t temp[4] = {0,0,0,0};
    for(; i < 4; i++)
    {
        while((*ip_str != '.') && (*ip_str != '\0'))
        {
            if((*ip_str >= '0') && (*ip_str <= '9'))
            {
                temp[i] = My_Pow(temp[i],1) + (*ip_str - '0');
            }
  
            ip_str++;   
        } 
        ip_str++;    
    }  
    if(i != 4)
        return false;
    os_printf("��ȡ�Ĳ���:" IPSTR "\n",(uint16)temp[0],(uint16)temp[1],(uint16)temp[2],(uint16)temp[3]);
    os_memcpy(ip_table, temp, sizeof(temp));
    return true;
}

static int My_Pow(int x, int y)
{
    while(y--)
    {
        x *= 10; 
    }
    return x;
} 