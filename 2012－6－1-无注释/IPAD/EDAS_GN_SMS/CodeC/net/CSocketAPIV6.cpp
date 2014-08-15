// CSocketAPIV6.cpp
//

#include "stdafx.h"

#include "CSocketAPIV6.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

LPSTR DecodeError(int ErrorCode)
{
    static char Message[1024];

    // If this program was multi-threaded, we'd want to use
    // FORMAT_MESSAGE_ALLOCATE_BUFFER instead of a static buffer here.
    // (And of course, free the buffer when we were done with it)

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
                  FORMAT_MESSAGE_MAX_WIDTH_MASK,
                  NULL, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPSTR)Message, 1024, NULL);
    return Message;
}

int CSocketAPIV6::Accept(int fd, struct sockaddr_storage * cliaddr)
{
  	int clilen;

	clilen=sizeof(struct sockaddr_storage);

	if(cliaddr==NULL){
		struct sockaddr_storage client;
		return accept(fd,(struct sockaddr *)&client,&clilen);
	}else memset(cliaddr,0,clilen);
	return accept(fd,(struct sockaddr *)cliaddr,&clilen);

}

int CSocketAPIV6::Connect(char * pszHostAddress, int port)
{
	//struct sockaddr_in sockAddr; 
//	struct sockaddr_in6 sockAddr;
	int fd;
//	struct hostent *lphost;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
    ADDRINFO hints,* ai,*addr;
	char sport[100];

	char on=1;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
    /* WinSock DLL.                                  */
		return -2;
	}
	
	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET6;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_protocol=IPPROTO_TCP;
//	hints.ai_flags=AI_NUMERICHOST;
	sprintf(sport,"%d",port);

	err=getaddrinfo((LPCTSTR )pszHostAddress,sport,&hints,&addr);
	if(err!=0){
		WSACleanup();
		return -1;
	}

    for (ai=addr;ai!=NULL;ai=ai->ai_next)
	{

		fd = socket(ai->ai_family,ai->ai_socktype,ai->ai_protocol);

		if(connect(fd,ai->ai_addr,ai->ai_addrlen)!= SOCKET_ERROR)
			break;
    
		err = WSAGetLastError();
		char host[128];
        if (getnameinfo(ai->ai_addr, ai->ai_addrlen, host,
                        sizeof(host), NULL, 0, NI_NUMERICHOST) != 0)
            strcpy(host, "<unknown>");
        printf( "connect() to %s failed with error %d: %s\n",
               host, err, DecodeError(err));

		closesocket(fd);
	}
	if(ai==NULL){
		WSACleanup();
        return -1;
	}

	setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE,&on,sizeof(on));
	SetNoBlock(fd);

    freeaddrinfo(addr);

	return fd;
}

int CSocketAPIV6::CreateListenServer(int * fd,int port)
{
    ADDRINFO hints;
	ADDRINFO * ai,*addr;
	char sport[100];
	char on=(char)1;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	int j,cfd;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
    /* WinSock DLL.                                  */
		return -2;
	}
	sprintf(sport,"%d",port);

	memset(&hints,0,sizeof(hints));
	hints.ai_family=AF_INET6;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_protocol=IPPROTO_TCP;
	hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;

    err = getaddrinfo(NULL, sport, &hints, &addr);
    if (err != 0) {
		printf("%s\n",gai_strerror(err));
        WSACleanup();
        return -1;
    }

    //
    // For each address getaddrinfo returned, we create a new socket,
    // bind that address to it, and create a queue to listen on.
    //
	j=0;
	ai=addr;
	while(ai!=NULL)
	{
        // Highly unlikely, but check anyway.
        if (j == FD_SETSIZE) {
            printf("getaddrinfo returned more addresses than we could use.\n");
            break;
        }

        // This example only supports PF_INET and PF_INET6.
        if ((ai->ai_family != PF_INET) && (ai->ai_family != PF_INET6)){
			ai = ai->ai_next;
            continue;
		}

        // Open a socket with the correct address family for this address.
        cfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (cfd == INVALID_SOCKET){
			ai = ai->ai_next;
			continue;
        }
	
		setsockopt(cfd,SOL_SOCKET,SO_KEEPALIVE,&on,sizeof(on));
		SetNoBlock(cfd);

        //
        // bind() associates a local address and port combination
        // with the socket just created. This is most useful when
        // the application is a server that has a well-known port
        // that clients know about in advance.
        //
        if (bind(cfd, ai->ai_addr, ai->ai_addrlen) == SOCKET_ERROR) {
			closesocket(cfd);
			ai=ai->ai_next;			
			continue;
        }

        //
        // So far, everything we did was applicable to TCP as well as UDP.
        // However, there are certain fundamental differences between stream
        // protocols such as TCP and datagram protocols such as UDP.
        //
        // Only connection orientated sockets, for example those of type
        // SOCK_STREAM, can listen() for incoming connections.
        //
         if (listen(cfd, 10) <0 )
		 {
           	closesocket(cfd);
			ai=ai->ai_next;
            continue;
        }
		fd[j]=cfd;
        printf("'Listening' on port %s,  protocol family %s\n",
               sport,(ai->ai_family == PF_INET) ? "PF_INET" : "PF_INET6");
		j++;
	    ai = ai->ai_next;
	}
    freeaddrinfo(addr);

	if(j==0)
		return -1;
	return j;
}
void CSocketAPIV6::CloseSocket(int fd)
{
	closesocket(fd);
}

