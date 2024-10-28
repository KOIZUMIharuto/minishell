/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 20:35:23 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:08:23 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(char *s, int c)
{
	char	*last_pos;

	last_pos = NULL;
	while (*s)
	{
		if (*s == (char)c)
		{
			last_pos = s;
		}
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return ((char *)last_pos);
}

// int	main(void)
// {
// 	char test[20] = "123456321";
// 	char *p, *q;
// 	p = ft_strrchr(test, '3');
// 	q = ft_strrchr(test, 0);
// 	printf("Result_3:%s\n", p);
// 	printf("Result_0:%s\n", q);
// 	return (0);
// }
