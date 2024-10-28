/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 22:22:01 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/14 16:04:28 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char *s, int start, size_t len)
{
	char	*sub;
	size_t	i;
	size_t	s_size;

	if (!s)
		return (NULL);
	s_size = ft_strlen(s);
	if ((size_t)start >= s_size)
		return (ft_strdup(""));
	sub = (char *)malloc((len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

// int	main(void)
// {
// 	char	*s2;

// 		char s1[] = "Hello everyone!!";
// 	s2 = ft_substr(s1, 1, 8);
// 	if (!s2)
// 	{
// 		printf("malloc faile");
// 		return (1);
// 	}
// 	printf("%s\n", s2);
// 	free(s2);
// 	return (0);
// }
