/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_limiter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lundaevv <lundaevv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 15:01:54 by lundaevv          #+#    #+#             */
/*   Updated: 2026/01/20 15:39:55 by lundaevv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ms_limiter_len(const char *s)
{
	size_t	len;
	int		i;
	char	q;

	len = 0;
	i = 0;
	q = 0;
	while (s && s[i])
	{
		if (ms_quote_step_i(s, &i, &q))
			continue ;
		len++;
		i++;
	}
	return (len);
}

char	*ms_unquote_limiter(const char *src)
{
	char	*dst;
	size_t	j;
	int		i;
	char	q;

	dst = (char *)malloc(ms_limiter_len(src) + 1);
	if (!dst)
		return (NULL);
	j = 0;
	i = 0;
	q = 0;
	while (src && src[i])
	{
		if (ms_quote_step_i(src, &i, &q))
			continue ;
		dst[j++] = src[i];
		i++;
	}
	dst[j] = '\0';
	return (dst);
}
