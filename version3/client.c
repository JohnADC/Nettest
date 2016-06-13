#include "headers.h"
#include "wrappers.h"

#define CEILING_POS(X) ((X-(int)(X)) > 0 ? (int)(X+1) : (int)(X))

volatile int misses = 0;

int findn(int num)
{
    if ( num <= 10 )
      return 1;
    if ( num <= 100 )
      return 2;
 	if ( num <= 1000 )
      return 3;
    if ( num <= 10000 )
      return 4;
    if ( num <= 100000 )
      return 5;
    if ( num <= 1000000 )
      return 6;  

  	return 0;
}

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
   	int bursts = 0, burst_packets = 0;


   	if(argc < 3 || argc > 5){
        commandInfo();
    }

    if(optind > argc){
    	commandInfo();
    }


    while((ch = getopt(argc, argv, "p:s:t:")) != -1){
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

    address = argv[optind];
    pps = atoi(argv[optind+1]);


    if(pps > MAXPACKETNUM){
    	error("error: Maximum pps is 10000000");
    }
    if(pps < MINPACKETNUM){
    	error("error: Minimum pps is 1");
    }
    if(!((pps%10) == 0)){
    	error("error: PPS must be divisabl with 10");
    }
    if(packet_size > MAXPACKETSIZE){
    	error("error: Maximum packet size is 65505");
    }
    if(packet_size < MINPACKETSIZE){
    	error("error: Minimum packet size is 1");
    }
    if(sending_time > MAXTIME){
    	error("error: Maximum sending time is 120s");
    }
    if(sending_time < MINTIME){
    	error("error: Minimum sending time is 1s");
    }

    
    int n = findn(pps);
    if(n>3){
    	bursts = 1000;
    } else if(n != 1) {
    	bursts = 10;
    } else {
        bursts = 1;
    }
    burst_packets = pps / bursts;
    int b=bursts;
    //int bp = burst_packets;
    pause = (MICRO / bursts);
	
	//OVO JE SVE PREVARA

	/*double interresult=((pps * packet_size) / (double)SENDBUF);
    bursts = CEILING_POS(interresult);
    int b=bursts;

    double justdoit=(MICRO / (double)bursts);	
    pause = CEILING_POS(justdoit);

    burst_packets = ( SENDBUF / packet_size );
    

    if((pps * packet_size) < 200000){
    	bursts = 1;
    }
	*/

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
    
    buf = malloc(MAXBUFF * sizeof(int));
    
    memset(buf, 0, MAXBUFF * sizeof(int));
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
	signal(SIGCHLD, SIG_IGN);

	sigset_t alarm_sig;
	int signum;
	sigemptyset(&alarm_sig);
	sigaddset(&alarm_sig, SIGALRM);

	gettimeofday(&start, NULL);

	//printf("st %d b %d bp %d p %d\n", sending_time, bursts, burst_packets, pause);
	
	pid_t pid;

	while(sending_time--){
		pnumber = 0;
		bursts=b;
		while (bursts--) {
		    sigwait(&alarm_sig, &signum);
		    
		    pid = fork();
		    if(pid==0){
		    	//burst_packets=bp;
			    while(burst_packets--){
				    ++pnumber;
					buf[0] = pnumber;	

					Sendto(sockfd, buf, packet_size, 0, p->ai_addr, p->ai_addrlen);
			    }
			    return 0;
			}

		}
		while (pid = waitpid(-1, NULL, 0)) {
		   if (errno == ECHILD) {
		      break;
		   }
		}
	}
	
	gettimeofday(&stop, NULL);
	
	int packets_lost=0;
	Recvfrom(sockfd, &packets_lost, sizeof(int) , 0, p->ai_addr, &(p->ai_addrlen));

	//printf("Sending time %f\n", ((stop.tv_sec-start.tv_sec)*1000000 + (stop.tv_usec-start.tv_usec)) / 1000000.0);
	//printf("Number of missed alarms: %d\n", misses);
	
	printf("%f %d %d\n", (((stop.tv_sec-start.tv_sec)*1000000 + (stop.tv_usec-start.tv_usec)) / 1000000.0), misses, pps);
	printf("%d %d\n", (pps-packets_lost), packets_lost);

    freeaddrinfo(servinfo);
    close(sockfd);


	return 0;
}