#include "headers.h"
#include "wrappers.h"


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
   	//int e_flag = 0;//, p_flag = 0 , s_flag = 0, t_flag = 0;
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
					case 'p':	//p_flag = 1;
								port = optarg;
								break;
					case 's':	//s_flag=1;
								packet_size = atoi(optarg);
								break;			
					case 't':	//t_flag = 1;
								sending_time = atoi(optarg);
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
    	error("error: Maximum packet size is 65505");
    }
    if(packet_size < 1){
    	error("error: Minimum packet size is 1");
    }
    if(sending_time > MAXTIME){
    	error("error: Maximum sending time is 10s");
    }
    if(sending_time < 1){
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
    struct timeval start, stop, start2, stop2;

    buf = malloc(packet_size * sizeof(int));
    
    memset(buf, 0, sizeof(&buf));
    memset(data, 0, sizeof(data));
    
    data[0] = htonl(pps);
    data[1] = htonl(packet_size);
    data[2] = htonl(sending_time);

    Sendto(sockfd, data, sizeof data, 0, p->ai_addr, p->ai_addrlen);

    Recvfrom(sockfd, &go, sizeof go , 0, p->ai_addr, &(p->ai_addrlen));

	while(sending_time--){
		gettimeofday(&start, NULL);
		while( pps > pnumber ){
			gettimeofday(&start2, NULL);

			++pnumber;
			buf[0] = pnumber;

		    Sendto(sockfd, buf, sizeof buf, 0, p->ai_addr, p->ai_addrlen);


			gettimeofday(&stop2, NULL);
		
			int a = pause - ((stop2.tv_sec-start2.tv_sec)*1000000 + (stop2.tv_usec-start2.tv_usec));
			if (a < 0) a = 1;


			usleep(a);
			printf("%d\n", a);
		}
		gettimeofday(&stop, NULL);

		printf("Sent %d packets in %f seconds\n",pps ,(((stop.tv_sec-start.tv_sec)*1000000 + (stop.tv_usec-start.tv_usec)) / 1000000.0));

		printf("Sent %d packets\n", pnumber);
		pnumber=0;
		usleep(5000);
	}
	
    freeaddrinfo(servinfo);
    close(sockfd);


	return 0;
}