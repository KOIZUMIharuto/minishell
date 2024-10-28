/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 21:42:41 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/02 15:21:05 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup( char *s1)
{
	int		i;
	char	*ptr;

	ptr = malloc(ft_strlen(s1) + 1);
	if (!ptr)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

// int	main(void)
// {
// 	char	*s2;

// 	s2 = ft_strdup(NULL);
// 	if (!s2)
// 	{
// 		printf("malloc faile");
// 		return (1);
// 	}
// 	printf("Rstult_s2:%s\n", s2);
// 	free(s2);
// 	return (0);
// }
