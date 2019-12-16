#include "icssh.h"
#include "linkedList.h"
#include <readline/readline.h>

int childDieFlag = 0;


int main(int argc, char* argv[]) {
	int exec_result;
	int exit_status;
	pid_t pid;
	pid_t wait_result;
	char* line;
	List_t bgEntryHead; 
	bgEntryHead.head = NULL; 
	bgEntryHead.length = 0; 
	bgEntryHead.comparator = &timeComparator; 
	char prevPath[100]; 
	getcwd(prevPath, 100); 

	 

#ifndef DEBUG
	rl_outstream = fopen("/dev/null", "w");
#endif

	// Setup segmentation fault handler
	if (signal(SIGSEGV, sigsegv_handler) == SIG_ERR) {
		perror("Failed to set signal handler");
		exit(EXIT_FAILURE);
	}
    // print the prompt & wait for the user to enter commands string
	while ((line = readline(SHELL_PROMPT)) != NULL) {
		
		signal(SIGCHLD, sigchldHandler);
		if (childDieFlag == 1)
		{
			int pid;
			int status; 
			//while ((pid = wait(NULL)) > 0) 
			while((pid = waitpid(-1, &status, WNOHANG)) > 0)
			{
				bgentry_t* bgpNeedRemove = removeByPid(&bgEntryHead, pid); 
				
				if (bgpNeedRemove != NULL)
					printf(BG_TERM, pid, bgpNeedRemove->job->line); 
			}
			childDieFlag = 0; 
		}
		
        // MAGIC HAPPENS! Command string is parsed into a job struct
        // Will print out error message if command string is invalid
		job_info* job = validate_input(line);
        if (job == NULL) { // Command was empty string or invalid
			free(line);
			continue;
		}

        //Prints out the job linked list struture for debugging
        debug_print_job(job);

		if (strcmp(job->procs->cmd, "exit") == 0) {
			// Terminating the shell
			free(line);
			free_job(job);

			if (bgEntryHead.length == 0)
				return  0;

			while (bgEntryHead.head != NULL){
				bgentry_t* bgpNeedRemove = removeFront(&bgEntryHead);
				if (bgpNeedRemove != NULL)
					printf(BG_TERM, pid, bgpNeedRemove->job->line); 
				kill(bgpNeedRemove->pid, SIGKILL);
			}
			return 0;
		}

		else if (strcmp(job->procs->cmd, "cd") == 0)
		{
			if (job->procs->argc == 1)
				chdir(getenv("HOME"));
			else
			{
				char* target = job->procs->argv[1]; 
				if (strcmp(target, ".") == 0)
				{

				}

				else if (strcmp(target, "-") == 0)
				{
					//getcwd(prevPath, 100); 
					if (chdir(prevPath) == 0)
					{
						char cwd[100]; 
						if (getcwd(cwd, sizeof(cwd)) != NULL)
						{
							printf("%s\n", cwd); 
						}
					} 
				}
				else if (strcmp(target, "..") == 0)
				{
					getcwd(prevPath, 100);
					if (chdir("..") == 0)
					{ 
						char cwd[100]; 
						if (getcwd(cwd, sizeof(cwd)) != NULL)
						{
							printf("%s\n", cwd); 
						}
					}
					else
					{
						fprintf(stderr, DIR_ERR); 
					}
				}
				else
				{
					char* relativePathPrefix = "./"; 
					char relativePath[100]; 
					strcpy(relativePath, relativePathPrefix); 
					strcat(relativePath, target); 
					getcwd(prevPath, 100);
					if (chdir(relativePath) == 0)
					{
						char cwd[100]; 
						if (getcwd(cwd, sizeof(cwd)) != NULL)
						{
							printf("%s\n", cwd); 
						}
					} 
					else
					{
						fprintf(stderr, DIR_ERR); 
					}
				}
			} 
			free(line); 
			free_job(job); 
			continue; 
		}

		else if (strcmp(job->procs->cmd, "estatus") == 0)
		{
			printf("%d\n", WEXITSTATUS(exit_status)); 
			free(line); 
			free_job(job); 
			continue; 
		}

		else if (strcmp(job->procs->cmd, "bglist") == 0)
		{
			//printf("%d\n", WEXITSTATUS(exit_status)); 
			if (bgEntryHead.head != NULL)
			{
				//dup2(3,2); 
				node_t* head = bgEntryHead.head;
				node_t* current = head;
				
				while (current != NULL) {
					print_bgentry((bgentry_t*)current->value); 
					current = current->next;
				}
				//close(2); 
				free(line); 
				free_job(job); 
			}
			continue; 
		}


		else if (job->bg == 1)
		{

			if ((pid = fork()) < 0)
			{
				perror("fork error");
				exit(EXIT_FAILURE);
			}
			
			if (pid == 0)
			{
				if (job->nproc == 1)
				{
					proc_info* proc = job->procs;
					exec_result = execvp(proc->cmd, proc->argv); 
					if (exec_result < 0) 
					{  //Error checking
						printf(EXEC_ERR, proc->cmd);
						exit(EXIT_FAILURE);
					}
					exit(EXIT_SUCCESS);
				}
			}
			else
			{
				bgentry_t* bgInfo = malloc(sizeof(bgentry_t)); 
				bgInfo->job = job; 
				bgInfo->pid = pid; 
				bgInfo->seconds = time(NULL); 
				insertInOrder(&bgEntryHead, bgInfo); 
				continue;  
			}
		}

		if (job->nproc>1)
		{
			int n = 1; 
			proc_info* temp = job->procs; 
			int pipeTable[100];  

			int i = 1; 
			for (; i<=job->nproc - 1; i++)
			{
				pipe(pipeTable+(i-1)*2); 
			}

			List_t pidList; 
			pidList.length = 0; 
			pidList.comparator = NULL;
			
			for (; n<=job->nproc; n++)
			{
				int* pid = malloc(sizeof(int)); 
				*pid = fork(); 
				if (*pid == 0)
				{
					//printf("%d", n); 

					if (temp->in_file != NULL)
					{
						if( access(temp->in_file, F_OK ) != -1 ) 
						{
							int fd1In = open(temp->in_file, O_RDONLY); 
							dup2(fd1In, 0); 
							// file exists
						} 
						else 
						{
							fprintf(stderr, RD_ERR);
							exit(EXIT_FAILURE); 
							// file doesn't exist
						}
					}

					if (temp->out_file != NULL)
					{
						int fd1Out = creat(temp->out_file, S_IRWXU); 
						dup2(fd1Out, 1); 
					}

					if (temp->err_file != NULL)
					{
						int fd1In = creat(temp->err_file, S_IRWXU); 
						dup2(fd1In, 2); 
					}

					if (n == 1)
					{
						dup2(pipeTable[1], 1); 
						close(pipeTable[0]); 	
						close(pipeTable[1]); 
					}

					else if (n < job->nproc && n > 1)
					{
						dup2(pipeTable[2*n-4], 0);
						dup2(pipeTable[2*n-1], 1); 
						close(pipeTable[2*n-4]); 
						close(pipeTable[2*n-1]); 
					}

					else if (n == job->nproc)
					{
						dup2(pipeTable[2*n-4], 0); 
						close(pipeTable[2*n-4]); 
					}
					exec_result = execvp(temp->cmd, temp->argv);

				}
				else
				{
					if (n != job->nproc)
						close(pipeTable[2*n-1]); 
					insertFront(&pidList, pid);
					temp = temp->next_proc; 
				}
				continue; 
			}
			
			while (pidList.head != NULL)
			{
				waitpid(*(int*)removeFront(&pidList), &exit_status, 0);
			}
			continue; 
		}
		
		// example of good error handling!
		if ((pid = fork()) < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {  //If zero, then it's the child process
            //get the first command in the job list

		    proc_info* proc = job->procs;

			if (proc->in_file != NULL)
			{
				if( access(proc->in_file, F_OK ) != -1 ) 
				{
					int fd1In = open(proc->in_file, O_RDONLY); 
					dup2(fd1In, 0); 
					// file exists
				} 
				else 
				{
					fprintf(stderr, RD_ERR);
					exit(EXIT_FAILURE); 
					// file doesn't exist
				}
			}

			if (proc->out_file != NULL)
			{
				int fd1Out = creat(proc->out_file, S_IRWXU); 
				dup2(fd1Out, 1); 
			}

			if (proc->err_file != NULL)
			{
				int fd1In = creat(proc->err_file, S_IRWXU); 
				dup2(fd1In, 2); 
			}

			exec_result = execvp(proc->cmd, proc->argv);
			if (exec_result < 0) {  //Error checking
				printf(EXEC_ERR, proc->cmd);
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		} else {
            // As the parent, wait for the foreground job to finish
			wait_result = waitpid(pid, &exit_status, 0);
			if (wait_result < 0) {
				printf(WAIT_ERR);
				exit(EXIT_FAILURE);
			}
		}
		free_job(job);  // if a foreground job, we no longer need the data
		free(line);
	}
#ifndef DEBUG
	fclose(rl_outstream);
#endif
	return 0;
}
