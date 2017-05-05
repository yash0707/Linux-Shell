#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>
#include <semaphore.h>
#define tok_bufff 64
#define readline_bufff 1024

char *func_str[] = {"cd","help","exit","printstack","clone","currentDirectory","gotoHome","man"};

void search(char *key)
{
	int i,j;
	int m=0;
	char ans[100][100];
	int length = sizeof(func_str)/sizeof(func_str[0]);
	for(i=0;i<length;i++)
	{
		int count=0;
		for(j=0;j<strlen(key);j++)
		{
			if(key[j] == func_str[i][j])
			{
				count++;
			}
		}
		if(count==strlen(key))
		{
			strcpy(ans[m++],func_str[i]);
		}
	}
	printf("Following commands start with %s:- \n",key );
	for(i=0;i<m;i++)
	{
		printf("%s\n",ans[i]);
	}
}

int launch_func(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{

		if (execvp(args[0], args) == -1)
		{
			perror("err");
		}
		exit(EXIT_FAILURE);
	}
	else if(pid < 0)
	{
		perror("err");
	}
	else
	{
		
		do
		{
			waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

int getch(void)
{
 int ch;
 struct termios oldt;
 struct termios newt;
 tcgetattr(STDIN_FILENO, &oldt); /*store old settings */
 newt = oldt; /* copy old settings to new settings */
 newt.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */
 tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */
 ch = getchar(); /* standard getchar call */
 tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /*reapply the old settings */
 return ch; /*return received char */
}

typedef struct node
{
	char str[100];
	struct node* next;
}stack_node;

stack_node *top = NULL;
stack_node *iter = NULL;

void push(char *abc)
{
	stack_node *s = (stack_node*) malloc(sizeof(stack_node));
	strcpy(s->str,abc);
	s->next = NULL;
	if(top==NULL)
	{
		top = s;
		
	}
	else{

		s->next = top;
		top = s;
	}

	//printf("%s\n",top->str );
	iter = s;
}

stack_node* pull()
{
	stack_node *st = (stack_node*)malloc(sizeof(stack_node));
	if(iter == NULL)
	{
		printf("Iterator is at NULL\n");
		return st;
	}
	
	strcpy(st->str,iter->str);
	iter = iter->next;
	
	return st;
}

int cd_comm(char **args);
int help_comm(char **args);
int exit_comm(char **args);
int printstack_func(char **args);
int clone(char **args);
int gotoHome(char **args);
int currentDirectory(char **args);
int man(char **args);

int (*func_calls[]) (char **) = {
&cd_comm,
&help_comm,
&exit_comm,
&printstack_func,
&clone,
&currentDirectory,
&gotoHome,
&man
};

int num_of_func()
{
	return sizeof(func_str) / sizeof(char *);
}

int printstack_func(char **args)
{
	stack_node * abc = top;
	while(abc)
	{
		printf("%s\n",abc->str);
		abc = abc->next;
	}

	return 1;
}

int currentDirectory(char **args)
{
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	printf("%s\n", cwd);
	return 1;
}

int gotoHome(char **args)
{
	//printf("in\n");
	chdir(getenv("HOME"));
	return 1;
}

int cd_comm(char **args)
{
	if (args[1] == NULL)
	{
		printf("expected argument to \"cd\"\n");
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("err");
		}
	}
	return 1;
}

int help_comm(char **args)
{
	int i;
	printf("\n\nYASH VARSHNEY(15103314),SHREY BATRA(15103308),AYUSHI JAIN(15103294)\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");

	for (i = 0; i < num_of_func(); i++)
	{
		printf(" %s\n", func_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}

int man(char **args)
{
	if(args[1]==NULL)
	{
		printf("Expected argument to \"man\"\n");
	}
	else{
		//printf("%s\n",args[1] );
		if(strcmp(args[1],"cd") == 0)
		{
			printf("OPEN THE DIRECTORY ENTERED BY YOU IF PRESENT.\nSYNTAX:- cd 'directory name'\n");
		}
		else if(strcmp(args[1],"gotoHome") == 0)
		{
			printf("TAKES YOU TO THE HOME DIRECTORY.\n SYNTAX:- gotoHome\n");
		}
		else if(strcmp(args[1],"currentDirectory") == 0)
		{
			printf("TELLS U THE CURRENT DIRECTORY.\n SYNTAX:- currentDirectory\n");
		}
		else if(strcmp(args[1],"printstack") == 0)
		{
			printf("TELLS U ALL THE HISTORY OF COMMANDS U HAVE USED. \n SYNTAX:- printstack\n");
		}
		else if(strcmp(args[1],"man") == 0)
		{
			printf("TELLS U HOW TO USE THE COMMAND AND SOME INFORMATION ANOUT THAT COMMAND.\n SYNTAX:- man 'command'\n");
		}
		else if(strcmp(args[1],"clone") == 0)
		{
			printf("OPEN THE NEW TERMINAL WINDOW AND TAKES U TO THE CURRENT DIRECTORY OPENED IN THE CURRENT TERMINAL WINDOW.\nSYNTAX:- clone");
		}
		else if(strcmp(args[1],"help") == 0)
		{
			printf("GIVES U ALL THE COMMAND.\nSYNTAX:- help\n");
		}
		else if(strcmp(args[1],"exit") == 0)
		{
			printf("EXIT THE TERMINAL WINDOW.\nSYNTAX:- exit\n");
		}
		else{
			return launch_func(args);
		}

	}
	return 1;
}

int exit_comm(char **args)
{
	return 0;
}

int clone(char **args)
{
	int status = system("gnome-terminal -e ./pred");
	return 1;
}




int execute_func(char **args)
{
	int i,j;

	if (args[0] == NULL)
	{
		return 1;
	}
	//printf("IN EXECUTE:- %s\n",args[0] );
	for (i = 0; i < num_of_func(); i++)
	{
		if (strcmp(args[0], func_str[i]) == 0)
		{
			return (*func_calls[i])(args);
		}
	}

	return launch_func(args);
}




char *readline_func(void)
{
	int bufsize = readline_bufff;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int i;
	char *line;
	if (!buffer)
	{
		printf("allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		int x=0;
		int y;
		int z;
		stack_node *s;
		x =getchar();
		if (x == 27)
		{
	   		y = getch();
		 	z = getch();

			if (x == 27 && y == 91)
			{
				//printf("ANDAR AA GYA\n");
				  switch (z)
				  {
				   case 65:
				   //printf("up arrow key pressed\n");
				   s= pull();
				   line = s->str;
				   strcpy(buffer,line);
				  // printf("%s\n",buffer);
				   printf("%s\n",line);
				   x=getchar();
				   position = strlen(buffer);	
				   //printf("completed\n");
				   
				   break;

				   case 66:
				   printf("down arrow key pressed\n");
				   break;

				   case 67:
				   printf("right arrow key pressed\n");
				   break;

				   case 68:
				   printf("left arrow key pressed\n");
				   break;
				  }
			}
		
		}
		/*else if(x=='\t')
		{
			int i;
			int m=0;
			printf("tab si pressed\n");
			
			char key[100];
			for(i=0;i<strlen(buffer);i++)
			{
				if(buffer[i] >=97 && buffer[i] <= 122)
				{
					key[m++]=buffer[i];
				}
				else{
					break;
				}
			}

			printf("BEFORE CALL:- %s\n",key );
			//printf("BE%d\n",(int)strlen(buffer));
			search(key);
			printf("AFTER CALL:- %s\n", key);
			//printf("AF%d\n",(int)strlen(buffer));

			position=0;
			char cwd[1024];
			getcwd(cwd,sizeof(cwd));
			printf("\n%s", cwd);
			printf("-->> ");

			
		}*/
		else if (x == EOF || x == '\n')
		{
			if(position > 0)
			{
				buffer[position++] = '\0';
				if(buffer[position-2]!='\t')
					push(buffer);
				return buffer;
			}
		}
		else
		{	
			buffer[position++] = x;
			//printf("%s\n",buffer );
		}
				


		if (position >= bufsize)
		{
			bufsize += readline_bufff;
			buffer = realloc(buffer, bufsize);
			if (!buffer)
			{
				printf("allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

	//printf("Position value  %d\n", position);

	}
	return buffer;
}




char **split_func(char *line)
{
	int bufsize = tok_bufff, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token, **tokens_backup;
	int i;
	if (!tokens)
	{
		printf("allocation error\n");
		exit(EXIT_FAILURE);
	}
	int count=0;
	int size=(int)strlen(line);
	//printf("%s",line);
	
	if(line[size-1] == '\t')
	{
		line[size-1]='\0';
		
		search(line);
		return tokens;
	}

	token = strtok(line," \t\r\n\a");
	
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize) 
		{
			bufsize += tok_bufff;
			tokens_backup = tokens;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens)
			{
				free(tokens_backup);
				printf("allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL," \t\r\n\a");
	}
	tokens[position] = NULL;
	return tokens;
}




void loop(void)
{
	char *line;
	char **args;
	int status;

	do
	{
		char cwd[1024];
		getcwd(cwd,sizeof(cwd));
		printf("\n%s", cwd);
		printf("-->> ");
		line = readline_func();
		//printf("IN LOOP:--   %s\n", line);
		


		args = split_func(line);
		


		//printf("\n\n%d\n",(int)strlen((args)[0]));
		//if(strcmp(*args,"NULL")!=0)
		status = execute_func(args);

		free(line);
		free(args);
		
	}while(status);
}



int main(int argc, char **argv)
{
	//gotoDesktop();
	//search("prin");
	//system("gnome-terminal -e ./pred");
	
	system("setterm -term linux -fore white");

	loop();

	//sem_t s1;



	return EXIT_SUCCESS;
}

