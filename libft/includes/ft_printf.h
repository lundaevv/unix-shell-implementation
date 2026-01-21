/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:23:42 by vlundaev          #+#    #+#             */
/*   Updated: 2026/01/21 15:19:19 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>

int	ft_printf(char const *str, ...);
int	printf_putchar(char ch);
int	printf_putnbr(long nb);
int	printf_putnbr_hexa(unsigned long nb, char up);
int	printf_putptr(void *ptr);
int	printf_putstr(char *str);

#endif
