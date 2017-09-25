/*
- file: cp1.c
- author: 2015118527_Yang Chang Yeop
- datetime: 2017-09-13 21:39:55
- description: uses read and write with tunable buffer size.
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

/* Print Error Method */
void oops(char *, char *);

int main(int ac, char * av[])
{
	int in_fd, out_fd, n_chars;
	char buf[BUFFERSIZE];

	/* Check Argument Condition */
	if (ac != 3) {
		fprintf(stderr, "usage: %s source destination\n", *av);
		exit(1);
	}

	/* Check Socket Open Condition */
	if ( (in_fd = open(av[1], O_RDONLY)) == EOF)
		oops("Cannot open ", av[1]);

	/* Check Socket Create  Condition */
	if ( (out_fd = creat(av[2], COPYMODE)) == EOF)
		oops("Cannot creat", av[2]);

	/* Read Buf Roop */
	while ( (n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
		if (write(out_fd, buf, n_chars) != n_chars)
			oops("Write error to ", av[2]);
	
	/* Check Read File Check */
	if (n_chars == EOF)
		oops("Read error from ", av[1]);

	/* Check Close File Check */
	if (close(in_fd) == EOF || close(out_fd) == EOF)
		oops("Error closing files", "");
}

void oops(char * s1, char * s2)
{
	fprintf(stderr, "Error: %s ", s1);
	perror(s2);
	exit(1);
}
