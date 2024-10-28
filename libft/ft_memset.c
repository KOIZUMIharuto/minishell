/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 19:51:09 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:05:06 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;
	char	*ptr;

	i = 0;
	ptr = (char *)b;
	while (i < len)
	{
		ptr[i] = (char)c;
		i++;
	}
	return (b);
}

// int	main(void)
// {
// 	char s1[] = "abcdefg";

// 	printf("%s\n", (char *)ft_memset(s1, 'x', (size_t)5));
// 	return (0);
// }