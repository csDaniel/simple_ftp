/* ******************************************************************************
*
*	filename:					sftp_host.c
*
*	created date:				3 May 2017
*	last modification date:		3 May 2017
*
*	overview:
*	sftp_host.c is a host program. It takes a specific port number and runs on
* 	an infinite loop until a kill command is sent from the hosts keyboard or
*	a kill command is sent from a client. 
*
*	pre: command line args will take a specificed portno, ex "45123"
*
*	post: in the future I'd like for it to log ANY command sent by a user, along
*	with a timestamp.
*	However for now, it returns nothing.
*
****************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

// int gethosthame()
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <errno.h>
#include <netdb.h>
#include <dirent.h>


#define BUF	256
#define IPBUF 15

typedef struct connection {
	// ex 255.255.255.255:
	char ip[IPBUF];
	// ex 12345
	char port[5];
	// contains the current working directory for any potential calls needed
	char current_dir[256];
} Connection;


// get host-side details and set up the connection info for start_host()
// get the ip of the host, get the current full working directory
// initialize the connection struct
// finally, return a boolean 1/0 if there is an error establishing the connection
// source: http://beej.us/guide/bgnet/output/html/multipage/syscalls.html#getaddrinfo
int initialize_connection(Connection *connect, char *portno);

// main control function for sftp_host.c
// uses the connection struct to help set up a connection and leave the host.c on a wait()
// when a client connects, they will send commands to the listeners on start_host(). It will
// call _process_req to read the request and return the appropriate answer
// returns boolean 1/0 if an error occurs
int start_host(Connection *connect);

// called by start_host(). takes the request string & determines the response via a decision
// tree. 
// calls kill_connection(), get_dir(), change_dir(), get_file()
void _process_req(char *req, char *response);

// ends the connection. Stops host from listening and exits sftp_host.c
void _kill_connection();

// gets the complete working directory from the connection struct and returns it as
// char response to send back to the client
void _get_dir(char *response);

// converts pre_data to post_data
void _get_char_array(char *pre_data, char *post_data);

int main(int argc, char *argv[]) {

	int response = 0;
	Connection ftpconfig;

	response = initialize_connection(&ftpconfig, argv[1]);


	printf("ip: %s\n", ftpconfig.ip);
	printf("port: %s\n", ftpconfig.port);


	return 0;
}

int initialize_connection(Connection *connect, char *portno) {

	char hostname[256];
	hostname[255] = '\0';
	gethostname(hostname, 255);
	printf("hostname as 'hostname': %s\n", hostname);

	// since gethostbyname is depreciated, use getaddrinfo
	struct addrinfo hints, *info, *p;
	int gai_result;

	memset(&hints, 0, sizeof(hints));
	// accept IPv4 or IPv6
	hints.ai_family = AF_UNSPEC;

	// establish for now, a TCP connection
	hints.ai_socktype = SOCK_STREAM;

	hints.ai_flags = AI_CANONNAME;

	if ((gai_result = getaddrinfo(hostname, "http", &hints, &info)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gai_result));
		exit(1);
	}

	for (p = info; p != NULL; p = p->ai_next) {
		printf("hostname: %s\n", p->ai_canonname);
	}

	freeaddrinfo(info);




	return 0;

}




























