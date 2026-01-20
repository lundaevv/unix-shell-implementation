#include "minishell.h"

/*

This file executes ONE external command (not a builtin).

It is always called from a CHILD process (after fork),
either:
    - from exec_pipeline() when p->count == 1
    - from child_run_pipeline_cmd() when running a pipeline

So this function is allowed to call exit() directly:
if execve succeeds -> this function never returns
if execve fails -> we print error and exit with proper code

What parser gives me for each command:
    cmd->argv = ["ls", "-la", NULL]  (NULL-terminated)

What this function does:
1) If argv empty -> exit(0)
2) Resolve the executable path:
      - if argv[0] contains '/' -> treat as direct path
      - else search in PATH from envp
3) If not found -> print "command not found" and exit(127)
4) If found -> execve(path, argv, envp)
5) If execve fails -> perror and exit(127)

Exit codes:
- 127 is the common shell code for "command not found" / exec failure.
(We can refine later: permission denied is usually 126, but for now ok.)

Memory:
- resolve_path() returns malloc'ed string -> must be freed if execve fails
- if execve succeeds -> OS replaces process image, no need to free

 */

static int	ms_exec_error_code(void)
{
	if (errno == EACCES || errno == EISDIR)
		return (126);
	return (127);
}

void	exec_cmd(t_cmd *cmd, char **envp)
{
	char	*path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		exit(0);
	path = resolve_path(cmd->argv[0], envp);
	if (!path)
	{
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(path, cmd->argv, envp);
	perror(cmd->argv[0]);
	free(path);
	exit(ms_exec_error_code());
}

