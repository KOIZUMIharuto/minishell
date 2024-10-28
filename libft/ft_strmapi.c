/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 21:35:15 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:06:34 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char *s, char (*f)(int, char))
{
	int		i;
	char	*ptr;

	if (!s || !f)
		return (NULL);
	ptr = ft_strdup(s);
	if (!ptr)
		return (NULL);
	i = 0;
	while (s[i])
	{
		ptr[i] = f(i, s[i]);
		i++;
	}
	return (ptr);
}

// char	aaaaa( int i, char c)
// {
// 	c += i;
// 	return (c);
// }

// char	upper( int i, char c)
// {
// 	(void)i;
// 	if (c >= 'a' && c <= 'z')
// 		c -= ('a' - 'A');
// 	return (c);
// }

// int	main(int argc, char **argv)
// {
// 	char	*str;

// 		char s1[] = "aaaaaaaaaaaaaaaa";
// 	(void)argc;
// 	if (argv[1][0] == '1')
// 		str = ft_strmapi(s1, &aaaaa);
// 	if (argv[1][0] == '2')
// 		str = ft_strmapi(s1, &upper);
// 	printf("%s\n", str);
// 	free(str);
// }
