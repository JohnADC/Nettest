#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <ctype.h>
#include <dirent.h>
#include <getopt.h>
#include <sys/time.h>


#define BACKLOG 10
#define PORT "10000"
#define MAXBUFF 65505
#define BUFF 1470
#define MINBUFF 1
#define MAXPACKETNUM 1000000
#define DEFAULTPACKETNUM 100000
#define MAXTIME 1000
#define DEFAULTTIME 1
#define MICRO 1000000L