int CSocketAPIV6::IsReadable(int * fd, int fdnum,struct timeval * timeout)
{
	fd_set fds;
	int bReadible=1;
	int err;
	int max=fd[0];
	int i;
	for(i=1;i<fdnum;i++)
	{
		if(max<fd[i])
			max=fd[i];
	}

	while(1){
		FD_ZERO(&fds);
		for( i=0;i<fdnum;i++)
			FD_SET(fd[i], &fds);

		bReadible=select(max+1,&fds,NULL,NULL,timeout);

	//	if(bReadible<0)
	//		perror("select: ");
		 err=WSAGetLastError();

		if(bReadible==0)/*time out*/
			return -2;
		else if(bReadible<0 && err==WSAEINTR)
			continue;
		else if(bReadible<0)
			return 0;
		for(i=0;i<fdnum;i++)
		{
			if(FD_ISSET(fd[i],&fds))
				return fd[i];	
		}
	}	
	return 0;
}

int CSocketAPIV6::IsWriteable(int * fd,int fdnum, struct timeval * timeout)
{
	fd_set fds;
	int bWriteable=1;
	int i;
	int max;

	for(i=1;i<fdnum;i++)
	{
		if(max<fd[i])
			max=fd[i];
	}
	while(1){
		FD_ZERO(&fds);
		for(i=0;i<fdnum;i++)
			FD_SET(fd[i], &fds);

		bWriteable=select(max,NULL,&fds,NULL,timeout);

	//	if(bWriteable<0)
	//		perror("select: ");

		int err=WSAGetLastError();

		if(bWriteable==0)/*time out*/
			return -2;
		//else if(bWriteable<0 && errno==EINTR)
		else if(bWriteable<0 && err==WSAEINTR)
			continue;
		else if(bWriteable<0)
			return 0;
		for(i=0;i<fdnum;i++)
		{
			if(FD_ISSET(fd[i],&fds))
				return fd[i];	
		}
	}	
	return 0;
}


