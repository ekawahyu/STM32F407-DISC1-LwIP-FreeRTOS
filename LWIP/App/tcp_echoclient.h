#ifndef __TCP_ECHOCLIENT_H__
#define __TCP_ECHOCLIENT_H__

#define DEST_IP_ADDR0   (uint8_t) 192
#define DEST_IP_ADDR1   (uint8_t) 168
#define DEST_IP_ADDR2   (uint8_t) 67
#define DEST_IP_ADDR3   (uint8_t) 1

#define DEST_PORT       ((uint16_t)7U)
 
/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
// #define IP_ADDR0   ((uint8_t) 192U)
// #define IP_ADDR1   ((uint8_t) 168U)
// #define IP_ADDR2   ((uint8_t) 67U)
// #define IP_ADDR3   ((uint8_t) 100U)

/*NETMASK*/
// #define NETMASK_ADDR0   ((uint8_t) 255U)
// #define NETMASK_ADDR1   ((uint8_t) 255U)
// #define NETMASK_ADDR2   ((uint8_t) 255U)
// #define NETMASK_ADDR3   ((uint8_t) 0U)

/*Gateway Address*/
// #define GW_ADDR0   ((uint8_t) 192U)
// #define GW_ADDR1   ((uint8_t) 168U)
// #define GW_ADDR2   ((uint8_t) 67U)
// #define GW_ADDR3   ((uint8_t) 1U)

void tcp_echoclient_process(void);

#endif /* __TCP_ECHOCLIENT_H__ */