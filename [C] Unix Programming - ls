/*
- file: ls1.c
- author: 2015118527_æÁ√¢ø±
- datetime: 2017-09-21 21:52
- description: Directory and Entry Basic ls.
*/

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char []);

void main(int ac, char * av[])
{
	if (ac == 1)
		do_ls(".");
	else
		while (--ac) {
			printf("%s:\n", *++av);
			do_ls(*av);
		}
}

void do_ls(char dirname[])
{
	DIR		* dir_ptr;	/* Directory */
	struct dirent	* direntp;	/* Entry */
	
	if ( (dir_ptr = opendir(dirname)) == NULL )
		fprintf(stderr, "ls1: cannot open %s\n", dirname);
	else
	{
		while ( (direntp = readdir(dir_ptr)) != NULL )
			printf("%s\n", direntp->d_name);
		closedir(dir_ptr);
	}
}


/*
- file: ls2.c
- author: 2015118527_æÁ√¢ø±
- datetime: 2017-09-21 21:53)
- description: πŸ¿Ã∆Æ ¥‹¿ß¿« µ∑∫≈‰∏Æ π◊ ∞¢ ∆ƒ¿œ¿« øÎ∑Æ √ﬂ∞° µ» ls
*/
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void do_ls(char dirname []);
void dostat(char * filename);
void show_file_info(char * filenam, struct stat * info_p);
void mode_to_letters(int mode, char str[]);
char * uid_to_name(uid_t uid);
char * gid_to_name(gid_t gid);

void main(int ac, char * av[])
{
	if (ac == 1) { do_ls("."); }
	else {
		while (--ac) {
			printf("%s:\n", *++av);
			do_ls(*av);
		}
	}
}

void do_ls(char dirname[])
{
	DIR		* dir_ptr;	/* Directory */
	struct dirent	* direntp;	/* Entry */
	
	if ( (dir_ptr = opendir(dirname)) == NULL )
	{ fprintf(stderr, "ls1: cannot open %s\n", dirname); }
	else
	{
		while ( (direntp = readdir(dir_ptr)) != NULL )
		{ dostat(direntp->d_name); }
		closedir(dir_ptr);
	}
}

void dostat(char * filename)
{
	struct stat info;
	
	if (stat(filename, &info) == -1) { perror(filename); }
	else { show_file_info(filename, &info); }
}

void show_file_info(char * filename, struct stat * info_p)
{
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];

	mode_to_letters(info_p->st_mode, modestr);
	
	printf("%s", modestr);
	printf("%4d ", (int) info_p->st_nlink);
	printf("%-8s ", uid_to_name(info_p->st_uid));
	printf("%-8s ", gid_to_name(info_p->st_gid));
	printf("%8ld ", (long)info_p->st_size);
	printf("%.12s ", 4+ctime(&info_p->st_mtime));
	printf("%s\n", filename);
}

void mode_to_letters(int mode, char str[])
{
	strcpy(str, "----------");

	if (S_ISDIR(mode)) { str[0] = 'd'; }
	if (S_ISCHR(mode)) { str[0] = 'c'; }
	if (S_ISBLK(mode)) { str[0] = 'b'; }

	if (mode & S_IRUSR) { str[1] = 'r'; }
	if (mode & S_IWUSR) { str[2] = 'w'; }
	if (mode & S_IXUSR) { str[3] = 'x'; }

	if (mode & S_IRGRP) { str[4] = 'r'; }
	if (mode & S_IWUSR) { str[5] = 'w'; }
	if (mode & S_IXGRP) { str[6] = 'x'; }

	if (mode & S_IROTH) { str[7] = 'r'; }
	if (mode & S_IWOTH) { str[8] = 'w'; }
	if (mode & S_IXOTH) { str[9] = 'x'; }
}