int CSocketAPIV6::read_all(int fd, void * buf, int n)
{
	int nleft;
	int nbytes;
	char *ptr;
	long dwStartTicks;
	long ntm;
	int err;

	time(&dwStartTicks);

	ptr=(char *)buf;
	nleft=n;
	for(;nleft>0;)
	{
		nbytes=recv(fd,ptr,nleft,0);

		if(nbytes<=0){
			err=WSAGetLastError();
			if(err!=WSAEWOULDBLOCK && err != WSAEINTR)
			{
			//	perror("read_all ");
				return -1;
			}else nbytes=0;
		/*printf("read  %d\n",GetLocaltime()- dwStartTicks);*/		
			Sleep(10);
			time(&ntm);
			if(ntm- dwStartTicks >  RWTIMEOUT)
			{
		//		printf("read time out\n");
				return -2;
			}
		}
		nleft-=nbytes;
		ptr+=nbytes;
	}
	return (n-nleft);
}
int CSocketAPIV6::ReadSockString(int fd,char * pszBuffer, int nBuf, char * pszTerminator)
{
	/*retrieve the reponse using until we
get the terminator or a timeout occurs*/
	int bReadible;
	struct timeval timeout;

	int bFoundTerminator = 0;
	int nReceived = 0;
	/*long dwStartTicks = Clock();*/
	int nData;
	long dwStartTicks,ntm;

	timeout.tv_sec=RWTIMEOUT;
	timeout.tv_usec=0;
	time(&dwStartTicks);
	while (!bFoundTerminator )
	{
		/*timeout has occured*/
		
		bReadible=IsReadable(&fd,1,&timeout);
		if(!bReadible){
			pszBuffer[nReceived] = '\0';
			return 0;
		}
			
		/*receive the data from the socket*/
		nData = recv(fd,pszBuffer+nReceived, nBuf-nReceived,0);
		if(nData<=0)
		{
			int	 err=WSAGetLastError();

			if(err==WSAEWOULDBLOCK || err==WSAEINTR){
				time(&ntm);
				if(ntm- dwStartTicks >  RWTIMEOUT)
					return 0;
				
				continue;
			}else {
				pszBuffer[nReceived] = '\0';
				return 0;	
			}
		}

		nReceived += nData;					

		pszBuffer[nReceived] = '\0';												
		bFoundTerminator = (strstr(pszBuffer, pszTerminator) != NULL);
		if(bFoundTerminator)
			break;
		time(&ntm);
		if(ntm- dwStartTicks >  RWTIMEOUT)
			return 0;
	}

	/*Remove the terminator from the response data*/
	pszBuffer[nReceived - strlen(pszTerminator)] = '\0';

	return 1;
}
int CSocketAPIV6::ReadSockResponse(int fd, char * pszBuffer, int nBuf, char * pszTerminator, int nExpectedCode)
{
	/*retrieve the reponse using until we
get the terminator or a timeout occurs*/
	int bReadible;
	struct timeval timeout;

	int bFoundTerminator = 0;
	int nReceived = 0;
	/*long dwStartTicks = Clock();*/
	int nData;
	char sCode[4];
	int nCommandResponseCode;
	int err;
	long dwStartTicks,ntm;

	timeout.tv_sec=RWTIMEOUT;
	timeout.tv_usec=0;
	time(&dwStartTicks);
	while (!bFoundTerminator )
	{
		/*timeout has occured*/
		
		bReadible=IsReadable(&fd,1,&timeout);
		if(!bReadible){
			pszBuffer[nReceived] = '\0';
			return 0;
		}
			
		/*receive the data from the socket*/
		nData = recv(fd,pszBuffer+nReceived, nBuf-nReceived,0);
		if(nData<=0)
		{
			 err=WSAGetLastError();
			 if(err==WSAEWOULDBLOCK || err==WSAEINTR){
				 time(&ntm);
				 if(ntm- dwStartTicks >  RWTIMEOUT)
					return 0;
				 continue;
			 } else {
				pszBuffer[nReceived] = '\0';
				return 0;	
			}
		}
	
		nReceived += nData;					

		pszBuffer[nReceived] = '\0';												
		bFoundTerminator = (strstr(pszBuffer, pszTerminator) != NULL);
		if(bFoundTerminator)
			break;
		time(&ntm);
		if(ntm- dwStartTicks >  RWTIMEOUT)
			return 0;
	}

	/*Remove the terminator from the response data*/
	pszBuffer[nReceived - strlen(pszTerminator)] = '\0';

/*printf("%s \n",pszBuffer);*/
	/*determine if the response is an error*/
	strncpy(sCode, pszBuffer, 3);
	sCode[3] = '\0';
	sscanf(sCode, "%d", &nCommandResponseCode);
	if(nCommandResponseCode == nExpectedCode)
		return 1;
	return 0;
}

int CSocketAPIV6::SetNoBlock(int fd)
{
	unsigned long flag;
	int err;
	flag=1;
	if(ioctlsocket(fd,FIONBIO,&flag)<0)
	{
		err=WSAGetLastError();
		return 0;
	}
	return 1;
}



int CSocketAPIV6::write_all(int fd, void * buf, int n)
{
	int nleft,nbytes;
	char *ptr;
	long dwStartTicks,ntm;
	int err;

	time(&dwStartTicks);

	nleft=n;
	ptr=(char *)buf;
	for(;nleft>0;)
	{
		nbytes=send(fd,ptr,nleft,0);
		if(nbytes<=0){
			err=WSAGetLastError();
			if(err!=WSAEWOULDBLOCK && err != WSAEINTR){
				perror("write_all :");
				return -1;
			}
			//else if(err==WSAEWOULDBLOCK)
			//	return n;
			else nbytes=0;
			Sleep(10);
			time(&ntm);
			if(ntm- dwStartTicks >  RWTIMEOUT)
			{
		//		printf("write time out\n");
				return -1;
			}
		}
		nleft-=nbytes;
		ptr+=nbytes;
	}
	return n;
}

long CSocketAPIV6::GetLocaltime()
{
	time_t tm;
	return time(&tm);
}

int CSocketAPIV6::GetHostname(char * host,int host_len)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return -2;
	}
 	return gethostname(host,host_len);
}
