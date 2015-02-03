/* 
 * Skeleton code for Shell processing
 * This file contains skeleton code for executing commands parsed in main-x.c.
 * Acknowledgement: derived from UCLA CS111
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "cmdline.h"
#include "myshell.h"


pid_t z_pid[512];
int z_init = 0;
/** 
 * Reports the creation of a background job in the following format:
 *  [job_id] process_id
 * to stderr.
 */
void report_background_job(int job_id, int process_id);

/* command_exec(cmd, pass_pipefd)
 *
 *   Execute the single command specified in the 'cmd' command structure.
 *
 *   The 'pass_pipefd' argument is used for pipes.
 *   On input, '*pass_pipefd' is the file descriptor used to read the
 *   previous command's output.  That is, it's the read end of the previous
 *   pipe.  It equals STDIN_FILENO if there was no previous pipe.
 *   On output, command_exec should set '*pass_pipefd' to the file descriptor
 *   used for reading from THIS command's pipe.
 *   If this command didn't have a pipe -- that is, if cmd->commandop != PIPE
 *   -- then it should set '*pass_pipefd = STDIN_FILENO'.
 *
 *   Returns the process ID of the forked child, or < 0 if some system call
 *   fails.
 *
 *   You must also handle the internal commands "cd" and "exit".
 *   These are special because they must execute in the shell process, rather
 *   than a child.  (Why?)
 *
 *   However, these special commands still have a status!
 *   For example, "cd DIR" should return status 0 if we successfully change
 *   to the DIR directory, and status 1 otherwise.
 *   Thus, "cd /tmp && echo /tmp exists" should print "/tmp exists" to stdout
 *   iff the /tmp directory exists.
 *   Not only this, but redirections should work too!
 *   For example, "cd /tmp > foo" should create an empty file named 'foo';
 *   and "cd /tmp 2> foo" should print any error messages to 'foo'.
 *
 *   How can you return a status, and do redirections, for a command executed
 *   in the parent shell?
 *   Hint: It is easiest if you fork a child ANYWAY!
 *   You should divide functionality between the parent and the child.
 *   Some functions will be executed in each process.
 */
static pid_t
command_exec(command_t *cmd, int *pass_pipefd)
{
	pid_t pid = -1;		// process ID for child
	int pipefd[2];		// file descriptors for this process's pipe
	static const char *CMD_EXIT = "exit";
	static const char *CMD_CD = "cd";
	static const char *CMD_JOBS = "jobs";
	char line[512];

	/* EXERCISE: Complete this function!
	 * We've written some of the skeleton for you, but feel free to
	 * change it.
	 */
	// Create a pipe, if this command is the left-hand side of a pipe.
	// Return -1 if the pipe fails.
	if (cmd->controlop == CMD_PIPE) {
		/* Your code here. */
		if (pipe(pipefd) < 0) {
			perror("pipe error\n");
			return -1;
		}
		//printf("PIPE\n");
	}

	// Fork the child and execute the command in that child.
	// You will handle all redirections by manipulating file descriptors.
	//
	// This section is fairly long.  It is probably best to implement this
	// part in stages, checking it after each step.  For instance, first
	// implement just the fork and the execute in the child.  This should
	// allow you to execute simple commands like 'ls'.  Then add support
	// for redirections: commands like 'ls > foo' and 'cat < foo'.  Then
	// add parentheses, then pipes, and finally the internal commands
	// 'cd' and 'exit'.
	//
	// In the child, you should:
	//    1. Set up stdout to point to this command's pipe, if necessary.
	//    2. Set up stdin to point to the PREVIOUS command's pipe (that
	//       is, *pass_pipefd), if appropriate.
	//    3. Close some file descriptors.  Hint: Consider the read end
	//       of this process's pipe.
	//    4. Set up redirections.
	//       Hint: For output redirections (stdout and stderr), the 'mode'
	//       argument of open() should be set to 0666.
	//    5. Execute the command.
	//       There are some special cases:
	//       a. Parentheses.  Execute cmd->subshell.  (How?)
	//       b. A null command (no subshell, no arguments).
	//          Exit with status 0.
	//       c. "exit".
	//       d. "cd".
	//
	if ((pid = fork()) < 0) {
		perror("fork error");
		return pid;
	}
	// child process
	else if (pid == 0) {

		int fd_re[3], i;

		if (cmd->controlop == CMD_PIPE) {
			// close the read end
			close(pipefd[0]);
			// set stdout to pipe write end
			if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
				perror("dup2 stdout error in child\n");
				close(pipefd[1]);
			}
			//close(pipefd[1]);
			if (dup2(*pass_pipefd, STDIN_FILENO) < 0) {
				perror("dup2 stdin error in child\n");
				close(*pass_pipefd);
			}

		}
		// configure input/output redirection
		for (i = 0; i < 3; i++) {
			if (cmd->redirect_filename[i] != 0) {
				if (i == 0)
					fd_re[i] = open(cmd->redirect_filename[i], O_RDONLY);
				else if (i > 0)
					fd_re[i] = open(cmd->redirect_filename[i],
							O_RDWR | O_CREAT | O_TRUNC, 0666);

				if (fd_re[i] < 0)
					perror("open redirect file error");
				dup2(fd_re[i], i);
				close(fd_re[i]);
			}
		}


		// execute the command
		if (cmd->subshell != NULL) {
			//printf("child run subshell\n");
			exit(command_line_exec(cmd->subshell));
		} else if (cmd->argv[0] != NULL) {
			//printf("child run %s\n", cmd->argv[0]);
			/*if (execvp(cmd->argv[0], cmd->argv) < 0) {
				perror("child cd\n");
				exit(1);
			}*/
			exit(execvp(cmd->argv[0], cmd->argv));
		} else {
			//printf("child empty cmmand\n");
			exit(0);
		}
	}

	// parent process
	// In the parent, you should:
	//    1. Close some file descriptors.  Hint: Consider the write end
	//       of this command's pipe, and one other fd as well.
	//    2. Handle the special "exit" and "cd" commands.
	//    3. Set *pass_pipefd as appropriate.
	//
	// "cd" error note:
	// 	- Upon syntax errors: Display the message
	//	  "cd: Syntax error on bad number of arguments"
	// 	- Upon system call errors: Call perror("cd")
	//
	// "cd" Hints:
	//    For the "cd" command, you should change directories AFTER
	//    the fork(), not before it.  Why?
	//    Design some tests with 'bash' that will tell you the answer.
	//    For example, try "cd /tmp ; cd $HOME > foo".  In which directory
	//    does foo appear, /tmp or $HOME?  If you chdir() BEFORE the fork,
	//    in which directory would foo appear, /tmp or $HOME?
	//

	/* Your code here. */
	if (cmd->argv[0] != NULL) {
		if (strcmp(cmd->argv[0], CMD_EXIT) == 0) {
			//printf("exit! good bye\n");
			exit(0);
		} else if (strcmp(cmd->argv[0], CMD_CD) == 0) {
/*			if (0 == chdir(cmd->argv[1])) {
				return 0;
			} else {
				perror("cd error");
				return -1;
			}*/
			chdir(cmd->argv[1]);
		} else if (strcmp(cmd->argv[0], CMD_JOBS) == 0) {
			int k = 0, z_status;
			while (z_pid[k] != 0) {

				if (waitpid(z_pid[k], &z_status, WNOHANG) == 0)
					report_background_job(k, z_pid[k]);

				k++;
			}

		} else if (cmd->controlop == CMD_PIPE) {
			int n;
			// close the write end
			close(pipefd[1]);

			if (dup2(pipefd[0], STDIN_FILENO) < 0)
				perror("dup2 stdin error parent\n");
			*pass_pipefd = dup(pipefd[0]);
			//n = read(pipefd[0], line, 512);
			//printf("p read %s\n", line);
			//write(*pass_pipefd, line, n);
			//close read end
			//close(pipefd[0]);
		}
	}

	// return the child process ID
	return pid;
}