char * uid_to_name(uid_t uid)
{
	struct passwd * getpwuid(), *pw_ptr;
	static char numstr[10];

	if ( ( pw_ptr = getpwuid(uid) ) == NULL ) {
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else { return pw_ptr->pw_name; }
}

char * gid_to_name(gid_t gid)
{
	struct group * getgrgid(), *grp_ptr;
	static char numstr[10];

	if ( (grp_ptr = getgrgid(gid) ) == NULL ) {
		sprintf(numstr,"%d", gid);
		return numstr;
	} 
	else { return grp_ptr->gr_name; }
}



/*
- file: ls3.c
- author: 2015118527_æÁ√¢ø±
- datetime: 2017-09-21 21:55
- description: Bloack ¥‹¿ß¿« ¿¸√º ∆ƒ¿œ ≈©±‚∏¶ ∆˜«‘«— ls
*/
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void do_ls(char dirname []);
void dostat(char * filename);
void show_file_info(char * filenam, struct stat * info_p);
void mode_to_letters(int mode, char str[]);
char * uid_to_name(uid_t uid);
char * gid_to_name(gid_t gid);

void main(int ac, char * av[])
{
	if (ac == 1) { do_ls("."); }
	else {
		while (--ac) {
			printf("%s:\n", *++av);
			do_ls(*av);
		}
	}
}

void do_ls(char dirname[])
{
	DIR		* dir_ptr;	/* Directory */
	struct dirent	* direntp;	/* Entry */

	struct stat	info;
	int		mEnd = 0;
	int		mStart = 0;
	int		mSumBlock = 0;
	char 		asFileName[100][255]; // File Name Array
	
	if ( (dir_ptr = opendir(dirname)) == NULL )
	{ fprintf(stderr, "ls1: cannot open %s\n", dirname); }
	else
	{
		for (mEnd = 0; (direntp = readdir(dir_ptr)) != NULL;)
		{
			if (direntp->d_name[0] == 46) { continue; } // Directory Pass 

			strcpy(asFileName[mEnd++], direntp->d_name);
			stat(direntp->d_name, &info);

			/* Sum Each File Block Size  */
			mSumBlock += (info.st_blocks / 2);
		}
	
		closedir(dir_ptr);

		/* Print Total Block Size */
		printf("total %d\n", mSumBlock);

		for (mStart = 0; mStart < mEnd; mStart++)
		{ dostat(asFileName[mStart]); }
	}
}

void dostat(char * filename)
{
	struct stat info;
	int mError = stat(filename, &info);

	if (mError == EOF) { perror(filename); return; }

	show_file_info(filename, &info);
}

void show_file_info(char * filename, struct stat * info_p)
{
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];
	
	mode_to_letters(info_p->st_mode, modestr);
	
	printf("%s", modestr);
	printf("%4d ", (int) info_p->st_nlink);
	printf("%-8s ", uid_to_name(info_p->st_uid));
	printf("%-8s ", gid_to_name(info_p->st_gid));
	printf("%8ld ", (long)info_p->st_size);
	printf("%.12s ", 4+ctime(&info_p->st_mtime));
	printf("%s\n", filename);
}

void mode_to_letters(int mode, char str[])
{
	strcpy(str, "----------");

	if (S_ISDIR(mode)) { str[0] = 'd'; }
	if (S_ISCHR(mode)) { str[0] = 'c'; }
	if (S_ISBLK(mode)) { str[0] = 'b'; }

	if (mode & S_IRUSR) { str[1] = 'r'; }
	if (mode & S_IWUSR) { str[2] = 'w'; }
	if (mode & S_IXUSR) { str[3] = 'x'; }

	if (mode & S_IRGRP) { str[4] = 'r'; }
	if (mode & S_IWUSR) { str[5] = 'w'; }
	if (mode & S_IXGRP) { str[6] = 'x'; }

	if (mode & S_IROTH) { str[7] = 'r'; }
	if (mode & S_IWOTH) { str[8] = 'w'; }
	if (mode & S_IXOTH) { str[9] = 'x'; }
}

