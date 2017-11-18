/*
- file: bounce1d.c
- author: 2015118527_양창엽
- datetime: 2017-11-18 17:26
- description: Purpose animation with user controlled apped and direction.
*/

#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <termios.h>

#define MESSAGE "hello"
#define BLANK	"	"

int row = 0, col = 0, dir = 0;

int set_ticker(int);

int main(void)
{
	int delay;
	int ndelay;
	int c;
	void move_msg(int);

	initscr();
	crmode();
	noecho();
	clear();

	row = 10;
	col = 0;
	dir = 1;
	delay = 200;

	move(row, col);
	addstr(MESSAGE);
	signal(SIGALRM, move_msg);
	set_ticker(delay);

	while(1)
	{
		ndelay = 0;
		c = getchar();
		if ( c == 'Q' ) break;
		if ( c == ' ' ) dir = -dir;
		if ( c == 'f' && delay > 2) ndelay  = delay / 2;
		if ( c == 's' ) ndelay = delay * 2;
		if (ndelay > 0) set_ticker(delay = ndelay);
	}

	endwin();
	return 0;
}

void move_msg(int signum)
{
	signal(SIGALRM, move_msg);
	move(row, col);
	addstr(BLANK);
	col += dir;
	move(row, col);
	addstr(MESSAGE);
	refresh();

	if (dir == EOF && col <= 0) { dir = 1; }
	else if (dir == 1  && col + strlen(MESSAGE) >= COLS) { dir = -1;  }
}


/*
- file: bounce2d.c
- author: 2015118527_양창엽
- datetime: 2017-11-18 19:14
- description: Block on read, but timer tick sends SIGALRM caught by ball_move.
*/

#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <string.h>
#include <termios.h>

#include "bounce.h"

int set_ticker(int);
int bounce_or_lose(struct ppball*);

struct ppball the_ball;

void set_up();
void wrap_up();

int main(void)
{
	int	c;

	/* Init Timer and SIGNAL Setting Method  */
	set_up();

	while( (c = getchar() != 'Q')) { 
		if (c == 'f') the_ball.x_ttm--;	/* Fast x Component  */
		else if (c == 's') the_ball.x_ttm++; /* Slow Down x Component */
		else if (c == 'F') the_ball.y_ttm--; /* Fast y Component  */
		else if (c == 'S') the_ball.y_ttm++; /* Slow Down y Component  */
	}
	
	wrap_up();

	return 0;
}

void set_up() 
{
	void ball_move(int);

	the_ball.y_pos = Y_INIT;
	the_ball.x_pos = X_INIT;
	the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
	the_ball.x_ttg = the_ball.x_ttm = X_TTM;
	the_ball.y_dir = 1;
	the_ball.x_dir = 1;
	the_ball.symbol = DFL_SYMBOL;
	initscr();
	noecho();
	crmode();

	signal(SIGINT, SIG_IGN);
	mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
	refresh();

	signal(SIGALRM, ball_move);
	set_ticker(1000/TICKS_PER_SEC);
}

void wrap_up()
{
	set_ticker(0);
	endwin();
}

void ball_move(int signum)
{
	int y_cur, x_cur, moved;

	signal(SIGALRM, SIG_IGN);
	y_cur = the_ball.y_pos;
	x_cur = the_ball.x_pos;
	moved = 0;

	/* y_ttm  */
	if (the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1) {
		the_ball.y_pos += the_ball.y_dir; // move
		the_ball.y_ttg = the_ball.y_ttm; // reset
		moved = 1;
	}
	/* x_ttm  */
	if (the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1) {
		the_ball.x_pos += the_ball.x_dir; // move
		the_ball.x_ttg = the_ball.x_ttm; // reset
		moved = 1;
	}

	if(moved) {
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
		bounce_or_lose(&the_ball);
		move(LINES-1, COLS-1);
		refresh();
	}

	signal(SIGALRM, ball_move);
}

int bounce_or_lose(struct ppball *bp)
{
	int	return_val = 0;
	
	if (bp->y_pos == TOP_ROW) { bp->y_dir = 1; return_val = 1; }
	else if (bp->y_pos == BOT_ROW) { bp->y_dir = -1; return_val = 1; }
	else if (bp->x_pos == LEFT_EDGE) { bp->x_dir = 1; return_val = 1; }
	else if (bp->x_pos == RIGHT_EDGE) { bp->x_dir = -1; return_val = 1; }

	return return_val;
}