/* command_line_exec(cmdlist)
 *
 *   Execute the command list.
 *
 *   Execute each individual command with 'command_exec'.
 *   String commands together depending on the 'cmdlist->controlop' operators.
 *   Returns the exit status of the entire command list, which equals the
 *   exit status of the last completed command.
 *
 *   The operators have the following behavior:
 *
 *      CMD_END, CMD_SEMICOLON
 *                        Wait for command to exit.  Proceed to next command
 *                        regardless of status.
 *      CMD_AND           Wait for command to exit.  Proceed to next command
 *                        only if this command exited with status 0.  Otherwise
 *                        exit the whole command line.
 *      CMD_OR            Wait for command to exit.  Proceed to next command
 *                        only if this command exited with status != 0.
 *                        Otherwise exit the whole command line.
 *      CMD_BACKGROUND, CMD_PIPE
 *                        Do not wait for this command to exit.  Pretend it
 *                        had status 0, for the purpose of returning a value
 *                        from command_line_exec.
 */
int
command_line_exec(command_t *cmdlist)
{
	int cmd_status = 0;	    // status of last command executed
	int z_status = 0;
	int pipefd = STDIN_FILENO;  // read end of last pipe
	int ret;
	pid_t pid = -1;
	//pid_t z_pid[512], i, j;
	int i, j;
	i = 0;
	if (z_init == 0) {
		memset(z_pid, 0, 512);
		z_init = 1;
	}

	
	while (cmdlist) {
		int wp_status;	    // Hint: use for waitpid's status argument!
				    // Read the manual page for waitpid() to
				    // see how to get the command's exit
				    // status (cmd_status) from this value.

		// TODO: Fill out this function!
		// If an error occurs in command_exec, feel free to abort().
		
		pid = command_exec(cmdlist, &pipefd);
		if (cmdlist->controlop != CMD_BACKGROUND
				&& cmdlist->controlop != CMD_PIPE) {
			pid = waitpid(pid, &cmd_status, 0);
			ret = WIFEXITED(cmd_status);
			cmd_status = WEXITSTATUS(cmd_status);
		} else {
			z_pid[i++] = pid;
		}

		switch (cmdlist->controlop) {
		case CMD_END:
			//goto done;
		case CMD_SEMICOLON:
			break;
		case CMD_BACKGROUND:
		case CMD_PIPE:
			cmd_status = 0;
			break;
		case CMD_AND:
			printf("child status %d", cmd_status);
			if (ret && cmd_status != 0 && cmd_status != 255) {
				printf("child status %d, AND false\n", cmd_status);
				goto done;
			}
			else
				printf("child status %d, AND true\n", cmd_status);
			break;
		case CMD_OR:
			if (ret && cmd_status == 0)
				goto done;
			break;
		default:
			perror("unknown CMD type ");
			abort();
		}

		cmdlist = cmdlist->next;
	}

done:
	j = 0;
	while (z_pid[j] != 0 ) {
		waitpid(z_pid[j++], &z_status, WNOHANG);
	}
	return cmd_status;
}

void report_background_job(int job_id, int process_id) {
    fprintf(stderr, "[%d] %d\n", job_id, process_id);
}
