/*
- file: play_again4.c
- author: 2015118527_양창엽
- datetime: 2017-11-01 18:35
- description: Set tty into chr-by-chr, no-echo mode, set tty into no-delay mode, read char, return result.
*/

#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#define ASK		"Do you want another transaction"
#define TRIES		3
#define SLEEPTIME	2
#define	BEEP		putchar('\a')

int get_response(char *, int);
void set_cr_noecho_mode(void);
int get_ok_char(void);
void set_nodelay_mode(void);
void tty_mode(int);
void ctrl_c_handler(int);

int main(void)
{
	int	response = 0;
	void	ctrl_c_handler(int);
	
	tty_mode(0);
	set_cr_noecho_mode();
	set_nodelay_mode();
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	response = get_response(ASK, TRIES); /* Get Some Answer */
	
	tty_mode(1);
	return response;
}

int get_response(char * question, int maxtries)
{
	int input;

	printf("%s (y/n)?", question);
	fflush(stdout); 	/* Clear Buffer */

	while(1) {
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());
		if( input == 'y') return 0;
		if( input == 'n') return 1;
		if( maxtries-- == 0) return 2; /* Time Limit */
		BEEP;
	}
}

int get_ok_char()
{
	int c;
	while( (c = getchar()) != EOF && strchr("yYnN", c) == NULL);
	return c;
}

void set_cr_noecho_mode(void)
{
	struct termios ttystate;
	
	tcgetattr(0, &ttystate);
	ttystate.c_lflag	&= ~ICANON;	/* No Buffering */
	ttystate.c_lflag	&= ~ECHO;	/* No ECHO	*/
	ttystate.c_cc[VMIN]	= 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how)
{
	static struct termios original_mode;
	static int	original_flags;
	static int	stored = 0;

	if (how == 0) { tcgetattr(0, &original_mode); original_flags = fcntl(0, F_GETFL); stored = 1;  } /* Save Current Mode  */
	else { tcsetattr(0, TCSANOW, &original_mode); fcntl(0, F_SETFL, original_flags);  } /* Resore Mode */
}

void set_nodelay_mode()
{
	int	termflags;
	termflags = fcntl(0, F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0, F_SETFL, termflags);
}

void ctrl_c_handler(int signum)
{
	tty_mode(1);
	exit(1);
}

/*
- file: play_again1.c
- author: 2015118527_양창엽
- datetime: 2017-11-01 17:49
- description: Set tty into char-by-char mode, read char, return result.
*/

#include <stdio.h>
#include <termios.h>

#define QUESTION	"Do you want another transaction"

int get_response(char *);
void set_crmode(void);
void tty_mode(int);

int main(void)
{
	int	response = 0;
	
	tty_mode(0);
	set_crmode();
	response = get_response(QUESTION); /* Get Some Answer */
	
	tty_mode(1);
	return response;
}

int get_response(char * question)
{
	int input;
	printf("%s (y/n)?", question);
	
	while(1) {
		switch(getchar()) {
			case 'y' : case 'Y' : return 0;
			case 'n' : case 'N' : return 1;
			case EOF : return 1;
			default : 
			{
				printf("\n cannot understand %c, ", input);
				printf("Please type y or no \n");
			}
		}
	}
}

void set_crmode(void)
{
	struct termios ttystate;
	
	tcgetattr(0, &ttystate);
	ttystate.c_lflag	&= ~ICANON;
	ttystate.c_cc[VMIN]	= 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how)
{
	static struct termios original_mode;
	
	if (how == 0) { tcgetattr(0, &original_mode); } /* Save Current Mode  */
	else { tcsetattr(0, TCSANOW, &original_mode);  } /* Resore Mode */
}

/*
- file: play_again2.c
- author: 2015118527_양창엽
- datetime: 2017-11-01 18:04
- description: Set tty into Char-By-Char mode and no-Echo mode read char, return result.
*/

#include <stdio.h>
#include <termios.h>

#define QUESTION	"Do you want another transaction"

int get_response(char *);
void set_cr_noecho_mode(void);
void tty_mode(int);

int main(void)
{
	int	response = 0;
	
	tty_mode(0);
	set_cr_noecho_mode();
	response = get_response(QUESTION); /* Get Some Answer */
	
	tty_mode(1);
	return response;
}

int get_response(char * question)
{
	printf("%s (y/n)?", question);
	
	while(1) {
		switch(getchar()) {
			case 'y' : case 'Y' : return 0;
			case 'n' : case 'N' : return 1;
			case EOF : return 1;
		}
	}
}

