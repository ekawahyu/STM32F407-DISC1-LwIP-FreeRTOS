#include <string.h>
#include "lwip/api.h"

static struct netconn *conn, *newconn;
static struct netbuf *buf;
static ip_addr_t *addr;
static unsigned short port;
char msg[100];
char response[100];

static void tcp_thread(void *arg)
{
	err_t err, accept_err;

	/* create a new netconn connection */
	conn = netconn_new(NETCONN_TCP);

	if (conn != NULL)
	{
		/* bind connection to port 7 (ECHO protocol) */
		err = netconn_bind(conn, IP_ADDR_ANY, 7);

		if (err == ERR_OK)
		{
			/* start listening on incoming connection */
			netconn_listen(conn);

			while (1)
			{
				/* grab new connection. */
				accept_err = netconn_accept(conn, &newconn);

				/* process the new connection. */
				if (accept_err == ERR_OK)
				{
					/* receive the data from the client */
					while (netconn_recv(newconn, &buf) == ERR_OK)
					{
						/* get client address */
						addr = netbuf_fromaddr(buf);
						/* get client port */
						port = netbuf_fromport(buf);
						/* process netconn buffer */
						do
						{
							/* get payload */
							strncpy (msg, buf->p->payload, buf->p->len + 1);
							int len = sprintf (response, "%s\n", msg);
							/* echo the payload back to client */
							netconn_write(newconn, response, len, NETCONN_COPY);
							/* clean buffer */
							memset (msg, 0, 100);
						}
						while (netbuf_next(buf) > 0);

						netbuf_delete(buf);
					}

					/* close and delete connection */
					netconn_close(newconn);
					netconn_delete(newconn);
				}
			}
		}
		else
		{
			netconn_delete(conn);
		}
	}
}

void tcp_echoserver_init(void)
{
	sys_thread_new("tcp_thread", tcp_thread, NULL, DEFAULT_THREAD_STACKSIZE, osPriorityNormal);
}