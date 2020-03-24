#include "driver/softap_station.h"


/*
_set_wifi_mode ѡ��
#define NULL_MODE       0x00
#define STATION_MODE    0x01
#define SOFTAP_MODE     0x02
#define STATIONAP_MODE  0x03

_select
0--->��ѯ Wi-Fi ��ǰ?����ģʽ,���� Wi-Fi ����ģʽ��Station��SoftAP ���� Station + SoftAP������?���浽 Flash��
1--->��ѯ������ Flash �е� Wi-Fi ?����ģʽ����,���� Wi-Fi ?����ģʽ��Station��SoftAP ���� Station+SoftAP���������浽 Flash��


*/
bool softap_station_scan(uint8_t _set_wifi_mode, uint8_t _select)
{
    bool _bool = false;
    uint8_t wifi_mode = 0;
    if(_select)
    {
        //��ѯ������ Flash �е� Wi-Fi ?����ģʽ����
        wifi_mode = wifi_get_opmode_default();
        //�����������ԭ���ò�ͬ������±��浽 Flash ϵͳ��������
        _bool = wifi_set_opmode(_set_wifi_mode);
    }
    else
    {
        //��ѯ Wi-Fi ��ǰ?����ģʽ
        wifi_mode = wifi_get_opmode();
        //���� Wi-Fi ?����ģʽ��Station��SoftAP ���� Station + SoftAP������?���浽 Flash��
        _bool = wifi_set_opmode_current(_set_wifi_mode);
    }
    switch(wifi_mode)
    {
        case 1: os_printf("��ǰwifi����ģʽ>>Station\n");break;
        case 2: os_printf("��ǰwifi����ģʽ>>SoftAP\n");break;
        case 3: os_printf("��ǰwifi����ģʽ>>Station+SoftAP\n");break;
        default : os_printf("��ǰwifi����ģʽ>>ERR\n");break;
    }
    return _bool;
}






/*
struct softap_config {
    uint8 ssid[32];
    uint8 password[64];
    uint8 ssid_len;    // Note: Recommend to set it according to your ssid
    uint8 channel;    // Note: support 1 ~ 13
    AUTH_MODE authmode;    // Note: Don't support AUTH_WEP in softAP mode.
    uint8 ssid_hidden;    // Note: default 0
    uint8 max_connection;    // Note: default 4, max 4
    uint16 beacon_interval;    // Note: support 100 ~ 60000 ms, default 100
};
*/
//��ȡAPģʽ�²���


bool My_Get_softap_Status(uint8_t _select)
{
    struct softap_config Softap_Config;
    bool _bool = false;

    if(_select)
    {
        //��ѯ ESP8266 Wi-Fi SoftAP ��?�ڱ����� Flash �е�����
        _bool = wifi_softap_get_config_default(&Softap_Config);
    }
    else
    {
        //��ѯ ESP8266 Wi-Fi SoftAP ��?�ڵĵ�ǰ����
        _bool = wifi_softap_get_config(&Softap_Config);
    }
    if(_bool == false) return _bool;
    os_printf("ssid_name:%s\n", Softap_Config.ssid);
    os_printf("password:%s\n", Softap_Config.password);
    os_printf("ssid_len:%d\n", Softap_Config.ssid_len);
    os_printf("max_connection:%d\n", Softap_Config.max_connection);
    return _bool;
}


//����APģʽ�µĲ���
bool My_Set_softap_Status(uint8_t* ssid, uint8_t* password,uint8_t _select)
{
    bool _bool = false;
    struct softap_config Softap_Config;

    memset(&Softap_Config,0,sizeof(struct softap_config));      //��struct softap_config Softap_Config;ȫ��ֵΪ0
    //this
    os_strcpy(&Softap_Config.ssid, ssid);        //����wifi��
    os_strcpy(&Softap_Config.password, password);//����wifi����
    //this

    //ע��ssid_len�ĳ����ǿɼ��ĳ���,���ܰ�'\0'����
    Softap_Config.ssid_len = os_strlen(ssid) ;                  //����wifi������
    Softap_Config.channel = 1;    // Note: support 1 ~ 13
    Softap_Config.authmode = AUTH_WPA_WPA2_PSK;    // Note: Don't support AUTH_WEP in softAP mode.
    Softap_Config.ssid_hidden = 0;    // Note: default 0 Ĭ�ϲ�����wifi��
    Softap_Config.max_connection = 4;    // Note: default 4, max 4
    //����·�������ű�������·�������͵������źţ���������ɨ�赽���ű��������Ӧ��·��������ʾ������Ĭ����100ms��
    Softap_Config.beacon_interval = 100;    // Note: support 100 ~ 60000 ms, default 100


    if(_select)
    {
        //��ѯ ESP8266 Wi-Fi SoftAP �ӿڱ����� Flash �е�����
        _bool = wifi_softap_set_config(&Softap_Config);
    }
    else
    {
        //��ѯ ESP8266 Wi-Fi SoftAP ��?�ڵĵ�ǰ����
        _bool = wifi_softap_set_config_current(&Softap_Config);
    }
    return _bool;
}


/*
struct station_info {
    STAILQ_ENTRY(station_info)    next;

    uint8 bssid[6];
    struct ip_addr ip;
};
*/
void Get_station_info_from_AP(void)
{
    struct station_info * pStation_info = NULL;
    struct station_info * next_station = NULL;
    uint8_t* Pip = NULL;
    int num = wifi_softap_get_station_num();
    os_printf("station �ն����Ӹ���:%d\n", num);

    pStation_info = wifi_softap_get_station_info();
    while(num--)
    {
        pStation_info->bssid;
        os_printf("MAC:"MACSTR"\n",MAC2STR(pStation_info->bssid)) ;
        Pip = (uint8_t*)&pStation_info->ip.addr;
        os_printf("IP:%d.%d.%d.%d\n",Pip[0],Pip[1],Pip[2],Pip[3]);
        pStation_info = pStation_info->next.stqe_next;
    }
    wifi_softap_free_station_info(); // Free it by calling functions
    /*#define STATION_IF 0x00--------#define SOFTAP_IF 0x01*/
}

// #define STATION_IF 0x00
// #define SOFTAP_IF 0x01
bool Get_localhost_info(uint8_t mode)
{
    struct ip_info ST_ESP8266_IP;	// IP��Ϣ�ṹ��
    uint8_t* Pip = NULL;
    
    if(wifi_get_ip_info(mode,&ST_ESP8266_IP))	// ����2��IP��Ϣ�ṹ��ָ��
    {
        os_printf("localhost (����������):\n");
        Pip = (uint8_t*)& ST_ESP8266_IP.ip.addr;
        os_printf("IP:%d.%d.%d.%d\n",Pip[0],Pip[1],Pip[2],Pip[3]);
        return true;
    }
    return false;
}
	
