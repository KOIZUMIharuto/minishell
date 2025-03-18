/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkoizumi <hkoizumi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:03:59 by hkoizumi          #+#    #+#             */
/*   Updated: 2025/03/17 12:21:21 by hkoizumi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ul_to_hex(unsigned long ul, char *num_list);

char	*ft_uitoa(unsigned int ui)
{
	char	*a_1;
	char	*a_2;
	char	*a;

	a_1 = ft_itoa((int)(ui / 10));
	if (!a_1)
		return (NULL);
	a_2 = ft_itoa((int)(ui % 10));
	if (!a_2)
	{
		free (a_1);
		return (NULL);
	}
	if (ui < 10)
	{
		free(a_1);
		return (a_2);
	}
	a = ft_strjoin(a_1, a_2);
	free(a_1);
	free(a_2);
	return (a);
}

static char	*ul_to_hex(unsigned long ul, char *num_list)
{
	unsigned long	ul_tmp;
	int				digit_count;
	char			*hex;

	ul_tmp = ul;
	digit_count = 1;
	while (ul_tmp >= 16)
	{
		ul_tmp /= 16;
		digit_count++;
	}
	hex = (char *)malloc ((digit_count + 1) * sizeof(char));
	if (!hex)
		return (NULL);
	hex[digit_count] = '\0';
	while (--digit_count >= 0)
	{
		hex[digit_count] = num_list[ul % 16];
		ul /= 16;
	}
	return (hex);
}

char	*arg_to_hex(va_list *arg, char sign)
{
	unsigned long	ul;
	char			*hex;
	char			*address;

	if (sign == 'p')
		ul = (unsigned long)va_arg(*arg, void *);
	else
		ul = (unsigned long)((unsigned int)va_arg(*arg, int));
	if (sign == 'p' || sign == 'x')
		hex = ul_to_hex(ul, "0123456789abcdef");
	else
		hex = ul_to_hex(ul, "0123456789ABCDEF");
	if (!hex)
		return (NULL);
	if (sign == 'x' || sign == 'X')
		return (hex);
	address = ft_strjoin("0x", hex);
	if (hex)
		free(hex);
	return (address);
}

char	*arg_to_str(va_list *arg)
{
	char	*input_str;
	char	*output_str;

	input_str = va_arg(*arg, char *);
	output_str = NULL;
	if (!input_str)
		output_str = ft_strdup("(null)");
	else
		output_str = ft_strdup(input_str);
	return (output_str);
}

char	*c_to_str(va_list *arg, char sign)
{
	char	*str;

	str = (char *)malloc (2 * sizeof(char));
	if (!str)
		return (NULL);
	if (sign == 'c')
		str[0] = va_arg(*arg, int);
	else if (sign == '%')
		str[0] = '%';
	str[1] = '\0';
	return (str);
}
