// 4 cases in my simple hls.
// ls /home
// ls /home -l
// ls -l
// ls

/* 21600786 Hong Seung Hun */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>

void get_help();
void print_normal(DIR *dp);
void print_long(DIR *dp, char *path);
char *time_convert(time_t rawTime);

int main(int argc, char **argv)
{
	//  option preprocessing
	bool long_opt = false;
	bool help = false;
	char *directory;
	int c;
	
	memcpy(directory, ".", strlen(".") + 1);
	if (argc > 1)
	{
		if (argv[1][0] == '/')
			memcpy(directory, argv[1], strlen(argv[1]) + 1);
	}

	while ((c = getopt(argc, argv, "lh")) != -1)
	{
		switch (c)
		{
		case 'l':
			long_opt = true;
			break;
		case 'h':
			get_help();
			break;
		case '?':
			perror("WRONG OPTION\n");
			exit(1);
		}
	}

	//directory stream을 열고, 얘를 함수에 보내자
	DIR *dp = NULL;
	if ((dp = opendir(directory)) == 0x0)
	{
		perror("CAN NOT OPEN DIRECTORY \n");
		exit(1);
	}

	// print results.
	if (long_opt == true)
		print_long(dp, directory);
	else
		print_normal(dp);

	closedir(dp);
	return 0;
}

void print_normal(DIR *dp)
{
	struct dirent *file = 0x0;
	int i = 0;
	while ((file = readdir(dp)) != 0x0)
	{
		if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0 || file->d_name[0] == '.')
			continue;

		printf("%s    ", file->d_name);
		i++;
		if (i % 4 == 0)
			printf("\n");
	}
	printf("\n");
}

void print_long(DIR *dp, char* directory)
{
	struct dirent *file = 0x0;
	struct stat sb;
	struct passwd *user;
	struct group *group;
	int fd;
	char* path;
	
	while ((file = readdir(dp)) != 0x0)
	{
		if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0 || file->d_name[0] == '.')
			continue;
		path = malloc(sizeof(directory)+sizeof(file->d_name)+1);
		sprintf(path, "%s/%s", directory, file->d_name);
		if (lstat(path, &sb) == -1)
		{
			perror("CAN NOT OPEN FILE STATUS \n");
			exit(1);
		}
		user = getpwuid(sb.st_uid);
		group = getgrgid(sb.st_gid);

		printf("%s", user->pw_name);
		printf(" %s", group->gr_name);
		printf(" %d", (int)sb.st_size);
		printf(" %s", time_convert(sb.st_mtime));
		printf(" %s\n", file->d_name);

		close(fd);
	}
}

char *time_convert(time_t rawTime)
{
	char *buf = malloc(sizeof(char) * 13);
	struct tm *ptime;
	ptime = localtime(&rawTime);
	int month = ptime->tm_mon + 1;
	int day = ptime->tm_mday;
	int hour = ptime->tm_hour;
	int min = ptime->tm_min;
	sprintf(buf, "%d월 %d일 %d:%d", month, day, hour, min);
	return buf;
}

void get_help()
{
	printf("HELLO :)\nIf you wanna ask something,\nPlease contact to 21600786@handong.edu\n");
	exit(1);
}