char * uid_to_name(uid_t uid)
{
	struct passwd * getpwuid(), *pw_ptr;
	static char numstr[10];

	if ( ( pw_ptr = getpwuid(uid) ) == NULL ) {
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else { return pw_ptr->pw_name; }
}

char * gid_to_name(gid_t gid)
{
	struct group * getgrgid(), *grp_ptr;
	static char numstr[10];

	if ( (grp_ptr = getgrgid(gid) ) == NULL ) {
		sprintf(numstr,"%d", gid);
		return numstr;
	} 
	else { return grp_ptr->gr_name; }
}



/*
- file: ls4.c
- author: 2015118527_æÁ√¢ø±
- datetime: 2017-09-21 21:59
- description: ƒ¸¡§∑ƒ¿ª ¿ÃøÎ«œø© ∆ƒ¿œ¿« ø¿∏ß¬˜º¯¿∏∑Œ ¡§∑ƒ«œø© √‚∑¬«œ¥¬ ls
*/
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void do_ls(char dirname []);
void dostat(char * filename);
void show_file_info(char * filenam, struct stat * info_p);
void mode_to_letters(int mode, char str[]);
char * uid_to_name(uid_t uid);
char * gid_to_name(gid_t gid);
// Qsort Comp Function
int cmp(const void * a, const void * b);

void main(int ac, char * av[])
{
	if (ac == 1) { do_ls("."); }
	else {
		while (--ac) {
			printf("%s:\n", *++av);
			do_ls(*av);
		}
	}
}

void do_ls(char dirname[])
{
	DIR		* dir_ptr;	/* Directory */
	struct dirent	* direntp;	/* Entry */

	struct stat	info;
	int		mEnd = 0;
	int		mStart = 0;
	int		mSumBlock = 0;
	char		asFileName[255][255];
	
	if ( (dir_ptr = opendir(dirname)) == NULL )
	{ fprintf(stderr, "ls1: cannot open %s\n", dirname); }
	else
	{
		for (mEnd = 0; (direntp = readdir(dir_ptr)) != NULL;)
		{
			if (direntp->d_name[0] == 46) { continue; } // Directory Pass

			strcpy(asFileName[mEnd++], direntp->d_name);
			stat(direntp->d_name, &info);

			/* Sum Each File Block Size  */
			mSumBlock += (info.st_blocks / 2);
		}
	
		closedir(dir_ptr);

		qsort(asFileName, mEnd, sizeof(asFileName[0]), cmp); // Qsort

		/* Print Total Block Size */
		printf("total %d\n", mSumBlock);

		for (mStart = 0; mStart < mEnd; mStart++){
			dostat(asFileName[mStart]);
		}
	}
}

void dostat(char * filename)
{
	struct stat info;
	int mError = stat(filename, &info);

	if (mError == EOF) { perror(filename); return; }

	show_file_info(filename, &info);
}

void show_file_info(char * filename, struct stat * info_p)
{
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];
	
	mode_to_letters(info_p->st_mode, modestr);
	
	printf("%s", modestr);
	printf("%4d ", (int) info_p->st_nlink);
	printf("%-8s ", uid_to_name(info_p->st_uid));
	printf("%-8s ", gid_to_name(info_p->st_gid));
	printf("%8ld ", (long)info_p->st_size);
	printf("%.12s ", 4+ctime(&info_p->st_mtime));
	printf("%s\n", filename);
}

void mode_to_letters(int mode, char str[])
{
	strcpy(str, "----------");

	if (S_ISDIR(mode)) { str[0] = 'd'; }
	if (S_ISCHR(mode)) { str[0] = 'c'; }
	if (S_ISBLK(mode)) { str[0] = 'b'; }

	if (mode & S_IRUSR) { str[1] = 'r'; }
	if (mode & S_IWUSR) { str[2] = 'w'; }
	if (mode & S_IXUSR) { str[3] = 'x'; }

	if (mode & S_IRGRP) { str[4] = 'r'; }
	if (mode & S_IWUSR) { str[5] = 'w'; }
	if (mode & S_IXGRP) { str[6] = 'x'; }

	if (mode & S_IROTH) { str[7] = 'r'; }
	if (mode & S_IWOTH) { str[8] = 'w'; }
	if (mode & S_IXOTH) { str[9] = 'x'; }
}

