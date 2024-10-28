/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 21:23:03 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:06:38 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(char *s1, char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n)
	{
		if ((s1[i] != s2[i] || !s1[i] || !s2[i]))
			return ((char)s1[i] - (char)s2[i]);
		i++;
	}
	return (0);
}

// int	main(void)
// {
// 	printf("%d\n", ft_strncmp(NULL, "hello", 1000));
// 	printf("%d\n", strncmp(NULL, "hello", 1000));
// 	return (0);
// }
