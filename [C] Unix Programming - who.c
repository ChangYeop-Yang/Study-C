/*
- file: who3.c
- author: 2015118527_Yang Chang Yeop
- datetime: 2017-09-13 21:23:39
- description: who with buffered reads.
*/

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SHOWHOST
void showtime(long);
void show_info(struct utmp*);

int main(void)
{
	struct utmp * utbufp, *utmp_next();

	if ( utmp_open( UTMP_FILE ) == EOF) {
		perror (UTMP_FILE);
		exit(1);
	}

	/* Check pt_temp nil Condition  */
	while ( (utbufp = utmp_next() ) != ((struct utmp *) NULL) )
		show_info(utbufp);

	utmp_close();

	return 0;
}

void show_info(struct utmp * utbufp)
{
	if (utbufp->ut_type != USER_PROCESS)
		return;

	printf("%-8.8s", utbufp->ut_name);
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);
	printf(" ");
	printf("%10ld", utbufp->ut_time);
	printf(" ");
	showtime(utbufp->ut_time);
#ifdef	SHOWHOST
	if( utbufp->ut_host[0] != '\0')
		printf("(%s)", utbufp->ut_host);
#endif
	printf("\n");
}
void showtime(long timeval)
{
	char * cp;
	cp = ctime(&timeval);
	
	printf("%12.12s", cp+4);
}
