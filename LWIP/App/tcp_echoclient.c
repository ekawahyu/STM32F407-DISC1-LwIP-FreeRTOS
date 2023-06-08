#include <stdio.h>
#include <string.h>
#include "lwip/api.h"

static struct netconn *conn;
static struct netbuf *buf;
static ip_addr_t *addr, dest_addr;
static unsigned short port, dest_port;
static unsigned int message_count;
char msg[100];
char request[100];
char response[100];

static void tcp_thread(void *arg)
{
	err_t err, connect_error;

	/* create a new netconn connection */
	conn = netconn_new(NETCONN_TCP);

	if (conn != NULL)
	{
		/* bind connection to client random port number */
		err = netconn_bind(conn, IP_ADDR_ANY, 0);

		if (err == ERR_OK)
		{
			/* echo sever IP address */
			IP4_ADDR( &dest_addr, DEST_IP_ADDR0, DEST_IP_ADDR1, DEST_IP_ADDR2, DEST_IP_ADDR3 );
            dest_port = DEST_PORT;

			/* connect to server */
			connect_error = netconn_connect(conn, &dest_addr, dest_port);

			/* connection to server is established */
			if (connect_error == ERR_OK)
			{
				/* receive echo response from server */
				while (netconn_recv(conn, &buf) == ERR_OK)
				{
					/* get client address */
					addr = netbuf_fromaddr(buf);
					/* get client port */
					port = netbuf_fromport(buf);
					/* process netconn buffer */
					do
					{
						/* increment message counter */
						message_count++;
						/* get payload */
						memset (response, 0, 100);
						strncpy (msg, buf->p->payload, buf->p->len);
						/* print out echo response over console */
						snprintf((char*)response, buf->p->len+1, "%s", (char*)buf->p->payload);
						printf("res: %s\n", (char*)response);
						/* clean buffer */
						memset (msg, 0, 100);
					}
					while (netbuf_next(buf) > 0);
					netbuf_delete(buf);
				}
			}
			else
			{
				/* close and delete connection */
				netconn_close(conn);
				netconn_delete(conn);
			}
		}
		else
		{
			netconn_delete(conn);
		}
	}
}

void tcp_echoclient_send(void)
{
	/* compose request */
	sprintf (request, "sending tcp client message %d", message_count);
    /* print out request to console */
    printf("req: %s\n", (char*)request);
	/* send the data to the connected connection */
    uint8_t test = strlen(request);
	netconn_write(conn, request, strlen(request), NETCONN_COPY);
}

void tcp_echoclient_connect(void)
{
	sys_thread_new("tcp_thread", tcp_thread, NULL, DEFAULT_THREAD_STACKSIZE, osPriorityNormal);
}

void tcp_echoclient_process(void)
{
	static uint8_t state;
	if (state)
	{
		tcp_echoclient_send();
	}
	else
	{
		state = 1;
		tcp_echoclient_connect();
	}
}