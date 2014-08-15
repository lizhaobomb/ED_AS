// CSocketAPI.cpp
//

#include "CSocketAPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

int CSocketAPI::Accept(int fd, struct sockaddr_in * cliaddr)
{
	int clilen=sizeof(struct sockaddr_in);

	if(cliaddr==NULL){
		struct sockaddr_in client;
		return accept(fd,(struct sockaddr *)&client,(socklen_t *)&clilen);
	}else memset(cliaddr,0,sizeof(struct sockaddr_in));
	return accept(fd,(struct sockaddr *)cliaddr,(socklen_t *)&clilen);

}

int CSocketAPI::Connect(char * pszHostAddress, int port)
{
	struct sockaddr_in sockAddr; 
	int fd;
	struct hostent *lphost;
	int err;
	char on=1;

	//wVersionRequested = MAKEWORD( 2, 2 );
	//err = WSAStartup( wVersionRequested, &wsaData );
	//if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
    /* WinSock DLL.                                  */
	//	return -2;
	//}
 
	fd=  socket(AF_INET, SOCK_STREAM, 0);
	if(fd<0)
		return -1;
	
/*	int optval;
	int optlen=4;
	getsockopt(fd,SOL_SOCKET,SO_RCVBUF,(char *)&optval,&optlen);
	printf("%d\n",optval);
	optval=614400;
	setsockopt(fd,SOL_SOCKET,SO_RCVBUF,(char *)&optval,optlen);
	getsockopt(fd,SOL_SOCKET,SO_RCVBUF,(char *)&optval,&optlen);
	printf("%d\n",optval);
*/	
	memset(&sockAddr, 0,sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(port);
	sockAddr.sin_addr.s_addr = inet_addr(pszHostAddress);

	if (sockAddr.sin_addr.s_addr == 0)
	{
		lphost =(struct hostent *) gethostbyname(pszHostAddress);
		if (lphost != NULL)
			memcpy((char *)&sockAddr.sin_addr,(char *)(lphost->h_addr),lphost->h_length);
		else{
			close(fd);
			return -1;
		}
	}
	if(connect(fd,(struct sockaddr *)&sockAddr,sizeof(sockAddr))<0)
	{
		close(fd);
		return -1;
	}

	setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE,&on,sizeof(on));
	SetNoBlock(fd);
	return fd;
}

int CSocketAPI::CreateListenServer(int port)
{
   	int fd;
	struct sockaddr_in servaddr;
	char on=(char)1;

	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd<0){
	//	printf("socket error\n");
		return fd;
	}
	setsockopt(fd,SOL_SOCKET,SO_KEEPALIVE,&on,sizeof(on));
	SetNoBlock(fd);

	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(fd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
	{
		printf("bind error: %s\n",strerror(errno));
		close(fd);
		return -1;
	}
	if(listen(fd,10)<0){
		printf("listen error \n");
		close(fd);
		return -1;
	}else printf("begin listen\n");
	return fd;
}
void CSocketAPI::CloseSocket(int fd)
{
	close(fd);
}

int CSocketAPI::IsReadable(int fd, struct timeval * timeout)
{
	fd_set fds;
	int bReadible=1;
	while(1){
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		bReadible=select(fd+1,&fds,NULL,NULL,timeout);

		if(bReadible==0)/*time out*/
			return -2;
		else if(bReadible<0 && errno==EINTR)
			continue;
		else if(bReadible<0)
			return 0;
		if(FD_ISSET(fd,&fds))
			return 1;	
	}	
	return 0;
}

int CSocketAPI::IsWriteable(int fd, struct timeval * timeout)
{
	fd_set fds;
	int bWriteable=1;
	
	while(1){
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		bWriteable=select(fd+1,NULL,&fds,NULL,timeout);

	//	if(bWriteable<0)
	//		perror("select: ");

		if(bWriteable==0)/*time out*/
			return -2;
		//else if(bWriteable<0 && errno==EINTR)
		else if(bWriteable<0 && errno==EINTR)
			continue;
		else if(bWriteable<0)
			return 0;
		if(FD_ISSET(fd,&fds))
			return 1;	
	}	
	return 0;
}


int CSocketAPI::read_all(int fd, void * buf, int n)
{
	int nleft;
	int nbytes;
	char *ptr;
	long dwStartTicks;
	long ntm;

	time(&dwStartTicks);

	ptr=(char *)buf;
	nleft=n;
	for(;nleft>0;)
	{
		nbytes=recv(fd,ptr,nleft,0);

		if(nbytes<=0){
			
			if(errno!=EWOULDBLOCK && errno != EINTR)
			{
			//	perror("read_all ");
				return -1;
			}else nbytes=0;
		/*printf("read  %d\n",GetLocaltime()- dwStartTicks);*/		
			usleep(10000);
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

int CSocketAPI::ReadSockString(int fd,char * pszBuffer, int nBuf, char * pszTerminator)
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
		
		bReadible=IsReadable(fd,&timeout);
		if(!bReadible){
			pszBuffer[nReceived] = '\0';
			return 0;
		}
			
		/*receive the data from the socket*/
		nData = recv(fd,pszBuffer+nReceived, nBuf-nReceived,0);
		if(nData<=0)
		{
			if(errno ==EWOULDBLOCK || errno == EINTR){
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
int CSocketAPI::ReadSockResponse(int fd, char * pszBuffer, int nBuf, char * pszTerminator, int nExpectedCode)
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
	long dwStartTicks,ntm;

	timeout.tv_sec=RWTIMEOUT;
	timeout.tv_usec=0;
	time(&dwStartTicks);
	while (!bFoundTerminator )
	{
		/*timeout has occured*/
		
		bReadible=IsReadable(fd,&timeout);
		if(!bReadible){
			pszBuffer[nReceived] = '\0';
			return 0;
		}
			
		/*receive the data from the socket*/
		nData = recv(fd,pszBuffer+nReceived, nBuf-nReceived,0);
		if(nData<=0)
		{
			 if(errno==EWOULDBLOCK || errno==EINTR){
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

int CSocketAPI::SetNoBlock(int fd)
{
	
	unsigned long flag;
	flag=1;
	if(ioctl(fd,FIONBIO,&flag)<0)
	{
		return 0;
	}
	return 1;
}



int CSocketAPI::write_all(int fd, void * buf, int n)
{
	int nleft,nbytes;
	char *ptr;
	long dwStartTicks,ntm;

	time(&dwStartTicks);

	nleft=n;
	ptr=(char *)buf;
	for(;nleft>0;)
	{
		nbytes=send(fd,ptr,nleft,0);
		if(nbytes<=0){
			if(errno !=EWOULDBLOCK && errno != EINTR){
				perror("write_all :");
				return -1;
			}
			//else if(err==WSAEWOULDBLOCK)
			//	return n;
			else nbytes=0;
			usleep(10000);
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

long CSocketAPI::GetLocaltime()
{
	time_t tm;
	return time(&tm);
}

int CSocketAPI::GetHostname(char * host,int host_len)
{
 	return gethostname(host,host_len);
}
