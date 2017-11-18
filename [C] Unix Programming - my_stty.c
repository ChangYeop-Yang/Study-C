/*
- file: my_stty.c
- author: 2015118527_Yang Chang Yeop
- datetime: 2017-11-04 19:50)
- description: 다중 인자 및 단일 인자를 지원하는 Stty 프로그램.
*/

#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>

#define ON "ON"
#define OFF "OFF"
#define TRUE 1
#define FALSE -1

struct flaginfo { int fl_value; char * fl_name; char * fl_char; int fl_modify; };

struct flaginfo input_flags[] = {
	IGNBRK, "Ignore break condition", "IGNBRK", FALSE,
	BRKINT, "Signal interrupt on break", "BRKINT", FALSE,
	IGNPAR, "Ignore chars with parity errors", "IGNPAR", FALSE,
	PARMRK, "Mark parity errors", "PARMRK", FALSE,
	INPCK, 	"Enable input parity check", "INPCK", FALSE,
	ISTRIP, "Strip character", "ISTRIP", FALSE,
	INLCR,	"Map NL to CR on input", "INLCR", FALSE,
	IGNCR,	"Ignore CR", "IGNCR", FALSE,
	ICRNL,	"Map CR to NL on input", "ICRNL", FALSE,
	IXON, 	"Enable start/stop output control", "IXON", FALSE,
	IXOFF,	"Enable start/stop input control", "IXOFF", FALSE,
	0,	NULL
};

struct flaginfo local_flags[] = {
	ISIG,	"Enable signals", "ISIG", FALSE,
	ICANON,	"Canonical input (erase and kill)", "ICANON", FALSE,
	ECHO,	"Enable echo", "ECHO", FALSE,
	ECHOE,	"Echo ERASE as BS-SPACE-BS", "ECHOE", FALSE,
	ECHOK,	"Echo KILL by starting new line", "ECHOK", FALSE,
	0,	NULL
};

struct cmd {
	char 	asName[BUFSIZ];
	char	asType[BUFSIZ];
};

/* Declear Function  */
void showbaud(int);
void show_some_flags(struct termios *);
void show_flagset(int, struct flaginfo[]);
void set_flag(int, int, char *, struct termios *);
int get_Location(int, struct flaginfo[], char *);

int main(int ac, char * av[])
{
	int mCount = 0;
	int mSwitch = FALSE;
	struct cmd mCMD;
	struct termios ttyinfo;
	
	/* Get Arrtibutes  */
	if (tcgetattr(0, &ttyinfo) == EOF) { perror("Cannot get params about stdin"); exit(1); }

	/* Get Command */
	for (mCount = 1; mCount < ac; mCount++)
	{
		/* Save Mode Name Value  */
		if (mCount % 2 != 0) { strcpy(mCMD.asName, av[mCount]); continue; }

		/* Save Mode Type Value */
		strcpy(mCMD.asType, av[mCount]);

		/* Input Flags Mode */
		mSwitch = get_Location(sizeof(input_flags)/sizeof(input_flags[0]), input_flags, mCMD.asName);
		if ( mSwitch != FALSE) {
			set_flag(FALSE, input_flags[mSwitch].fl_value, mCMD.asType, &ttyinfo);
			input_flags[mSwitch].fl_modify = TRUE;
			goto save;
		}

		/* Local Flags Mode */
                mSwitch = get_Location(sizeof(local_flags)/sizeof(local_flags[0]), local_flags, mCMD.asName);
                if ( mSwitch != FALSE) {
                      set_flag(TRUE, local_flags[mSwitch].fl_value, mCMD.asType, &ttyinfo);
			local_flags[mSwitch].fl_modify = TRUE;
                      goto save;
                }
		
		save : { if (tcsetattr(0, TCSANOW, &ttyinfo) == EOF) { printf("Error Setting Bit.\n"); } }
	}

	showbaud(cfgetospeed(&ttyinfo));

	show_some_flags(&ttyinfo);

	return 0;
}

int get_Location(int mLength, struct flaginfo mInfo[], char * target)
{
	int mCount = 0;
	for (mCount = 0; mCount < mLength-1; mCount++) { 
		if (strcmp(mInfo[mCount].fl_char, target) == 0) { return mCount; }
	}

	return FALSE;
}

void set_flag(int mode, int value, char * type, struct termios * tty)
{
	if (mode == TRUE) 
	{
		if (strcmp(ON, type) == 0) { tty->c_lflag |= value; }
		else { tty->c_lflag &= ~value;  }
	}
	else
	{
		if(strcmp(OFF, type) == 0) { tty->c_iflag &= ~value; }
		else { tty->c_iflag |= value;  }
	}
}

void showbaud(int thespeed)
{
	printf("\nThe baud rate is");
	switch(thespeed) {
		case B300: { printf("300\n"); break; }
		case B600: { printf("600\n"); break; }
		case B1200: { printf("1200\n"); break; }
		case B1800: { printf("1800\n"); break; }
		case B2400: { printf("2400\n"); break; }
		case B4800: { printf("4800\n"); break; }
		case B9600: { printf("9600\n"); break; }
		default : { printf("Fase\n"); break; }
	}
}

void show_some_flags(struct termios * ttyp)
{
	printf("\n\x1b[31m:::: Input Flage Mode  ::::\x1b[0m\n");
	show_flagset(ttyp->c_iflag, input_flags);
	printf("\n\x1b[32m:::: local Flags Mode  ::::\x1b[0m\n");
	show_flagset(ttyp->c_lflag, local_flags);
}

void show_flagset(int thevalue, struct flaginfo thebitnames[])
{
	int 	i;
	for (i = 0; thebitnames[i].fl_value; i++) {

		if (thebitnames[i].fl_modify == TRUE) 
		{ printf("%d.	\x1b[35m*%s: %s is \x1b[0m", i+1, thebitnames[i].fl_char, thebitnames[i].fl_name); }
		else { printf("%d.	%s: %s is ", i+1, thebitnames[i].fl_char, thebitnames[i].fl_name); }

		if (thevalue & thebitnames[i].fl_value) { printf("\x1b[33mON\x1b[0m\n"); } else { printf("\x1b[34mOFF\x1b[0m\n"); }
	}
}
