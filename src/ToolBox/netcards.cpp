/*
 ================================================
 *      ######
 *    #########                            ##
 *   ####    ###                           ##
 *   ##       ##                           ##
 *  ##         ## ##   ##   ####     ### ######
 *  ##         ## ##   ##  ######  ##### ######
 *  ##         ## ##   ## ###  ## ###      ##
 *  ##    #    ## ##   ## ##  ##  #####    ##
 *   ##   ##   ## ##   ## ## ##    #####   ##
 *   ###   ## ##  ##   ## ###   #     ##   ##
 *    ##########  ####### ####### ######   ##
 *      #### ##    ######   ####   ####    ##
 *            ##
 *             ##       CREATE: 2023-07-20
 *              #
 ================================================
        以太网网卡设备枚举小工具
 ================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment (lib, "iphlpapi.lib")

/*
=======================================
    主程序
=======================================
*/
int main ()
{
    ULONG               size;
    PIP_ADAPTER_INFO    list;
    PIP_ADAPTER_INFO    node;

    list = (PIP_ADAPTER_INFO)malloc(sizeof(IP_ADAPTER_INFO));
    if (list == NULL)
        return (-1);
    size = sizeof(IP_ADAPTER_INFO);
    if (GetAdaptersInfo(list, &size) == ERROR_BUFFER_OVERFLOW) {
        list = (PIP_ADAPTER_INFO)malloc(size);
        if (list == NULL)
            return (-1);
    }
    if (GetAdaptersInfo(list, &size) != NO_ERROR)
        return (-1);

    for (node = list; node != NULL; node = node->Next)
    {
        BYTE    mac[6];

        if (node->Type == MIB_IF_TYPE_ETHERNET) {
            printf("\nDESC: %s\n", node->Description);
            printf("\tNAME: %s\n", node->AdapterName);
            printf("\tIP: %s\n", node->IpAddressList.IpAddress.String);
            printf("\tMSK: %s\n", node->IpAddressList.IpMask.String);
            printf("\tGW: %s\n", node->GatewayList.IpAddress.String);
            memcpy(mac, node->Address, sizeof(mac));
            printf("\tMAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        }
    }
    return (0);
}
