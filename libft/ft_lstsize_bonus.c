/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 23:34:07 by syonekur          #+#    #+#             */
/*   Updated: 2023/09/30 13:54:55 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return (i);
}

// int	main(void)
// {
// 	t_list	*head;
// 	t_list	*tmp;

// 	head = ft_lstnew("Three");
// 	ft_lstadd_front(&head, ft_lstnew("Two"));
// 	ft_lstadd_front(&head, ft_lstnew("One"));
// 	printf("List size: %d\n", ft_lstsize(head));
// 	while (head)
// 	{
// 		tmp = head;
// 		head = head->next;
// 		free(tmp);
// 	}
// 	return (0);
// }
