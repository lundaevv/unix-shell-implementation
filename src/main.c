/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 13:47:23 by gperedny          #+#    #+#             */
/*   Updated: 2026/01/08 03:21:04 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

/* Helper to run a single command pipeline */
static void run_single(t_shell *sh, char **argv)
{
    t_cmd cmd;
    t_pipeline p;

    cmd.argv = argv;
    cmd.redirs = NULL;
    cmd.redir_count = 0;

    p.cmds = &cmd;
    p.count = 1;

    exec_pipeline(sh, &p);
    printf("[status] %d\n", sh->exit_status);
}

/* Helper to run a 2-command pipeline */
static void run_pipe2(t_shell *sh, char **a0, char **a1)
{
    t_cmd cmds[2];
    t_pipeline p;

    cmds[0].argv = a0;
    cmds[0].redirs = NULL;
    cmds[0].redir_count = 0;

    cmds[1].argv = a1;
    cmds[1].redirs = NULL;
    cmds[1].redir_count = 0;

    p.cmds = cmds;
    p.count = 2;

    exec_pipeline(sh, &p);
    printf("[status] %d\n", sh->exit_status);
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    t_shell sh;

    sh.envp = env_copy(envp);
	if(!sh.envp)
	{
		perror("env_copy");
		return (1);
	}
    sh.exit_status = 0;
    sh.should_exit = 0;

    printf("========== STAGE 6 TEST HARNESS ==========\n");

    /* 1) pwd */
    printf("\n--- TEST 1: pwd ---\n");
    {
        char *a[] = {"pwd", NULL};
        run_single(&sh, a);
    }

    /* 2) export A=1, then env should contain A=1 */
    printf("\n--- TEST 2: export A=1 then env | grep A (manual check) ---\n");
    {
        char *a[] = {"export", "A=1", NULL};
        char *b[] = {"env", NULL};

        printf("[cmd] export A=1\n");
        run_single(&sh, a);

        printf("[cmd] env (look for A=1)\n");
        run_single(&sh, b);
    }

    /* 3) unset A, then env should NOT contain A=1 */
    printf("\n--- TEST 3: unset A then env (A should be gone) ---\n");
    {
        char *a[] = {"unset", "A", NULL};
        char *b[] = {"env", NULL};

        printf("[cmd] unset A\n");
        run_single(&sh, a);

        printf("[cmd] env (A should be gone)\n");
        run_single(&sh, b);
    }

    /* 4) cd .. changes cwd in parent */
    printf("\n--- TEST 4: cd .. changes cwd (pwd before/after) ---\n");
    {
        char *p1[] = {"pwd", NULL};
        char *cd[] = {"cd", "..", NULL};

        printf("[cmd] pwd (before)\n");
        run_single(&sh, p1);

        printf("[cmd] cd ..\n");
        run_single(&sh, cd);

        printf("[cmd] pwd (after)\n");
        run_single(&sh, p1);
    }

    /* 5) Builtin in pipeline does NOT persist: export B=2 | cat, then env */
    printf("\n--- TEST 5: pipeline builtin does NOT persist (export B=2 | cat) ---\n");
    {
        char *x0[] = {"export", "B=2", NULL};
        char *x1[] = {"cat", NULL};
        char *env[] = {"env", NULL};

        printf("[pipe] export B=2 | cat\n");
        run_pipe2(&sh, x0, x1);

        printf("[cmd] env (B=2 should NOT be present)\n");
        run_single(&sh, env);
    }

    /* 6) echo in pipeline: echo hello | wc -c */
    printf("\n--- TEST 6: echo hello | wc -c ---\n");
    {
        char *x0[] = {"echo", "hello", NULL};
        char *x1[] = {"wc", "-c", NULL};
        run_pipe2(&sh, x0, x1);
    }

    /* 7) exit sets should_exit flag */
    printf("\n--- TEST 7: exit sets should_exit flag ---\n");
    {
        char *e[] = {"exit", NULL};
        run_single(&sh, e);
        printf("[should_exit] %d\n", sh.should_exit);
    }

    printf("\n========== END TESTS ==========\n");
    return sh.exit_status;
}

