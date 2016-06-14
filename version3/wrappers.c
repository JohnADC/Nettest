#include "headers.h"
#include "wrappers.h"


void Bind(int sockfd, const struct sockaddr *myaddr, int addrlen){
	int error;
	error=bind(sockfd, myaddr, addrlen);
	if(error == -1){
		perror("Bind");
		exit(1);
	}
}

void Listen(int sockfd, int backlog){
	int error;
	error=listen(sockfd, backlog);
	if(error == -1){
		perror("Listen");
		exit(1);
	}
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
	int n;
	n=accept(sockfd, addr, addrlen);
	if(n == -1){
		perror("Accept");
		exit(1);
	}
	return n;	
}

int Socket(int domain, int type, int protocol){
	int n;
	n=socket(domain, type, protocol);
	if(n == -1){
		perror("Socket");
		exit(1);
	}
	return n;	
}

void Connect(int sockfd, const struct sockaddr *name, socklen_t namelen){
	int error;
	error = connect(sockfd, name, namelen);
	if(error == -1){
		perror("Connect");
		exit(1);
	}
}
	
void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags){
	int error;
	error=getnameinfo(sa, salen, host, hostlen, serv, servlen, flags);
	if(error){
		perror("Getnameinfo");
		exit(1);
	}
}

void Getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result){
	int error;
	error=getaddrinfo(hostname, service, hints, result);
	if(error){
		perror("Getaddrinfo");
		exit(1);
	}
}


int Sendto(int sockfd, const void *msg, size_t len, int flags, const struct sockaddr *to, socklen_t tolen){
	int n;
	n=sendto(sockfd, msg, len, flags, to, tolen);
	if(n == ENOBUFS){
		return n;
	}
	if(n == -1){
		perror("Sendto");
		exit(1);
	}
	return n;
}

int Recvfrom(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, socklen_t *fromlen){
	int n;
	n=recvfrom(sockfd, buf,len, flags, from, fromlen);
	if(n == -1){
		if(errno == EWOULDBLOCK){
			return -1;
		}
		perror("Recvfrom");
		exit(1);
	}
	
	return n;
}

int Recv(int sockfd, void *buf, size_t len, int flags){
	int n;
	n=recv(sockfd, buf, len, flags);
	if(n == -1){
		perror("Recv");
		exit(1);
	}
	return n;
}

int Send(int sockfd, const void *msg, size_t len, int flags){
	int n;
	n=send(sockfd, msg, len, flags);
	if(n == -1){
		perror("Send");
		exit(1);
	}
	return n;
}

int Read(int sockfd, void *buf, int max){
	int n;
	n=read(sockfd, buf, max);
	if(n == -1){
		perror("Read");
		exit(1);
	}
	return n;
}

int Write(int sockfd, void *buf, int num){
	int n;
	n=write(sockfd, buf, num);
	if(n == -1){
		perror("Write");
		exit(1);
	}
	return n;
}

void Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlen){
    if (getsockopt(fd, level, optname, optval, optlen) < 0)
        errx(1, "getsockopt error");
}

void Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen){
    if (setsockopt(fd, level, optname, optval, optlen) < 0)
        errx(1, "setsockopt error");
}

ssize_t Readn(int fd, void *vptr, size_t n){
	
	size_t nleft;
	ssize_t nread;
	char *ptr;
	
	ptr=vptr;
	nleft=n;
	
	while(nleft>0){
		if((nread=Read(fd, ptr, nleft)) <0){
			if(errno==EINTR)
				nread=0;
			else
				return (-1);
		}else if(nread==0){
			break;
		}
		nleft -=nread;
		ptr   += nread;
	}
	return (n-nleft);
}

ssize_t Writen(int fd,const void *vptr, size_t n){
	
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;
	
	ptr=vptr;
	nleft=n;
	
	while(nleft>0){
		if((nwritten=write(fd, ptr, nleft)) <=0){
			if(nwritten<0 && errno==EINTR)
				nwritten=0;
			else
				return (-1);
		}
		nleft -=nwritten;
		ptr   += nwritten;
	}
	return (n);
}
