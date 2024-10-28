/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 23:31:51 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:06:16 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;
	size_t	i;

	d_len = 0;
	if (dest)
		d_len = ft_strlen(dest);
	s_len = ft_strlen(src);
	if (d_len >= size)
		return (size + s_len);
	i = 0;
	while (src[i] && d_len + i < size - 1)
	{
		dest[d_len + i] = src[i];
		i++;
	}
	dest[d_len + i] = '\0';
	return (d_len + s_len);
}

// int	main(void)
// {
// char dest[20] = "Hello World!";
// char dest_reset[20] = "Hello World!";
// char dest_emp[20] = "";
// char src[] = "Good Morning";
// char src_emp[] = "";
// Test 1: Basic test
// printf("Result:%s,Return value:%zu\n", dest, ft_strlcat(NULL, src, 0));
// strcpy(dest, dest_reset);
// Test2:
// 	printf("Result:%s,Return value:%zu\n", dest_emp, ft_strlcat(dest_emp,
// src,
// 20));
// 	strcpy(dest_emp, "");
// Test3:
// 	printf("Result:%s,Return value:%zu\n", dest_emp, ft_strlcat(dest_emp,
// src_emp, 20));
// 	strcpy(dest_emp, "");
// // Test4:
// printf("Result:%s,Return value:%zu\n", dest, strlcat(dest, src_emp, 50));
// strcpy(dest, dest_reset);
// Test5:
// 	printf("Result:%s,Return value:%zu\n", dest, ft_strlcat(dest, src, 10));
// 	return (0);
// }
