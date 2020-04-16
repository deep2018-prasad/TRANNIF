/* This file contains the basic code to move control between the server and the client programs */
/* some functions need definitions from the following header files */
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

/* two main funcions for client and the server are declared here */
extern int server_program (char *);
extern int client_program (char *);

/* function for helping the user for options and the arguments */
void print_usage (FILE * stream, int exit_code)
{
        fprintf (stream, "Usage of tranif\n");
	fprintf (stream, "Syntax.:\n"
			 "tranif -option [client/server IP ADDRESS.............]\n");
	fprintf (stream, "\nOptions.:\n");
        fprintf (stream, "\n\t\t-h     --help           for help\n"
                         "\t\t-s     --server		for server program\n"
                         "\t\t-c     --client		for client program\n");
        exit (exit_code);
}


/* the main function for controlling the flow of the program for client and the server */
int main (int argc, char * argv[])
{
	fprintf (stdout, "\n\t\t\t\t\tTRANIF\t\n");
	fprintf (stdout, "\t\tCopyright (C) 2020 version.1.0 <deeprasad1996@gmail.com>\n\n");
	int i;
	char * short_options = "hs:c:"; /* defining the short options */
	struct option long_option [] = {{"help", 0, NULL, 'h'}, {"server", 1, NULL, 's'}, {"client", 1, NULL, 'c'}}; /* defining the long options*/

	int next_option;
	i = 1;
	do 
	{
		/* using the getopt_long () function call for getting next option of the user */
		next_option = getopt_long (argc, argv, short_options, long_option, NULL);
		switch (next_option)
		{
			case 'h':

					print_usage (stdout, 0);
					break;
			case 's':
					/* transfer the flow control to the server program */
					i = server_program (argv [optind - 1]);
					if (i == 0)
					{
						printf ("Server Program has worked. Hope data has been received successfully.\n");
					}
					else if (i == -1)
					{
						printf ("Oops !! Error occured in the server program.\n");
						exit (EXIT_FAILURE);
					}
					break;
			case 'c':
					/* transfer the flow control to the client program */
					i = client_program (argv [optind - 1]);
					if (i == 0)
					{
						printf ("Client Program has worked. Hope data has been sent successfully.\n");
					}
					else if (i == -1)
					{
						printf ("Oops !! Error occured in the client program.\n");
						exit (EXIT_FAILURE);
					}
					break;
			case '?':
					print_usage (stderr, -1);
					break;
			case -1:
					break;
			default :
					abort ();
		}
	}while (next_option != -1);
	return 0;
}
