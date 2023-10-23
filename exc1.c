//Antoniadhs Giorgos csd4840

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

char* readInput() {
	char* input;
	int length = 0, counter = 16, c;
	input = malloc(sizeof(char) * counter);
	c = getchar();
	while (c != EOF && c != '\n') {
		input[length++] = c;
		if (length == counter) {
			counter += 16;
			input = realloc(input, sizeof(char)*counter);
		}
		c = getchar();
	}
	input[length++] = '\0';
	return realloc(input, sizeof(char)*length);
}

void clearSpaces(char* input) {
	int i, start = 0, end = strlen(input) - 1;
	while (input[start] == ' ' || input[start] == '\t') ++start;
	while (end >= start && (input[end] == ' ' || input[end] == '\t')) --end;
	for (i = start; i <= end; ++i) {
		input[i - start] = input[i];
	}
	input[i - start] = '\0';
}

char** split_arguments(char* input, const char* delimeter) {
	char** arguments;
	char* token;
	int length = 0, counter = 4;
	arguments = malloc(sizeof(char*) * counter);
	token = strtok(input, delimeter);
	while (token) {
		arguments[length++] = token;
		if (length == counter) {
			counter += 4;
			arguments = realloc(arguments, sizeof(char*) * counter);
		}
		token = strtok(NULL, delimeter);
	}
	arguments[length++] = NULL; //Fail-safe variable. When iterating the array, this value will prevent from reading data outside of it
	return realloc(arguments, sizeof(char*) * length);
}

int check_special_commands(char* input) {
	//exit
	if (strstr(input, "quit")) {
		printf("\e[0m");
		exit(0);
	}
	//chdir command
	else if (strstr(input, "chdir")) {
		strtok(input, " ");
		chdir(strtok(NULL, " "));
		return 1;
	} else if (strstr(input, "about")) {
		printf("\033[0;36m" "Linux Shell Version 1.0. Can execute everything you want it to!\nMade by:\n");
		printf("\033[0;35m" "        GGGGGGGGGGGGG  iiii                                                             iiii                    222222222222222         000000000     \n");
 	 	printf("     GGG::::::::::::G i::::i                                                           i::::i                  2:::::::::::::::22     00:::::::::00   \n");
   		printf("   GG:::::::::::::::G  iiii                                                             iiii                   2::::::222222:::::2  00:::::::::::::00 \n");
   		printf("  G:::::GGGGGGGG::::G                                                                                          2222222     2:::::2 0:::::::000:::::::0\n");
    		printf(" G:::::G       GGGGGGiiiiiii    ooooooooooo   rrrrr   rrrrrrrrr      ggggggggg   gggggiiiiiii     ssssssssss               2:::::2 0::::::0   0::::::0\n");
    		printf("G:::::G              i:::::i  oo:::::::::::oo r::::rrr:::::::::r    g:::::::::ggg::::gi:::::i   ss::::::::::s              2:::::2 0:::::0     0:::::0\n");
    		printf("G:::::G               i::::i o:::::::::::::::or:::::::::::::::::r  g:::::::::::::::::g i::::i ss:::::::::::::s          2222::::2  0:::::0     0:::::0\n");
    		printf("G:::::G    GGGGGGGGGG i::::i o:::::ooooo:::::orr::::::rrrrr::::::rg::::::ggggg::::::gg i::::i s::::::ssss:::::s    22222::::::22   0:::::0 000 0:::::0\n");
    		printf("G:::::G    G::::::::G i::::i o::::o     o::::o r:::::r     r:::::rg:::::g     g:::::g  i::::i  s:::::s  ssssss   22::::::::222     0:::::0 000 0:::::0\n");
    		printf("G:::::G    GGGGG::::G i::::i o::::o     o::::o r:::::r     rrrrrrrg:::::g     g:::::g  i::::i    s::::::s       2:::::22222        0:::::0     0:::::0\n");
    		printf("G:::::G        G::::G i::::i o::::o     o::::o r:::::r            g:::::g     g:::::g  i::::i       s::::::s   2:::::2             0:::::0     0:::::0\n");
    		printf(" G:::::G       G::::G i::::i o::::o     o::::o r:::::r            g::::::g    g:::::g  i::::i ssssss   s:::::s 2:::::2             0::::::0   0::::::0\n");
    		printf("  G:::::GGGGGGGG::::Gi::::::io:::::ooooo:::::o r:::::r            g:::::::ggggg:::::g i::::::is:::::ssss::::::s2:::::2       2222220:::::::000:::::::0\n");
    		printf("   GG:::::::::::::::Gi::::::io:::::::::::::::o r:::::r             g::::::::::::::::g i::::::is::::::::::::::s 2::::::2222222:::::2 00:::::::::::::00\n");
    		printf("     GGG::::::GGG:::Gi::::::i oo:::::::::::oo  r:::::r              gg::::::::::::::g i::::::i s:::::::::::ss  2::::::::::::::::::2   00:::::::::00\n");
    		printf("        GGGGGG   GGGGiiiiiiii   ooooooooooo    rrrrrrr                gggggggg::::::g iiiiiiii  sssssssssss    22222222222222222222     000000000\n");
    		printf("                                                                              g:::::g\n");
    		printf("                                                                  gggggg      g:::::g\n");
    		printf("                                                                  g:::::gg   gg:::::g\n");
    		printf("                                                                   g::::::ggg:::::::g\n");
    		printf("                                                                    gg:::::::::::::g\n");
    		printf("                                                                      ggg::::::ggg\n");
    		printf("                                                                         gggggg\n" "\033[0;32m");
		return 1;
	}
	return 0;
}

