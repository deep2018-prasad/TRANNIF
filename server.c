/* This file contains the code for the server of the communication of the system */
/* some header files need to included in the main program file */
/* some functions are defined in the included library*/

#include "/usr/include/stdio.h"
#include "/usr/include/stdlib.h"
#include "/usr/include/unistd.h"
#include "/usr/include/sys/socket.h"
#include "/usr/include/netinet/in.h"
#include "/usr/include/arpa/inet.h"
#include "/usr/include/sys/stat.h"
#include "/usr/include/sys/fcntl.h"

/* read_client function call for reading the data sent by the client */
void read_client (int fd, char * file)
{
      	printf ("Reading the client's file..........\n");
        int count; /* count for read system call */
        count = 1; /* initialize the count by 1 for while loop for the first time */

        char ch; /* a temporary character type buffer for storing one charater at a time */
	/* we need a temporary buffer to store the data sent by the client */
        char * buff;
	/* a counter for the every character of the buffer */
        int i;
        i = 0; 
        while (count)
        {
		/* reading the data from the client socket using read () system call */
                count = read (fd, &ch, 1);
                buff = realloc (buff, i + 1);          
		/* store every character one at a time into the temporary buffer 'buff' */
                buff [i] = ch;
                i = i + 1;
        }
	/* the last character of the temporary buffer 'buff' should be null character */
        buff [i] = '\0';
	/* done with the client socket, then close the client socket's file descriptor */
        close (fd);

	/* declare the file descriptor for local file system */
        int local_fd;
	/* open the file if exists or create a new one with the filename given by the user */
        local_fd = open (file, O_CREAT | O_WRONLY );

	/* write all the data stored in the buffer into the local file */
        write (local_fd, buff, i);
        free (buff);
       printf ("Client's Data has been successfully stored in the file %s.\n", file);
	/* if everything written in the file then close it */
        close (local_fd);
}

/* The main program for contolling every type of function calls is defined here */
/* need one command line argument in the main function for the IP. address of the device */

int server_program (char * a)
{
	printf ("This is server program.\n");
	int socket_fd; /* Declaring the socket file descriptor */ 
	char choice [3] = {'Y', 'e', 's'}; /* A contolling choice of the upcoming for loop. Initialization has been done with "Yes"*/
	int i;
	/* Calling the socket() system call for creating a socket on the local device. It will return the file descriptor of socket*/
	socket_fd = socket (PF_INET, SOCK_STREAM, 0);

	/* If any error report the error and terminate the program execution immediately */
	if (socket_fd == -1)
	{
		perror ("socket");
		return -1;
	}
	else
	{
		printf ("Socket for the server has been created.\n");
	}
	/* Get the filename from the user after initializing  the filename buffer */
	char filename [20];
	fprintf (stdout, "Enter the filename to store the data.:\t");
	scanf ("%s", filename);

	/* Define the structure of the address of the socket */
	struct sockaddr_in address;
	/* The domain or the family of the socket which have to be used is internet domain socket so AF_INET*/
	address.sin_family = AF_INET;
	/* The port 9784 has been used for the communication */
	address.sin_port = htons (9784);
	/* The IP address will be converted in the unique unsined long int integer by inet_addr () function call */
	address.sin_addr.s_addr = inet_addr (a);

	printf ("The address structure for the server socket has been created.\n");
	/* 'i' for detecting the error for bind () system call */
	/* We need to name the socket by binding the address with the local socket */
	i = bind (socket_fd, (struct sockaddr *)&address, sizeof (address));
	/* if any error, report it and terminate the program execution immediately */
	if (i < 0)
	{
		perror ("bind");
		return -1;
	}
	else
	{
		printf("Naming has been done.\n");
	}
	/* define a queue of clients using the listen() function call */
	listen (socket_fd, 5);
	printf ("Server is waiting................\n");
	/* while loop for the accepting the connection request of the clients repeatitatively */
	while (1)//(choice [0] == 'Y' || choice [0] == 'y') && (choice [1] == 'E' || choice [1] == 'e') && (choice [2] == 'S' || choice [2] == 's'))
	{
		
		int client_socket_fd; /* declation of a file descriptor for the client socket through a port */
		struct sockaddr_in client_name; /* The client socket address structure need only to be declared */
		/* length of the socket address structure */
		socklen_t client_len;
		/* the filename need to be obtained from the user side */

		/* Need to accept the connection request by the server */
		/* connect () function call will be used which will return the client socket file descriptor */
		client_socket_fd = accept (socket_fd, (struct sockaddr *) &client_name, &client_len);
		/* If any error during accepting the request of client report it and terminate the execution of the program */
		if (client_socket_fd == -1)
		{
			perror ("accept");
			return -1;
		}
		else
		{
			printf ("Client connection request has been accepted.\n");
		}
		/* call the function read client for reading the file data sent by the client */
		read_client (client_socket_fd, filename);
		break;
	}
	/* If the communication is done then close the socket */
	close (socket_fd);
	printf("Server socket has been closed.\n");
	return 0;
}
