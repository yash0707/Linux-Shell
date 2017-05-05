#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>
#include <semaphore.h>

int main()
{
	system("gnome-terminal -e ./pred");
	return 0;

}