void redirect_executor(char* filepath, int mode) {
	int fd;
	clearSpaces(filepath);
	switch (mode) {
		case 1: //Redirect output
			fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, 1);
			break;
		case 2: //Redirect input
			fd = open(filepath, O_RDONLY);
			dup2(fd, 0);
			break;
		case 3: //Redirect append
			fd = open(filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(fd, 1);
			break;
		default:
			return;
	}
}

void redirects_checker(char* input) {
	char* prev;
	char* next;
	int flag = 0;
	prev = strtok(input, "<"); //Seperating by input
	next = strtok(NULL, "<");
	if (next) {
		if (strstr(next, ">")) { //Input and then output
			if (strstr(next, ">>")) { //If the output is appended and not overwritive
				prev = strtok(next, ">>"); //In
				next = strtok(NULL, ">>"); //Out
				redirect_executor(prev, 2);
				redirect_executor(next, 3);
			} else {
				prev = strtok(next, ">"); //In
				next = strtok(NULL, ">"); //Out
				redirect_executor(prev, 2);
				redirect_executor(next, 1);
			}
		//	return 3;
		} else if (strstr(prev, ">")) { //Output and then input
			if (strstr(prev, ">>")) { //If the output is appended and not overwritive
				strtok(prev, ">>");
				prev = strtok(NULL, ">>");
				redirect_executor(prev, 3);
				redirect_executor(next, 2);
			} else {
				strtok(prev, ">");
				prev = strtok(NULL, ">"); //Output
				//next is Input
				redirect_executor(prev, 1);
				redirect_executor(next, 2);
			}
		//	return 3;
		} else { //Input only
			redirect_executor(next, 2);
		//	return 2;
		}
	} else if (strstr(prev, ">")) { //Output only
		if (strstr(prev, ">>")) { //Appended only output
			strtok(prev, ">>");
			next = strtok(NULL, ">>"); //Output
			redirect_executor(next, 3);
		} else {
			strtok(prev, ">");
			next = strtok(NULL, ">"); //Output
			redirect_executor(next, 1);
		}
		//return code here
		//return 1;
	}
//	return 0;
}

void pipe_executor(char* input) {
	//stdin = 0, stdout = 1, stderr = 2
	int i, pid, status, prev_output, fd[2]; //fd[0] is the read end and fd[1] is the write end
	char** args;
	char** pipe_data;
	pipe_data = split_arguments(input, "|");
	for(i=0; pipe_data[i]; ++i) {
		if (check_special_commands(pipe_data[i])) continue;
		if (pipe(fd)) {
			perror("An error occured while initializing the pipe...\n");
			break;
		}
		pid = fork();
		//Child
		if (pid == 0) {
			//If it is a special command then we quit the child instance
			//if (check_special_commands(pipe_data[i])) exit(0);
			if (pipe_data[i+1]) {
				dup2(fd[1], 1); //Redirect output to be pipe write end
				//close(fd[1]);
				//close(fd[0]);
			}
			if (i != 0) {
				dup2(prev_output, 0); //Redirect prev output to current input
				close(prev_output);
			}
			close(fd[0]);
			close(fd[1]);
			redirects_checker(pipe_data[i]);
			args = split_arguments(pipe_data[i], " ");
			execvp(args[0], args);
			//If we reach this line, this means that the command wasn't finished properly
			perror("Error: ");
			exit(1);
		} else if (pid > 0) { //Parent
			close(fd[1]);
			waitpid(-1, &status, 0); //Waiting for child process to finish
			if (i != 0) close(prev_output);
			prev_output = fd[0];
		} else { //Failure
			perror("Unfortunatelly, an error has been encountered which prevented the forking progress... :-(");
			break;
		}
	}
	close(prev_output);
	free(pipe_data);
}

void seperate_executor(char* input) {
	int i, pid, status;
	char** args;
	if (check_special_commands(input)) return;
	pid = fork();
	if (pid == 0) { //Child
		redirects_checker(input);
		args = split_arguments(input, " ");
		execvp(args[0], args);
		perror("Invalid input\n");
		exit(1);
	} else if (pid > 0) { //Parent
		waitpid(-1, &status, 0);
	} else { //Failure
		printf("Unfortunatelly, an error has been encountered which prevented the forking progress... :-(  (ProcessID = %d)\n", pid);
	}
}

void command_hub(char* input) {
	//Split all sub-commands seperated by comma first
	int i;
	char** command_list;
	command_list = split_arguments(input, ";");
	for (i=0; command_list[i]; ++i) {
		//If it is a piped command
		if (strstr(command_list[i], "|")) {
			pipe_executor(command_list[i]);
		} else {
			seperate_executor(command_list[i]);
		}
	}
	free(command_list);
}

int main(void) {
	char* input;
	char working_dir[1024], username[32];
	while (1) {
		getcwd(working_dir, sizeof(working_dir));
		getlogin_r(username, sizeof(username));
		if (working_dir && username) {
			printf("\033[0;32m" "csd4840-hy345sh@%s:%s ", 
			username, working_dir);
		} else {
			printf("\033[0;32m" "Error while fetching the current directory and/or user: ");
		}
		input = readInput();
		command_hub(input);
		//Cleanup
		free(input);
	}
	return 0;
}
