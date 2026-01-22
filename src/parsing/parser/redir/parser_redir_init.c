/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_init.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 19:28:22 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/22 19:29:03 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_redirs(t_redir *r, int count)
{
	int	i;

	i = 0;
	while (i < count)
		r[i++].hd_fd = -1;
}
