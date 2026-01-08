/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:26:51 by gperedny          #+#    #+#             */
/*   Updated: 2025/05/02 14:46:39 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;
	char			sign;

	i = 0;
	sign = c;
	if (s == NULL)
		return (NULL);
	while (s[i] != '\0' && s[i] != sign)
		i++;
	if (s[i] == sign)
		return ((char *)(s + i));
	return (NULL);
}
