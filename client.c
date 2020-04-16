/* This file contains the code of the communication system */
/* some function calls and system calls of the code are defined in the fillowing included header files */

#include "/usr/include/stdio.h"
#include "/usr/include/string.h"
#include "/usr/include/stdlib.h"
#include "/usr/include/unistd.h"
#include "/usr/include/string.h"
#include "/usr/include/fcntl.h"
#include "/usr/include/sys/socket.h"
#include "/usr/include/netinet/in.h"
#include "/usr/include/arpa/inet.h"

/* write_file for the writting the data on the socket */
/* Now that data will be sent to server */
void write_file (int socket_fd, char * message)
{
	printf ("Writting data to the socket.......\n");
	size_t len;
	len = strlen (message) + 1;
	write (socket_fd, message, len);
}

/* a main function call for managing every other function call */
/* It will need one command line argument for the IP address  of the server */

int client_program (char * a)
{
	printf ("This is client program.\n");
	int socket_fd; /* the socket file descriptor for the client system */
	char filename[20]; /* filename given  by the user */
	
	int i;
	/* Get the filename to be sent from the user */
	printf ("Enter the filename to send.:\t");
	scanf ("%[^\n]", filename);
	
	int local_fd;
	char * buff;
	buff = NULL;
	local_fd = open (filename, O_RDONLY);
	int count;
	count = 1;
	char ch;
	i = 0;
	while (count)
	{
		count = read (local_fd, &ch, 1);
		buff = realloc (buff, i + 1);
		buff [i] = ch;
		i = i + 1;
	}
	buff [i] = '\0';
	/* Creating a socket on the client system */
	
	socket_fd = socket (PF_INET, SOCK_STREAM, 0);
	/* if any error while creating the socket report it and terminate the execution of the program immediately */
	if (socket_fd == -1)
	{
		perror ("socket");
		return -1;
	}
	else
	{
		printf ("A socket for the client has been created.\n");
	}
	/* Defining a socket address structure */
	struct sockaddr_in address;

	/* Domain should be INTERNET NETWORKING (AF_INET) */
	address.sin_family = AF_INET;
	/* Address of the server will be assigned to the address structure */
	address.sin_addr.s_addr = inet_addr (a);
	/* The port 9784 has been used for the communication */
	address.sin_port = htons (9784);

	printf ("Address structure for the client socket has been created.\n");
	i = 0;
	/* requesting for the connection by the client to the server */
	i = connect (socket_fd, (struct sockaddr *)&address, sizeof (address));
	/* if any error in the connection request then report it and terminate the execution of the program immediately */
	if (i == -1)
	{
		perror ("connect");
		return -1;
	}
	else
	{
		printf ("Connection has been established successfully.\n");
	}
	/* call the write_file function call to send the data to the server */
	write_file (socket_fd, buff);
	free (buff);
	printf ("File has been sent.\n");
	/* done sending, now close the socket */

	close (socket_fd);
	printf ("Socket of the client has been closed.\n");
	return 0;
}
