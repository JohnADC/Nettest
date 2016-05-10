#include "headers.h"
#include "utility.h"


//funkcije za pretvorbu rade s big endianom
void intToByteLong(char *bytefield, int number){
		*(bytefield+3)=(number >> 24) & 0xFF;
		*(bytefield+2)=(number >> 16) & 0xFF;
		*(bytefield+1)=(number >> 8) & 0xFF;
		*(bytefield+0)=number & 0xFF;
}

long int byteToIntLong(unsigned char *bytefield){
	return (((unsigned int)(bytefield[3])*256*256*256)+((unsigned int)(bytefield[2])*256*256)+((unsigned int)(bytefield[1])*256)+(unsigned int)(bytefield[0]));
}

void intToByteShort(char *bytefield, int number){
		*(bytefield+1)=(number >> 8) & 0xFF;
		*(bytefield+0)=number & 0xFF;
}

short int byteToIntShort(unsigned char *bytefield){	
	return (((unsigned int)(bytefield[1])*256)+(unsigned int)(bytefield[0]));
}

int daemon_init(const char *pname, int facility){
	int i;
	pid_t pid;
	
	if((pid = fork()) < 0){
		return(-1);
	}else if(pid){
		_exit(0);
	}	
	
	if(setsid()<0){
		return(-1);
	}
	
	signal(SIGHUP, SIG_IGN);
	
	if((pid = fork()) < 0){
		return(-1);
	}else if(pid){
		_exit(0);
	}
	
	//chdir("/");
	
	for(i=0; i<64; i++)
			close(i);
			
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);
	
	openlog(pname, LOG_PID, facility);
	return(0);
}

/*int parse_arguments_client(OptionsClient *arguments, int argc, char **argv){
	
	static struct option long_options[] = 
	{
		{"port", required_argument, NULL, 'p'},
		{0, 0, NULL, 0}
	};

	int option_index = 0;

	while( ( c = getopt_long (argc, argv, "p", long_options, &option_index) ) != -1 ){
		switch ( c ) {
			case 'p':
				arguments->port=optarg;
				break;
			default:
				showInfo();
		}
	}

	if (optind < argc){
      while (optind < argc)
        printf ("%s ", argv[optind++]);
    
    }

	return 0;
}


int parse_arguments_server(OptionsServer *arguments, int argc, char **argv){



	return 0;
}

void showInfo(){
	
}*/