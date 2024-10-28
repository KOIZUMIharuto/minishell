/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 23:33:10 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:06:07 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_strlen_s(char *s)
{
	size_t	len;

	if (s == NULL)
		return (0);
	len = 0;
	while (s[len])
	{
		len++;
	}
	return (len);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	pre_len;
	size_t	suf_len;
	char	*ptr;
	char	*org_ptr;

	pre_len = ft_strlen_s(s1);
	suf_len = ft_strlen_s(s2);
	ptr = malloc(pre_len + suf_len + 1);
	if (!ptr)
		return (NULL);
	org_ptr = ptr;
	while (pre_len--)
		*ptr++ = *s1++;
	while (suf_len--)
		*ptr++ = *s2++;
	*ptr = '\0';
	return (org_ptr);
}

// int	main(void)
// {
// 	char	*s1;
// 	char	*s2;
// 	char	*ans;

// 	s1 = "Hello ";
// 	s2 = "World";
// 	ans = ft_strjoin(s1, s2);
// 	printf("%s\n", ans);
// 	free(ans);
// 	return (0);
// }
