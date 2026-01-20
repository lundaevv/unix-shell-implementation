/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gperedny <gperedny@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:19:30 by gperedny          #+#    #+#             */
/*   Updated: 2025/05/02 14:41:45 by gperedny         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(int n)
{
	int		count;
	long	j;

	count = 0;
	j = (long)n;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++;
		j *= -1;
	}
	while (j != 0)
	{
		j = j / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	long	j;

	j = (long)n;
	str = malloc(sizeof(char) * (ft_count(n) + 1));
	if (!str)
		return (NULL);
	i = ft_count(n);
	str[i--] = '\0';
	if (n < 0)
	{
		str[0] = '-';
		j *= -1;
	}
	if (n == 0)
		str[0] = '0';
	while (j > 0)
	{
		str[i--] = ((j % 10) + '0');
		j = j / 10;
	}
	return (str);
}
