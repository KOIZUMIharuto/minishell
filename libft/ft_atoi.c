/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:54:21 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/14 20:22:35 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	check_overflow_nbr(char *s, int sign)
{
	long	ans;

	ans = 0;
	while (*s >= '0' && *s <= '9')
	{
		ans = ans * 10 + (*s - '0');
		s++;
	}
	return (ans * sign);
}

long	ft_atoi(char *str)
{
	int		sign;
	long	ans;

	sign = 1;
	while (*str == '\t' || *str == '\n' || *str == '\v' || *str == '\f'
		|| *str == '\r' || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	ans = check_overflow_nbr(str, sign);
	return (ans);
}

// int	main(void)
// {
// 	char	*s;

// 	s = "-9223372036854775808";
// 	printf("org:%d\n", ft_atoi(s));
// 	printf("imi:%d\n", atoi(s));
// 	// printf("ans:%ld\n", LONG_MAX);
// 	// printf("ans:%ld\n", LONG_MIN);
// 	return (0);
// }
