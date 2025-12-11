/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 14:08:14 by vlundaev          #+#    #+#             */
/*   Updated: 2025/12/11 17:15:05 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Count how many times "$?" appears in the string.
*/
static int	count_status_markers(const char *value)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (value[i] != '\0')
	{
		if (value[i] == '$' && value[i + 1] == '?')
			count++;
		i++;
	}
	return (count);
}

/*
** Copy status_str into dst, advancing *j.
*/
static void	copy_status_str(char *dst, int *j, const char *status_str)
{
	int	k;

	k = 0;
	while (status_str[k] != '\0')
	{
		dst[*j] = status_str[k];
		(*j)++;
		k++;
	}
}

/*
** Allocate a new string where all "$?" are replaced
** by status_str. 'count' is the number of "$?" in src.
*/
static char	*replace_status_markers(
	const char *src, const char *status_str, int count)
{
	int		src_len;
	int		status_len;
	char	*dst;
	int		i;
	int		j;

	src_len = ft_strlen(src);
	status_len = ft_strlen(status_str);
	dst = (char *)malloc(src_len + count * (status_len - 2) + 1);
	if (!dst)
		return (NULL);
	i = 0;
	j = 0;
	while (src[i] != '\0')
	{
		if (src[i] == '$' && src[i + 1] == '?')
		{
			copy_status_str(dst, &j, status_str);
			i += 2;
		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
	return (dst);
}

/*
** Expand "$?" inside a single token value.
** Returns a newly allocated string, or NULL on error.
*/
static char	*expand_status_in_value(const char *value, int last_exit_status)
{
	char	*status_str;
	int		status_count;
	char	*expanded_value;

	status_str = ft_itoa(last_exit_status);
	if (!status_str)
		return (NULL);
	status_count = count_status_markers(value);
	if (status_count == 0)
	{
		free(status_str);
		return (ft_strdup(value)); // protect??????
	}
	expanded_value = replace_status_markers(value, status_str, status_count);
	free(status_str);
	return (expanded_value);
}

/*
** Expand all tokens before parsing.
** "$?" and VAR in WORD tokens.
*/
int	expand_tokens(t_token *list, char **envp, int last_exit_status)
{
	char	*tmp1;
	char	*tmp2;

	while (list)
	{
		if (list->type == TOKEN_WORD)
		{
			tmp1 = expand_status_in_value(list->value, last_exit_status);
			if (!tmp1)
				return (1);
			tmp2 = expand_variables_in_value(tmp1, envp);
			free(tmp1);
			if (!tmp2)
				return (1);
			free(list->value);
			list->value = tmp2;
		}
		list = list->next;
	}
	return (0);
}
