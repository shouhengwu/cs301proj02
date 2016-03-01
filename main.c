#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <errno.h>

/*Adapted from previous lab*/
char** tokenify(const char *s, const char *delimiters) {
    char *copy = strdup(s); //first copy to get the size 
    char *token = strtok(copy, delimiters);

    int counts = 1; //account for NULL

    for (; token != NULL; token = strtok(NULL, delimiters)){
        counts++;
    }

    free(copy);

    char **rv = malloc(counts * sizeof(char *));

    char *copy1 = strdup(s);    //second copy to tokenify
    
    int i = 0;
    token = strtok(copy1, delimiters);

    while (token != NULL){
        *(rv + i) = strdup(token);  //rv[i]
        token = strtok(NULL, delimiters);
        i++;
    }
    *(rv + i) = NULL;

    free(copy1);

    return rv;
}

/*borrowed from previous lab*/
void free_tokens(char **tokens) {
    int i = 0;
    while (tokens[i] != NULL) {
        free(tokens[i]); // free each string
        i++;
    }
    free(tokens); // then free the array
}

/*Remove comment from tokens by replacing the first
comment token by NULL*/
void remove_comment(char **tokens){
	for (int i = 0; tokens[i] != NULL; i++){
		if (tokens[i][0] == '#'){
			tokens[i] = NULL;
			return;
		}
	}
}

char ***get_commands(char **tokens){
    int count = 1;
    for (int i = 0; tokens[i] != NULL; i++){
        count++;
    }

    char ***rv = malloc(count * sizeof(char **));

    int j = 0;
    for (int i = 0; tokens[i] != NULL; i++){
        char **temp = tokenify(tokens[i], " \t\n");
        if (temp[0] != NULL){ //add non-empty input
            rv[j] = temp;
            j++;
        }
        else{
            free_tokens(temp); //free empty input
        }
    }

    rv[j] = NULL;
    return rv;
}

void free_commands(char ***cmd){
    for (int i = 0; cmd[i] != NULL; i++){
        free_tokens(cmd[i]);
    }
    free(cmd);
}

int get_input(char *buffer){
    while (fgets(buffer, 1024, stdin) != NULL){
        return 0; //0-normal input
    }
    return 1; //1-eof or error
}

int check_exit(char **cmd, int *p_done){
    if (strcasecmp(cmd[0], "exit") == 0){
        *p_done = 1;
        return 0; //0-exit command found
    }
    return 1; //1-exit command not found
}

int check_mode(char **cmd, int *p_mode){
    if (strcasecmp(cmd[0], "mode") == 0){
        if (cmd[1] == NULL){
            if (*p_mode == 1){
                printf("The current mode is sequential.\n");
            }
            else{
                printf("The current mode is parallel.\n");
            }
        } //end of cmd[1]==NULL if

        else if (strcasecmp(cmd[1], "sequential") == 0 || strcasecmp(cmd[1], "s") == 0){
            *p_mode = 1;
        }

        else if (strcasecmp(cmd[1], "parallel") == 0 || strcasecmp(cmd[1], "p") == 0){
            *p_mode = 0;
        }

        else{
            fprintf(stderr, "Invalid mode!\n");
        }

        return 0; //0-mode command found
    }

    return 1; //1-mode command not found
}


//-----------------------------------------------

struct node{

	char directory[256];
	struct node *next;

};

void list_append(struct node **head, char line[256]){
	if(*head == NULL){
		*head = malloc(sizeof(struct node));
		strlcpy((*head)->directory, line, sizeof((*head)->directory));
		(*head)->next = NULL;
	}
	else{
		struct node *current = *head;
		while(NULL != current->next){
			current = current->next;
		}//end while
		current->next = malloc(sizeof(struct node));
		strlcpy(current->next->directory, line, sizeof(current->next->directory));
		current->next->next = NULL;

	}//end else

}//end add

char* construct_path(const char *directory, const *char name){

	char *rv = malloc(sizeof(char) * (strlen(*directory) + strlen(*name) + 1));
	int pos = 0; 
	for(int i = 0; i < strlen(directory); i++){
		rv[pos] = direcotry[i];
		pos++;
	}//end for

	for(int i = 0; i < strlen(name); i++){
		rv[pos] = name[i];
		pos++;
	}//end for

	rv[pos] = '\0';

	return rv;
}//end construct_path

const struct node **path_list_head(){

	FILE *fp;
	fp = fopen("shell-config", "r");
	if(fp == NULL){
		fprintf(stderr, "Failed to open shell-config.\n");
	}//end if
	char line[256];
	line[0] = '\0';

	struct node *first_node = malloc(sizeof(struct node ));
	first_node = NULL;
	struct node **path_list_head = malloc(sizeof(struct node *));

	path_list_head = &first_node;
	
	while(NULL != fgets(line, sizeof(line), fp)){
		for(int i = 0; i < strlen(line); i++){
			if(line[i] == '\n'){//get rid of the new line character
				line[i] = '\0';
				break;
			}//end if
		}//end for

		list_append(path_head, line);
	}//end while

}//end path_list_head


//-----------------------------------------------



int main(int argc, char **argv) {

	const struct node **path_list_head = construct_path_list();



	
	struct node *curr = *path_list_head;
	while(NULL != curr){
		
		struct stat statresult;
		char *path = construct_path(curr->directory, program_name);
		int rv = stat(path, &statresult);
		if(rv >= 0){//that the file as given exists, modify the entry in cmd[cmd_n][0] so that they contain the full path 
			break;
		}//end if

		curr = curr->next;
	}//end while
	
	fclose(fp);











    const char prompt[] = "prompt>>> ";

    int done = 0;
    int mode = 1; //1-sequential, 0-parallel

    while (!done){

	int current_mode = mode;

    	printf("%s", prompt);
    	fflush(stdout);

    	char buffer[1024];
    	if (get_input(buffer) == 1){ //eof encountered
            printf("\n");
            break;
        }

        char **tokens = tokenify(buffer, ";");
        remove_comment(tokens);
        char ***cmd = get_commands(tokens);

        free_tokens(tokens);

        int cmd_n = 0;






	








        while (cmd[cmd_n] != NULL){

            if (check_exit(cmd[cmd_n], &done) == 0){ //exit found
                cmd_n++;
                continue;
            }

            if (check_mode(cmd[cmd_n], &mode) == 0){ //mode found
                cmd_n++;
                continue;
            }

            int status;
            pid_t pid = fork();
            
            if (pid == 0){ //child process, executes a command

                if (execv(cmd[cmd_n][0], cmd[cmd_n]) < 0){
                    fprintf(stderr, "execv failed: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
            else if (pid > 0){ //parent process, wait in sequential mode, execute next command in parallel mode

		if(current_mode == 1){ 
               	    waitpid(pid, &status, 0);
                    cmd_n++;
		}//end if
		else{
		    cmd_n++;
		}//end else
            }
            else{   //fork failed, quit
                fprintf(stderr, "fork failed: %s\n", strerror(errno));
                free_commands(cmd);
                return 1;
            }
        } //end of cmd while 

        free_commands(cmd);
    } //end of done while

    return 0;
}

