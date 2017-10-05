/*
- file: utmplib.c
- author: 2015118527_Yang Chang Yeop
- datetime: 2017-09-13 21:42:03
- description: Functions to buffer reads form utmp file.
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define	UTSIZE  (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int num_recs;
static int cur_rec;
static int fd_utmp = EOF;

utmp_open(char * filename)
{
	fd_utmp = open(filename, O_RDONLY);	/* File Open */
	cur_rec = num_recs = 0;		/* Init  */
	return fd_utmp;
}
struct utmp * utmp_next()
{
	struct utmp * recp;
	/* Check File Error Condition */
	if (fd_utmp == EOF)
		return NULLUT;

	/* Check Empty File Condition */
	if (cur_rec == num_recs && utmp_reload() == 0 )
		return NULLUT;

	/* Get Address of next record */
	recp = (struct utmp *) &utmpbuf[cur_rec * UTSIZE];
	cur_rec++;
	return recp;
}
int utmp_reload()
{
	int amt_read;
	amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
	num_recs = amt_read / UTSIZE;

	/* Reset Current Point */
	cur_rec = 0;
	return num_recs;
}
utmp_close()
{
	if ( fd_utmp != EOF)
		close(fd_utmp);
};