char * uid_to_name(uid_t uid)
{
	struct passwd * getpwuid(), *pw_ptr;
	static char numstr[10];

	if ( ( pw_ptr = getpwuid(uid) ) == NULL ) {
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else { return pw_ptr->pw_name; }
}

char * gid_to_name(gid_t gid)
{
	struct group * getgrgid(), *grp_ptr;
	static char numstr[10];

	if ( (grp_ptr = getgrgid(gid) ) == NULL ) {
		sprintf(numstr,"%d", gid);
		return numstr;
	} 
	else { return grp_ptr->gr_name; }
}

int cmp(const void * a, const void * b)
{
	return strcmp((char*)a, (char*)b);
}



/*
- file: ls5.c
- author: 2015118527_æÁ√¢ø±
- datetime: 2017-09-21 23:58)
- description: ±‚¡∏ ls¥¬ ¿Œ¿⁄∏¶ πﬁæ∆ ±∏«ˆ«œ¡ˆ ∏¯«œ¥¬ ∫Œ∫–¿ª «ÿ∞·«— ls
*/
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void do_ls(char dirname []);
void dostat(char * filename);
void show_file_info(char * filenam, struct stat * info_p);
void mode_to_letters(int mode, char str[]);
char * uid_to_name(uid_t uid);
char * gid_to_name(gid_t gid);
int cmp(const void * a, const void * b);

void main(int ac, char * av[])
{
	if (ac == 1) { do_ls("."); }
	else { do_ls(av[1]); }
}

void do_ls(char dirname[])
{
	DIR		* dir_ptr;	/* Directory */
	struct dirent	* direntp;	/* Entry */

	struct stat	info;
	int		mEnd = 0;
	int		mStart = 0;
	int		mSumBlock = 0;
	char		asFileName[255][255];
	
	if ( (dir_ptr = opendir(dirname)) == NULL )
	{ fprintf(stderr, "ls5: cannot open %s\n", dirname); }
	else
	{
		for (mEnd = 0; (direntp = readdir(dir_ptr)) != NULL;)
		{
			if (direntp->d_name[0] == 46) { continue; }

			/* Cat Dir and FileName */
			sprintf(asFileName[mEnd++], "%s/%s", dirname, direntp->d_name);
			
			//strcat(asFileName[mEnd], dirname);
			//strcat(asFileName[mEnd], "/");
			//strcat(asFileName[mEnd++], direntp->d_name);
			stat(asFileName[mEnd-1], &info);

			/* Sum Each File Block Size  */
			
			mSumBlock += (info.st_blocks / 2);
		}
	
		closedir(dir_ptr);

		qsort(asFileName, mEnd, sizeof(asFileName[0]), cmp);

		/* Print Total Block Size */
		printf("total %d\n", mSumBlock);

		for (mStart = 0; mStart < mEnd; mStart++){
			dostat(asFileName[mStart]);
		}
	}
}

void dostat(char * filename)
{
	struct stat info;
	int mError = stat(filename, &info);

	if (mError == EOF) { perror(filename);}

	show_file_info(filename, &info);
}

void show_file_info(char * filename, struct stat * info_p)
{
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];
	
	mode_to_letters(info_p->st_mode, modestr);
	
	printf("%s", modestr);
	printf("%4d ", (int) info_p->st_nlink);
	printf("%-8s ", uid_to_name(info_p->st_uid));
	printf("%-8s ", gid_to_name(info_p->st_gid));
	printf("%8ld ", (long)info_p->st_size);
	printf("%.12s ", 4+ctime(&info_p->st_mtime));
	printf("%s\n", filename);
}

void mode_to_letters(int mode, char str[])
{
	strcpy(str, "----------");

	if (S_ISDIR(mode)) { str[0] = 'd'; }
	if (S_ISCHR(mode)) { str[0] = 'c'; }
	if (S_ISBLK(mode)) { str[0] = 'b'; }

	if (mode & S_IRUSR) { str[1] = 'r'; }
	if (mode & S_IWUSR) { str[2] = 'w'; }
	if (mode & S_IXUSR) { str[3] = 'x'; }

	if (mode & S_IRGRP) { str[4] = 'r'; }
	if (mode & S_IWUSR) { str[5] = 'w'; }
	if (mode & S_IXGRP) { str[6] = 'x'; }

	if (mode & S_IROTH) { str[7] = 'r'; }
	if (mode & S_IWOTH) { str[8] = 'w'; }
	if (mode & S_IXOTH) { str[9] = 'x'; }
}

char * uid_to_name(uid_t uid)
{
	struct passwd * getpwuid(), *pw_ptr;
	static char numstr[100];

	if ( ( pw_ptr = getpwuid(uid) ) == NULL ) {
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else { return pw_ptr->pw_name; }
}

char * gid_to_name(gid_t gid)
{
	struct group * getgrgid(), *grp_ptr;
	static char numstr[100];

	if ( (grp_ptr = getgrgid(gid) ) == NULL ) {
		sprintf(numstr,"%d", gid);
		return numstr;
	} 
	else { return grp_ptr->gr_name; }
}

int cmp(const void * a, const void * b)
{
	return strcmp((char*)a, (char*)b);
}