/*
- file: bounce_async.c
- author: 2015118527_양창엽
- datetime: 2017-11-18 20:12
- description: Purpose animation with user control, using O_ASYNC on fd.
*/

#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#define MESSAGE "hello"
#define BLANK 	"	"

int row = 10, col = 0, dir = 1, delay = 200, done = 0;

int set_ticker(int);

int main(void)
{
	void 	on_alarm(int);
	void	on_input(int);
	void	enable_kbd_signals();

	initscr();
	crmode();
	noecho();
	clear();

	signal(SIGIO, on_input);
	enable_kbd_signals();
	signal(SIGALRM, on_alarm);
	set_ticker(delay);
	
	move(row, col);
	addstr(MESSAGE);
	
	while( !done ) { pause(); }

	endwin();

	return 0;
}

void on_input(int signum)
{
	int c = getch();
	if (c == 'Q' || c == EOF) done = 1;
	else if (c == ' ') dir = -dir;
}

void on_alarm(int signum)
{
	signal(SIGALRM, on_alarm);
	mvaddstr(row, col, BLANK);
	col += dir;
	mvaddstr(row, col, MESSAGE);
	refresh();

	if (dir == EOF && col <= 0) dir = 1;
	else if (dir == 1 && col + strlen(MESSAGE) >= COLS) dir = EOF;
}

void enable_kbd_signals()
{
	int fd_flags;
	
	fcntl(0, F_SETOWN, getpid());
	fd_flags = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, (fd_flags|O_ASYNC));
}


/*
- file: bounce_aio.c
- author: 2015118527_양창엽
- datetime: 2017-11-18 20:58
- description: Purpose animation with user contorl, using aio_read(0) etc.
*/

#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>
#include <aio.h>
#include <string.h>
#include <termios.h>

#define MESSAGE "hello"
#define BLANK	"	"

int row = 10, col = 0, dir = 1, delay = 200, done = 0;

struct aiocb kbcbuf;

int set_ticker(int);

int main(void)
{
	void on_alarm(int);
	void on_input(int);
	void setup_aio_buffer();
	
	initscr();
	crmode();
	noecho();
	clear();
	
	signal(SIGIO, on_input);
	setup_aio_buffer();
	aio_read(&kbcbuf);

	signal(SIGALRM, on_alarm);
	set_ticker(delay);
	
	mvaddstr(row, col, MESSAGE);
	
	while (!done) { pause(); }

	endwin();
}

void on_input(int snum)
{
	int c;
	char * cp = (char*) kbcbuf.aio_buf;

	if (aio_error(&kbcbuf) != 0)
		perror("reading failed");
	else
		if (aio_return(&kbcbuf) == 1) {
			c = *cp;
			if (c == 'Q' || c == EOF) { done = 1; }
			else if (c == ' ') { dir = -dir;  }
		}

	aio_read(&kbcbuf);
}

void on_alarm(int snum)
{
	signal(SIGALRM, on_alarm);	
	mvaddstr(row, col, BLANK);
	col += dir;
	mvaddstr(row, col, MESSAGE);
	refresh();

	if (dir == EOF && col <= 0) dir = 1;
	else if (dir == 1 && col + strlen(MESSAGE) >= COLS) dir = -1;
}

void setup_aio_buffer()
{
	static char input[1];

	kbcbuf.aio_fildes = 0;
	kbcbuf.aio_buf = input;
	kbcbuf.aio_nbytes = 1; // number to read
	kbcbuf.aio_offset = 0; // offset in file

	kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	kbcbuf.aio_sigevent.sigev_signo = SIGIO;
}


#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>

int set_ticker(int n_msecs)
{
	struct itimerval new_timeset;
	long	n_sec, n_usecs;
	
	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;
	
	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;
	
	/* Return -1 Error, 0 Success  */
	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}


#define BLANK	' '
#define DFL_SYMBOL	'o'
#define TOP_ROW	5
#define BOT_ROW 20
#define LEFT_EDGE	10
#define RIGHT_EDGE	70

#define X_INIT	10
#define Y_INIT	10
#define TICKS_PER_SEC	50

#define X_TTM	5
#define Y_TTM	8

struct ppball {
	int y_pos, x_pos, y_ttm, x_ttm, y_ttg, x_ttg, y_dir, x_dir;
	char symbol;
};
