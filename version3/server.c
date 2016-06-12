#include "headers.h"
#include "wrappers.h"


void printStatistics(int numrcv, int packetnum, int* stat){
	int j=0, i=1;
	for(i=1 ; i < numrcv; ++i){
		if( ! ( *(stat+j) == i ) ){
			j--;
			//printf("Packet number %d missing\n", i);
		}
		++j;
	}



	printf("Packets recived: %d\nPackets lost: %d\n", numrcv, packetnum-numrcv);
}

void commandInfo(){
	fprintf(stderr,"usage: ./server [-p port]\n");
    exit(1);
}


int main(int argc, char* argv[]){
	

    int sockfd, ch;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    int enable = 1;
    char *port = PORT;

    if(argc == 0 || argc > 2){
    	commandInfo();
    }

    while((ch = getopt(argc, argv, "p:")) != -1){
			switch(ch){
					case 'p':	port = optarg;
								break;
					default:	commandInfo();
			}
	}

    socklen_t addr_len;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    Getaddrinfo(NULL, port, &hints, &servinfo);
    
	
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);
    
    while(1){

	    printf("listener: waiting to recvfrom...\n");
	    addr_len = sizeof their_addr;
		
		int numrcv=0;
		int n=0;
		int buffsize=0, packetnum=0, repeat=0;
		unsigned int totalpackets=0;
		int* buf;
		int data[4];
	    int start=1;
	    //struct timeval start_t, stop;

	    struct timeval timeout;
	    timeout.tv_sec=0;
	    timeout.tv_usec=0;

	    Setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

		Recvfrom(sockfd, data, sizeof(data) , 0, (struct sockaddr *)&their_addr, &addr_len);
		
		packetnum=ntohl(data[0]);
		buffsize=ntohl(data[1]);
		repeat=ntohl(data[2]);
		int r=repeat;

		buf = malloc(buffsize * sizeof(int));
			
		timeout.tv_sec=2;
		Setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

		Sendto(sockfd, &start, sizeof start, 0, (struct sockaddr *)&their_addr, addr_len);

		//gettimeofday(&start_t, NULL);

		while(repeat--){
			while(n != -1 && numrcv < packetnum){
			
			    n = Recvfrom(sockfd, buf, sizeof buf , 0, (struct sockaddr *)&their_addr, &addr_len);
			    //stat[numrcv]=buf[0];
			    numrcv++;

			}
			
			totalpackets += numrcv;

			//printStatistics(numrcv, packetnum, stat);
			numrcv=0;
		}
		//gettimeofday(&stop, NULL);

		unsigned int packetslost=((packetnum * r) - totalpackets);
		//long double aproxtime=(((stop.tv_sec-start_t.tv_sec)*1000000 + (stop.tv_usec-start_t.tv_usec)) / 1000000.0) / r;

		printf("%u %u\n", totalpackets, packetslost);


	}	
	
	close(sockfd);
    return 0;
}