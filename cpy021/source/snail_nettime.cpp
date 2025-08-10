#include <stdio.h>
#include <stdlib.h>
#include <Winsock2.h>
#include <Windows.h>
#include <time.h>
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "snail_nettime.h"

struct   NTP_Packet  
{  
    int      Control_Word;     
    int      root_delay;     
    int      root_dispersion;     
    int      reference_identifier;     
    __int64 reference_timestamp;     
    __int64 originate_timestamp;     
    __int64 receive_timestamp;     
    int      transmit_timestamp_seconds;     
    int      transmit_timestamp_fractions;     
};  

int snail_ResetSystemTime(char* nettimesrv)
{
    WORD    wVersionRequested;  
    WSADATA wsaData;  
    unsigned long ul=1;

    if (NULL == nettimesrv || strlen(nettimesrv) < 1)
    {
        SNAIL_LOG_ERROR("reset system time failed, invalid argument.");
        return -1;
    }
    
    // 初始化版本  
    wVersionRequested = MAKEWORD( 1, 1 );  
    if (0!=WSAStartup(wVersionRequested, &wsaData))   
    {  
        WSACleanup();

        SNAIL_LOG_ERROR("Get network time failed: WSAStartup exception, errno:%d.",
            WSAGetLastError());
        return -1;  
    }  
    if (LOBYTE(wsaData.wVersion)!=1 || HIBYTE(wsaData.wVersion)!=1)   
    {  
        WSACleanup( );

        SNAIL_LOG_ERROR("Get network time failed: version not correct, errno:%d.",
            WSAGetLastError());
        return -1;
    }  
    
    // 这个IP是中国大陆时间同步服务器地址，可自行修改  
    SOCKET soc=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);  
    struct sockaddr_in addrSrv; 

    //http://www.ntp.org.cn/pool.php列表上的ntp时间服务器
    //106.186.122.232 106.187.100.179 129.250.35.251
    addrSrv.sin_addr.S_un.S_addr=inet_addr(nettimesrv);
    addrSrv.sin_family=AF_INET;  
    addrSrv.sin_port=htons(123);  
    
    ioctlsocket(soc,FIONBIO,(unsigned long *)&ul);//设置成非阻塞模式。

    NTP_Packet NTP_Send,NTP_Recv;   
    NTP_Send.Control_Word   =   htonl(0x0B000000);     
    NTP_Send.root_delay     =   0;     
    NTP_Send.root_dispersion   =   0;     
    NTP_Send.reference_identifier   =   0;     
    NTP_Send.reference_timestamp    =   0;     
    NTP_Send.originate_timestamp    =   0;     
    NTP_Send.receive_timestamp      =   0;     
    NTP_Send.transmit_timestamp_seconds     =   0;     
    NTP_Send.transmit_timestamp_fractions   =   0;   
    
    if(SOCKET_ERROR==sendto(soc,(const char*)&NTP_Send,sizeof(NTP_Send),  
        0,(struct sockaddr*)&addrSrv,sizeof(addrSrv))) 
    {  
        closesocket(soc);  

        SNAIL_LOG_ERROR("Get network time failed: request exception, errno:%d, nettimesrv:%s",
            WSAGetLastError(), nettimesrv);
        return FALSE;  
    }  
    int sockaddr_Size =sizeof(addrSrv);  
    if(SOCKET_ERROR==recvfrom(soc,(char*)&NTP_Recv,sizeof(NTP_Recv),  
        0,(struct sockaddr*)&addrSrv,&sockaddr_Size)) 
    {  
        closesocket(soc);  

        SNAIL_LOG_ERROR("Get network time failed: response exception, errno:%d, nettimesrv:%s",
            WSAGetLastError(), nettimesrv);
        return -1;  
    }  
    closesocket(soc);  
    WSACleanup();  
    
    SYSTEMTIME  newtime;  
    float       Splitseconds;  
    struct      tm  *lpLocalTime;  
    time_t      ntp_time;  
    
    // 获取时间服务器的时间  
    ntp_time    = ntohl(NTP_Recv.transmit_timestamp_seconds)-2208988800;  
    lpLocalTime = localtime(&ntp_time);  
    if(lpLocalTime == NULL)  
    {  
        SNAIL_LOG_ERROR("Set network time failed, nettimesrv:%s", nettimesrv);
        return -1;  
    }  
    
    // 获取新的时间  
    newtime.wYear      =lpLocalTime->tm_year+1900;  
    newtime.wMonth     =lpLocalTime->tm_mon+1;  
    newtime.wDayOfWeek =lpLocalTime->tm_wday;  
    newtime.wDay       =lpLocalTime->tm_mday;  
    newtime.wHour      =lpLocalTime->tm_hour;  
    newtime.wMinute    =lpLocalTime->tm_min;  
    newtime.wSecond    =lpLocalTime->tm_sec;  
    
    // 设置时间精度  
    Splitseconds=(float)ntohl(NTP_Recv.transmit_timestamp_fractions);  
    Splitseconds=(float)0.000000000200 * Splitseconds;  
    Splitseconds=(float)1000.0 * Splitseconds;  
    newtime.wMilliseconds   =   (unsigned   short)Splitseconds;  
    
    // 修改本机系统时间  
    SetLocalTime(&newtime);  

    SNAIL_LOG_DEBUG("Set network time success, nettimesrv:%s", nettimesrv);
    return 0;  
}
int snail_UpdateSysTime()
{
	WORD    wVersionRequested;  
    WSADATA wsaData;  
    unsigned long ul=1;

    // 初始化版本  
    wVersionRequested = MAKEWORD( 1, 1 );  
    if (0!=WSAStartup(wVersionRequested, &wsaData))   
    {  
        WSACleanup();

        SNAIL_LOG_ERROR("Get network time failed: WSAStartup exception, errno:%d.",
            WSAGetLastError());
        return -1;  
    }  
    if (LOBYTE(wsaData.wVersion)!=1 || HIBYTE(wsaData.wVersion)!=1)   
    {  
        WSACleanup( );

        SNAIL_LOG_ERROR("Get network time failed: version not correct, errno:%d.",
            WSAGetLastError());
        return -1;
    }  
    
    // 这个IP是中国大陆时间同步服务器地址，可自行修改  
    SOCKET soc=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);  
    struct sockaddr_in addrSrv; 

    //http://www.ntp.org.cn/pool.php列表上的ntp时间服务器
    //106.186.122.232 106.187.100.179
    addrSrv.sin_addr.S_un.S_addr=inet_addr("129.250.35.251");
    addrSrv.sin_family=AF_INET;  
    addrSrv.sin_port=htons(123);  
    
    ioctlsocket(soc,FIONBIO,(unsigned long *)&ul);//设置成非阻塞模式。

    NTP_Packet NTP_Send,NTP_Recv;   
    NTP_Send.Control_Word   =   htonl(0x0B000000);     
    NTP_Send.root_delay     =   0;     
    NTP_Send.root_dispersion   =   0;     
    NTP_Send.reference_identifier   =   0;     
    NTP_Send.reference_timestamp    =   0;     
    NTP_Send.originate_timestamp    =   0;     
    NTP_Send.receive_timestamp      =   0;     
    NTP_Send.transmit_timestamp_seconds     =   0;     
    NTP_Send.transmit_timestamp_fractions   =   0;   
    
    if(SOCKET_ERROR==sendto(soc,(const char*)&NTP_Send,sizeof(NTP_Send),  
        0,(struct sockaddr*)&addrSrv,sizeof(addrSrv))) 
    {  
        closesocket(soc);  

        SNAIL_LOG_ERROR("Get network time failed: request exception, errno:%d.",
            WSAGetLastError());
        return FALSE;  
    }  
    int sockaddr_Size =sizeof(addrSrv);  
    if(SOCKET_ERROR==recvfrom(soc,(char*)&NTP_Recv,sizeof(NTP_Recv),  
        0,(struct sockaddr*)&addrSrv,&sockaddr_Size)) 
    {  
        closesocket(soc);  

        SNAIL_LOG_ERROR("Get network time failed: response exception, errno:%d.",
            WSAGetLastError());
        return -1;  
    }  
    closesocket(soc);  
    WSACleanup();  
    
    SYSTEMTIME  newtime;  
    float       Splitseconds;  
    struct      tm  *lpLocalTime;  
    time_t      ntp_time;  
    
    // 获取时间服务器的时间  
    ntp_time    = ntohl(NTP_Recv.transmit_timestamp_seconds)-2208988800;  
    lpLocalTime = localtime(&ntp_time);  
    if(lpLocalTime == NULL)  
    {  
        SNAIL_LOG_ERROR("Set network time failed.");
        return -1;  
    }  
    
    // 获取新的时间  
    newtime.wYear      =lpLocalTime->tm_year+1900;  
    newtime.wMonth     =lpLocalTime->tm_mon+1;  
    newtime.wDayOfWeek =lpLocalTime->tm_wday;  
    newtime.wDay       =lpLocalTime->tm_mday;  
    newtime.wHour      =lpLocalTime->tm_hour;  
    newtime.wMinute    =lpLocalTime->tm_min;  
    newtime.wSecond    =lpLocalTime->tm_sec;  
    
    // 设置时间精度  
    Splitseconds=(float)ntohl(NTP_Recv.transmit_timestamp_fractions);  
    Splitseconds=(float)0.000000000200 * Splitseconds;  
    Splitseconds=(float)1000.0 * Splitseconds;  
    newtime.wMilliseconds   =   (unsigned   short)Splitseconds;  
    
    // 修改本机系统时间  
    SetLocalTime(&newtime);  

    SNAIL_LOG_DEBUG("Set network time success.");
    return 0;  
}