/*
- file: my_write.c
- author: 2015118527_Yang Chang Yeop
- datetime: 2017-11-04 15:58
- description: 현재 접속 된 사용자들을 확인하여 통신하는 프로그램입니다.
*/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <utmp.h>

#define TRUE 1
#define FALSE 0

int main(int ac, char * av[])
{
	int 		fd = 0;
	int		mSwitch = FALSE;
	char 		buf[BUFSIZ];
	char		sBuf[BUFSIZ];
	struct utmp	utbuf;
	int 		utmpfd;
	
	/* Check Arguments  */
	if (ac != 2) { fprintf(stderr, "usage: my_write ttyname\n"); exit(1); }
	
	fd = open(av[1], O_WRONLY);
	if (fd == EOF) { printf("- Current User(: %s) is Not-Connected.\n", av[1]); exit(1); }

	if ( (utmpfd = open(UTMP_FILE, O_RDONLY)) == EOF) { perror(UTMP_FILE); exit(1); }
	while (read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf)) {
		sprintf(sBuf, "/dev/%s", utbuf.ut_line);
		if (utbuf.ut_type == USER_PROCESS && strcmp(sBuf, av[1]) == 0) { mSwitch = TRUE; break; }
	}

	/* Only Current Connect User */
	if (mSwitch) {
		printf("- Current User(: %s) is Connected.\n", av[1]);
		while (fgets(buf, BUFSIZ, stdin) != NULL) 
		{ if (write(fd, buf, strlen(buf)) == EOF) { break; } }
	} else { printf("- Current User(: %s) is Not-Connected.\n", av[1]);  }

	close(utmpfd);
	close(fd);

	return 0;
}
