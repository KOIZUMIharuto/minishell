/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 20:16:36 by syonekur          #+#    #+#             */
/*   Updated: 2023/10/02 11:48:18 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	if (size == 0 || count == 0)
	{
		size = 1;
		count = 1;
	}
	else if (count > SIZE_MAX / size)
		return (NULL);
	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}

// int	main(void)
// {
// 	int	*values;

// 	values = ft_calloc(ALLOCATE_SIZE, sizeof(int));
// 	if (values == NULL)
// 		exit(EXIT_FAILURE);
// 	for (int i = 0; i < ALLOCATE_SIZE; i++)
// 	{
// 		printf("%d", values[i]);
// 	}
// 	printf("\n");
// 	for (int i = 0; i < ALLOCATE_SIZE; i++)
// 	{
// 		values[i] = i;
// 		printf("%d", values[i]);
// 	}
// 	printf("\n");
// 	for (int i = 0; i < ALLOCATE_SIZE; i++)
// 	{
// 		printf("%d", values[i]);
// 	}
// 	printf("\n");
// 	free(values);
// }
