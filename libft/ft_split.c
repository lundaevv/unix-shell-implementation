/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:19:27 by gperedny          #+#    #+#             */
/*   Updated: 2025/08/14 19:56:31 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_count(char const *s, char c)
{
	int	i;
	int	flag;
	int	count;

	i = 0;
	count = 0;
	flag = 0;
	while (s[i])
	{
		if (s[i] != c && flag == 0)
		{
			count++;
			flag = 1;
		}
		else if (s[i] == c)
			flag = 0;
		i++;
	}
	return (count);
}

static char	**malloc_free(char **s, int index)
{
	while (index--)
		free(s[index]);
	free(s);
	return (NULL);
}

static char	*put_word(char **res, char const *s, char c)
{
	int	i;
	int	j;
	int	str_len;

	i = 0;
	j = 0;
	while (s[i])
	{
		str_len = 0;
		if (s[i] != c)
		{
			while (s[i + str_len] != c && s[i + str_len] != '\0')
				str_len++;
			res[j] = malloc((sizeof(char)) * (str_len + 1));
			if (!res[j])
				malloc_free(res, j);
			ft_strlcpy(res[j], s + i, str_len + 1);
			j++;
			i += str_len;
		}
		else
			i++;
	}
	res[j] = NULL;
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	// int		j = 0;

	// j = 0;
	res = malloc(sizeof(char *) * (word_count(s, c) + 1));
	if (!res)
		return (NULL);
	put_word(res, s, c);
	return (res);
}
