/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 23:13:41 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:15:20 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(char *haystack, char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (!*needle)
		return ((char *)haystack);
	if (len == 0)
		return (NULL);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		if (haystack[i] == needle[j])
		{
			while (needle[j] && i + j < len && haystack[i + j] == needle[j])
			{
				j++;
			}
			if (needle[j] == '\0')
				return ((char *)&haystack[i]);
		}
		i++;
	}
	return (NULL);
}

// int	main(void)
// {
// 	char	*s1;
// 	char	*s2;
// 	size_t	max;

// 	char	*p;
// 	char test[20] = "abcdefghijk";
// 	p = strnstr(test, "fgh", -1);
// 	printf("Result:%s\n", p);
// 	printf("%s\n", ft_strnstr(NULL, "fake", 0));
// 	s1 = "oh no not the empty string !";
// 	s2 = "";
// 	max = 0;
// 	printf("%s\n", strnstr(s1, s2, max));
// 	printf("%s\n", ft_strnstr(s1, s2, max));
// }
