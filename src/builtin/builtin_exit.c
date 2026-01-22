/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:05:42 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 19:07:11 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_num_required(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static void	print_too_many(void)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
}

int	bi_exit(t_shell *sh, t_cmd *cmd)
{
	long long	n;

	if (!sh || !cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (!cmd->argv[1])
	{
		sh->should_exit = 1;
		return (sh->exit_status);
	}
	if (!ms_atoll_checked(cmd->argv[1], &n))
	{
		print_num_required(cmd->argv[1]);
		sh->should_exit = 1;
		sh->exit_status = 2;
		return (2);
	}
	if (cmd->argv[2])
	{
		print_too_many();
		return (1);
	}
	sh->should_exit = 1;
	return ((unsigned char)n);
}