void set_cr_noecho_mode(void)
{
	struct termios ttystate;
	
	tcgetattr(0, &ttystate);
	ttystate.c_lflag	&= ~ICANON;	/* No Buffering */
	ttystate.c_lflag	&= ~ECHO;	/* No ECHO	*/
	ttystate.c_cc[VMIN]	= 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how)
{
	static struct termios original_mode;
	
	if (how == 0) { tcgetattr(0, &original_mode); } /* Save Current Mode  */
	else { tcsetattr(0, TCSANOW, &original_mode);  } /* Resore Mode */
}

/*
- file: play_again3.c
- author: 2015118527_양창엽
- datetime: 2017-11-01 18:14
- description: Set tty into chr-by-chr, no-echo mode, set tty into no-delay mode.
*/

#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>

#define ASK		"Do you want another transaction"
#define TRIES		3
#define SLEEPTIME	2
#define	BEEP		putchar('\a')

int get_response(char *, int);
void set_cr_noecho_mode(void);
int get_ok_char(void);
void set_nodelay_mode(void);
void tty_mode(int);

int main(void)
{
	int	response = 0;
	
	tty_mode(0);
	set_cr_noecho_mode();
	set_nodelay_mode();
	response = get_response(ASK, TRIES); /* Get Some Answer */
	
	tty_mode(1);
	return response;
}

int get_response(char * question, int maxtries)
{
	int input;

	printf("%s (y/n)?", question);
	fflush(stdout); 	/* Clear Buffer */

	while(1) {
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());
		if( input == 'y') return 0;
		if( input == 'n') return 1;
		if( maxtries-- == 0) return 2; /* Time Limit */
		BEEP;
	}
}

int get_ok_char()
{
	int c;
	while( (c = getchar()) != EOF && strchr("yYnN", c) == NULL);
	return c;
}

void set_cr_noecho_mode(void)
{
	struct termios ttystate;
	
	tcgetattr(0, &ttystate);
	ttystate.c_lflag	&= ~ICANON;	/* No Buffering */
	ttystate.c_lflag	&= ~ECHO;	/* No ECHO	*/
	ttystate.c_cc[VMIN]	= 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how)
{
	static struct termios original_mode;
	static int	original_flags;

	if (how == 0) { tcgetattr(0, &original_mode); original_flags = fcntl(0, F_GETFL);  } /* Save Current Mode  */
	else { tcsetattr(0, TCSANOW, &original_mode); fcntl(0, F_SETFL, original_flags);  } /* Resore Mode */
}

void set_nodelay_mode()
{
	int	termflags;
	termflags = fcntl(0, F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0, F_SETFL, termflags);
}

/*
- file: play_again4.c
- author: 2015118527_양창엽
- datetime: 2017-11-01 18:35
- description: Set tty into chr-by-chr, no-echo mode, set tty into no-delay mode, read char, return result.
*/

#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

#define ASK		"Do you want another transaction"
#define TRIES		3
#define SLEEPTIME	2
#define	BEEP		putchar('\a')

int get_response(char *, int);
void set_cr_noecho_mode(void);
int get_ok_char(void);
void set_nodelay_mode(void);
void tty_mode(int);
void ctrl_c_handler(int);

int main(void)
{
	int	response = 0;
	void	ctrl_c_handler(int);
	
	tty_mode(0);
	set_cr_noecho_mode();
	set_nodelay_mode();
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	response = get_response(ASK, TRIES); /* Get Some Answer */
	
	tty_mode(1);
	return response;
}

int get_response(char * question, int maxtries)
{
	int input;

	printf("%s (y/n)?", question);
	fflush(stdout); 	/* Clear Buffer */

	while(1) {
		sleep(SLEEPTIME);
		input = tolower(get_ok_char());
		if( input == 'y') return 0;
		if( input == 'n') return 1;
		if( maxtries-- == 0) return 2; /* Time Limit */
		BEEP;
	}
}

int get_ok_char()
{
	int c;
	while( (c = getchar()) != EOF && strchr("yYnN", c) == NULL);
	return c;
}

void set_cr_noecho_mode(void)
{
	struct termios ttystate;
	
	tcgetattr(0, &ttystate);
	ttystate.c_lflag	&= ~ICANON;	/* No Buffering */
	ttystate.c_lflag	&= ~ECHO;	/* No ECHO	*/
	ttystate.c_cc[VMIN]	= 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void tty_mode(int how)
{
	static struct termios original_mode;
	static int	original_flags;
	static int	stored = 0;

	if (how == 0) { tcgetattr(0, &original_mode); original_flags = fcntl(0, F_GETFL); stored = 1;  } /* Save Current Mode  */
	else { tcsetattr(0, TCSANOW, &original_mode); fcntl(0, F_SETFL, original_flags);  } /* Resore Mode */
}

void set_nodelay_mode()
{
	int	termflags;
	termflags = fcntl(0, F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0, F_SETFL, termflags);
}

void ctrl_c_handler(int signum)
{
	tty_mode(1);
	exit(1);
}
