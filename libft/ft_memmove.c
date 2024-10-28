/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 20:57:34 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:04:59 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, void *src, size_t len)
{
	char	*dest_ptr;
	char	*src_ptr;

	if (dest == NULL && src == NULL)
		return (NULL);
	dest_ptr = (char *)dest;
	src_ptr = (char *)src;
	if (dest_ptr <= src_ptr)
		ft_memcpy(dest_ptr, src_ptr, len);
	else
	{
		while (len--)
		{
			dest_ptr[len] = src_ptr[len];
		}
	}
	return (dest);
}

// int	main(void)
// {
// 	char buf[] = "ABCDEFG";
// 	ft_memmove(buf + 3, buf, 3);
// 	printf("Result:%s\n", buf);
// 	return (0);
// }
