// CSocketAPI.h
//

#if !defined(CSocketAPI_h)
#define CSocketAPI_h

/*
 * a common class wrapping socket function
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include<sys/ioctl.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define SOCKTIMEOUT 300000/*microsecond*/
#define RWTIMEOUT 300/*read and write time out 5 minutes*/


class CSocketAPI
{
public:

    /*
     * accept connection.return >=0: client connection fd
     * <0: no connection
     * parameter:
     * int fd:listen fd
     * struct sockaddr_in * cliaddr: client ip address
     */
    int Accept(int fd, struct sockaddr_in * cliaddr);
 
    /*
     * connect to a socket server. connection is no blocking. return:
     * >0: connection fd
     * -1: socket error
     * parameter:
     * char * pszHostAddress: server ip address
     * int port: server port
     */
    int Connect(char * pszHostAddress, int port);
	/*close socket fd*/
	void CloseSocket(int fd);
    /*
     * create a socket server. the connection is no blocking.
     * return:
     * >0: listen fd
     * -1: socket error
     * parameter:
     * int port: bind port
     */
    int CreateListenServer(int port);
    /*
     * test the socket is readable.return :
     * 1--readable
     * 0--not readable
     * -2--time out
     * parameter:
     * int fd:connection fd
     * struct timeval * timeout: time out value
     */
    int IsReadable(int fd, struct timeval * timeout);
    /*
     * test the socket is writeable. see IsReadable
     */
    int IsWriteable(int fd, struct timeval * timeout);
    
    /*
     * read fixed bytes from socket to data buffer.
     * return:
     * >0: read bytes
     * -1: socket error
     * -2: time out
     * int fd: connection fd
     * void * buf: received data buffer
     * int n: read bytes
     */
    int read_all(int fd, void * buf, int n);
    /*
     * read sockets until find the terminator or read timeout.if
     * receive expected code, return 1, else return 0.
     * parameter:
     * int fd: connection fd
     * char *pszBuffer: received data buffer
     * int nBuf: max length of pszbuffer
     * char * pszTerminator:terminator,ex. "\r\n"
     * int nExpectedCode: expected code
     * 
     */
    int ReadSockResponse(int fd, char * pszBuffer, int nBuf, char * pszTerminator, int nExpectedCode);
	int ReadSockString(int fd,char * pszBuffer, int nBuf, char * pszTerminator);

    /*
     * set connection to no blocking mode.return 1:sucess,0:error.
     * parameter:
     * int fd:connection fd
     */
    int SetNoBlock(int fd);
   
    int write_all(int fd, void * buf, int n);
	long GetLocaltime();
	//ȡ��������host:��Ż�������host_len:host���ȣ�����0-�ɹ���<0:����
	int GetHostname(char * host,int host_len);

protected:


private:


};

#endif /* CSocketAPI_h */
