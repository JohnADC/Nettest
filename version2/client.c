#include "headers.h"
#include "wrappers.h"

volatile int misses = 0;

void missed_alarm(int signum) {
	    /* we missed a timer signal, so won't be sending packets fast enough!!! */
	    //write(2, "Missed Alarm!\n", 14);
	    ++misses;
}

void commandInfo(){
	fprintf(stderr,"usage: ./client hostname pps [-p port] [-s packet_size] [-t time]\n");
    exit(1);
}

void error(char* err){
	fprintf(stderr, "%s\n", err);
	exit(1);
}

int main(int argc, char* argv[]){

	int sockfd;
    struct addrinfo hints, *servinfo, *p;
   	char *port = PORT;
   	char *address;
   	int packet_size = BUFF;
   	int pps = DEFAULTPACKETNUM;
   	int sending_time = DEFAULTTIME;
   	int pause = 0;
   	int ch;


   	if(argc < 3 || argc > 5){
        commandInfo();
    }

    if(optind > argc){
    	commandInfo();
    }

    address = argv[1];
    pps = atoi(argv[2]);


    while((ch = getopt(argc, argv, "p:s:t:e")) != -1){
			switch(ch){
					case 'p':	port = optarg;
								break;
					case 's':	packet_size = atoi(optarg);
								break;			
					case 't':	sending_time = atoi(optarg);
								break;
					default:	commandInfo();
			}
	}

    if(pps > MAXPACKETNUM){
    	error("error: Maximum pps is 1000000");
    }
    if(pps < 1){
    	error("error: Minimum pps is 1");
    }
    if(packet_size > MAXBUFF){
    	error("error: Maximum packet size is 16376");
    }
    if(!((packet_size%4) == 0)){
    	error("error: Packet size must be divisable with 4");	
    }
    if(packet_size < MINBUFF){
    	error("error: Minimum packet size is 1");
    }
    if(sending_time > MAXTIME){
    	error("error: Maximum sending time is 10s");
    }
    if(sending_time < MINTIME){
    	error("error: Minimum sending time is 1s");
    }

    pause = ( MICRO / pps );

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    
    Getaddrinfo(address, port, &hints, &servinfo);

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }


    if (p == NULL) {
	    fprintf(stderr, "talker: failed to create socket\n");
	    return 2;
    }


    int go;
    int pnumber = 0;
    int* buf;
    int data[3];
    struct timeval start, stop;

    buf = malloc(packet_size * sizeof(int));
    
    memset(buf, 0, sizeof(&buf));
    memset(data, 0, sizeof(data));
    
    data[0] = htonl(pps);
    data[1] = htonl(packet_size);
    data[2] = htonl(sending_time);

    Sendto(sockfd, data, sizeof data, 0, p->ai_addr, p->ai_addrlen);

    Recvfrom(sockfd, &go, sizeof go , 0, p->ai_addr, &(p->ai_addrlen));

    struct itimerval timer;
	timer.it_interval.tv_sec = timer.it_value.tv_sec = 0;
	timer.it_interval.tv_usec = timer.it_value.tv_usec = pause;
	if (setitimer(ITIMER_REAL, &timer, 0) < 0) {
	    perror("setitimer");
	    exit(1); 
	}

	signal(SIGALRM, missed_alarm);

	sigset_t alarm_sig;
	int signum;
	sigemptyset(&alarm_sig);
	sigaddset(&alarm_sig, SIGALRM);

	gettimeofday(&start, NULL);
	
	while(sending_time--){
		pnumber = 0;
		while (pps > pnumber) {
		    sigwait(&alarm_sig, &signum);
		    
		    ++pnumber;
			buf[0] = pnumber;

			Sendto(sockfd, buf, sizeof buf, 0, p->ai_addr, p->ai_addrlen);
		}
	}
	
	gettimeofday(&stop, NULL);
	
	printf("Sending time %f\n", ((stop.tv_sec-start.tv_sec)*1000000 + (stop.tv_usec-start.tv_usec)) / 1000000.0);
	printf("Number of missed alarms: %d\n", misses);
	
    freeaddrinfo(servinfo);
    close(sockfd);


	return 0;
}