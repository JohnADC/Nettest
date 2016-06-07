void Bind(int sockfd, const struct sockaddr *myaddr, int addrlen);

int Socket(int domain, int type, int protocol);
	
void Getnameinfo(const struct sockaddr *myaddr, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags);

void Getaddrinfo(const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result);

int Sendto(int sockfd, const void *msg, size_t len, int flags, const struct sockaddr *to, socklen_t tolen);

int Recvfrom(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, socklen_t *fromlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void Connect(int sockfd, const struct sockaddr *name, socklen_t namelen);

int Send(int sockfd, const void *msg, size_t len, int flags);

int Recv(int sockfd, void *buf, size_t len, int flags);

int Write(int sockfd, void *buf, int num);

int Read(int sockfd, void *buf, int max);

void Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlen);

void Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);

ssize_t Writen(int fd,const void *vptr, size_t n);

ssize_t Readn(int fd, void *vptr, size_t n);
