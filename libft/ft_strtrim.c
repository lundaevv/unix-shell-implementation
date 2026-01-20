/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:35:52 by gperedny          #+#    #+#             */
/*   Updated: 2025/05/02 14:55:56 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_set_compare(char const *set, char s)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == s)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	end;
	size_t	start;
	size_t	i;
	char	*str;

	end = ft_strlen(s1);
	start = 0;
	if (end == 0)
		return (ft_strdup(""));
	while (s1[start] && ft_set_compare(set, s1[start]))
		start++;
	while (s1[end - 1] && end > start && ft_set_compare(set, s1[end - 1]))
		end--;
	i = 0;
	str = ft_calloc((end - start + 1), sizeof(char));
	if (!str)
		return (NULL);
	while (i < (end - start))
	{
		str[i] = s1[i + start];
		i++;
	}
	str[i] = '\0';
	return (str);
}
