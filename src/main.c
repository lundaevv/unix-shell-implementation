/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 13:47:23 by gperedny          #+#    #+#             */
/*   Updated: 2026/01/17 21:31:32 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

/*
Needs to follow norms.

This file is the main control loop of minishell.

It does NOT parse commands and does NOT execute commands itself.
It only connects everything together:

    input->parser->executor

My execution part is already finished!!!
Your parser will be plugged in HERE.

Program flow:

1) Initialize shell state (environment, exit status, exit flag)
2) Loop forever:
      - set interactive signals (Ctrl-C, Ctrl-\ behavior)
      - read a line with readline()
      - send this line to the parser
      - parser returns a ready t_pipeline structure
      - executor runs this pipeline
      - free all parser memory
3) Exit when user presses Ctrl-D or runs builtin `exit`



HOW YOU MUST CONNECT YOUR PARSER HERE

Right now I use parse_stub() ONLY FOR TESTING execution.

You must replace this part:

    parse_stub(line, &p)

With your real parser:

    parse_line(line, &sh, &p)

Your parser(IN THEORY) MUST follow this contract:

    int parse_line(const char *line, t_shell *sh, t_pipeline *out);

Rules:

1) On success return 0
2) On syntax error return non-zero
3) You must fully fill t_pipeline:

    out->count = number of commands in pipeline

    out->cmds = malloc array of t_cmd with size = out->count

Each t_cmd must contain:

    cmd.argv = NULL-terminated array
        Example: ["ls", "-la", NULL]

    cmd.redirs = array of t_redir (or NULL)

    cmd.redir_count = number of redirections

Redirections must already be classified as:

    REDIR_IN, REDIR_OUT, REDIR_APPEND, REDIR_HEREDOC

My executor assumes ALL parsing is already correct.
I DON'T handle quotes, $, tokens, syntax errors.



MEMORY RULES BETWEEN PARSER AND EXECUTOR

Your parser OWNS all memory it creates.

After execution I will call:

    free_pipeline(&p);

You must implement:

    void free_pipeline(t_pipeline *p);

That function must free:

    - each argv array
    - each redirs array
    - cmds array itself

Execution never frees parser memory.



SIGNALS

Before readline():
    signals_interactive()

During execution:
    exec_pipeline() switches to parent/child signal modes itself.

So parser does NOT touch signals.


*/

int main(int argc, char **argv, char **envp)
{
    t_shell sh;
    char    *line;

    (void)argc;
    (void)argv;

    /* Copy system env into our own memory.
       Builtins (export/unset) modify sh.envp. */
    sh.envp = env_copy(envp);
    if (!sh.envp)
        return (perror("env_copy"), 1);

    /* exit_status = status of last executed command
       should_exit = set by builtin exit */
    sh.exit_status = 0;
    sh.should_exit = 0;

    while (!sh.should_exit)
    {
        /* Ctrl-C prints new prompt, Ctrl-\ ignored */
        signals_interactive();

        /* Read user input */
        line = readline("minishell$ ");
        if (!line)
        {
            /* Ctrl-D => exit shell */
            write(1, "exit\n", 5);
            break;
        }

        if (*line)
            add_history(line);

        if (*line)
        {
            t_pipeline p;

            /* TEMPORARY!!!!! stub parser for testing execution */
            if (parse_stub(line, &p) == 0)
            {
                /* Run pipeline */
                exec_pipeline(&sh, &p);

                /* Free parser memory */
                free_pipeline_stub(&p);
            }
        }

        free(line);
    }

    return (sh.exit_status);
}

