/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:24:26 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/17 12:26:30 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_arg(char sign, va_list *arg);

int	ft_printf(const	char *str, ...)
{
	va_list	arguments;
	int		len;
	int		is_print_success;

	va_start(arguments, str);
	len = 0;
	while (*str)
	{
		if (*str == '%')
			is_print_success = print_arg(*(str++ + 1), &arguments);
		else
			is_print_success = write (1, (char *)str, 1);
		if (is_print_success < 0)
			return (-1);
		len += is_print_success;
		str++;
	}
	va_end(arguments);
	return (len);
}

static int	print_arg(char sign, va_list *arg)
{
	char	*str_for_write;
	int		len;

	str_for_write = NULL;
	if (sign == 'c' || sign == '%')
		str_for_write = c_to_str(arg, sign);
	else if (sign == 's')
		str_for_write = arg_to_str(arg);
	else if (sign == 'p' || sign == 'x' || sign == 'X')
		str_for_write = arg_to_hex(arg, sign);
	else if (sign == 'd' || sign == 'i')
		str_for_write = ft_itoa(va_arg(*arg, int));
	else if (sign == 'u')
		str_for_write = ft_uitoa(va_arg(*arg, unsigned int));
	if (!str_for_write)
		return (-1);
	len = ft_strlen(str_for_write);
	len = write (1, str_for_write, len);
	free(str_for_write);
	return (len);
}
