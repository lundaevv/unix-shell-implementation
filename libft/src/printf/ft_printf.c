/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:08:14 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/21 15:19:50 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_conversion(const char type, va_list vargs)
{
	if (type == 'c')
		return (printf_putchar(va_arg(vargs, int)));
	else if (type == 'u')
		return (printf_putnbr(va_arg(vargs, unsigned int)));
	else if ((type == 'i') || (type == 'd'))
		return (printf_putnbr(va_arg(vargs, int)));
	else if (type == 's')
		return (printf_putstr(va_arg(vargs, char *)));
	else if ((type == 'x') || (type == 'X'))
		return (printf_putnbr_hexa(va_arg(vargs, unsigned int), type));
	else if (type == 'p')
		return (printf_putptr(va_arg(vargs, void *)));
	else if (type == '%')
		return (printf_putchar('%'));
	return (-1);
}

int	ft_printf(const char *str, ...)
{
	va_list	vargs;
	int		len;
	int		check;

	va_start(vargs, str);
	len = 0;
	while (*str)
	{
		if (*str == '%')
		{
			check = ft_conversion(*(++str), vargs);
			if (check == -1)
				return (-1);
			len += check;
		}
		else
		{
			if (write(1, str, 1) == -1)
				return (-1);
			len++;
		}
		str++;
	}
	va_end(vargs);
	return (len);
}
