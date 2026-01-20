/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 15:48:39 by lundaevv          #+#    #+#             */
/*   Updated: 2026/01/20 15:40:38 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Return 1 if we consumed a quote char (opened/closed quotes).
** Return 0 if current char should be treated as a normal char.
**
** Rules:
** - when q == 0:
**     '\'' or '"' opens quote mode (char is NOT copied)
** - when q == '\'':
**     '\'' closes quote mode (char is NOT copied)
** - when q == '"':
**     '"' closes quote mode (char is NOT copied)
** - other quotes inside quotes are normal characters
*/
int	ms_quote_step_i(const char *s, int *i, char *q)
{
	if (*q == 0 && (s[*i] == '\'' || s[*i] == '"'))
	{
		*q = s[*i];
		(*i)++;
		return (1);
	}
	if (*q == '\'' && s[*i] == '\'')
	{
		*q = 0;
		(*i)++;
		return (1);
	}
	if (*q == '"' && s[*i] == '"')
	{
		*q = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

int	ms_quote_step_p(const char **p, char *q)
{
	if (*q == 0 && (**p == '\'' || **p == '"'))
	{
		*q = **p;
		(*p)++;
		return (1);
	}
	if (*q == '\'' && **p == '\'')
	{
		*q = 0;
		(*p)++;
		return (1);
	}
	if (*q == '"' && **p == '"')
	{
		*q = 0;
		(*p)++;
		return (1);
	}
	return (0);
}
