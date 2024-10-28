/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 19:46:49 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:03:57 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*set_nbr(int c, int size, char *ptr, int sign)
{
	if (sign == 1)
	{
		ptr[0] = '-';
	}
	ptr[size] = '\0';
	while (--size >= sign)
	{
		ptr[size] = (c % 10) + '0';
		c /= 10;
	}
	return (ptr);
}

static int	cnt_size(int c)
{
	int	size;

	size = 1;
	while (c >= 10)
	{
		size++;
		c /= 10;
	}
	return (size);
}

char	*ft_itoa(int c)
{
	int		sign;
	int		size;
	char	*ptr;
	char	*s1;

	if (c == -2147483648)
	{
		s1 = "-2147483648";
		return (ft_strdup(s1));
	}
	sign = 0;
	if (c < 0)
	{
		sign = 1;
		c *= -1;
	}
	size = cnt_size(c);
	ptr = (char *)malloc(size + sign + 1);
	if (!ptr)
		return (NULL);
	set_nbr(c, size + sign, ptr, sign);
	return (ptr);
}

// int	main(void)
// {
// 	int i;
// 	char *s;

// 	i = -2147483647;
// 	s = ft_itoa(i);
// 	printf("ans:%s\n", s);
// 	if (s != NULL)
// 		free(s);
// 	return (0);
// }
