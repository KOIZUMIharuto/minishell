/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 19:15:37 by syonekur          #+#    #+#             */
/*   Updated: 2023/10/10 19:59:17 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*tmp;

	if (!lst || !*lst || !del)
		return ;
	current = *lst;
	while (current)
	{
		tmp = current->next;
		del(current->content);
		free(current);
		current = tmp;
	}
	*lst = NULL;
}

// void	de_content(void *content)
// {
// 	free(content);
// }

// int	main(void)
// {
// 	t_list	*head;

// 	// Creating a list with three integer nodes
// 	head = ft_lstnew(malloc(sizeof(int)));
// 	*((int *)head->content) = 42;
// 	head->next = ft_lstnew(malloc(sizeof(int)));
// 	*((int *)head->next->content) = 21;
// 	head->next->next = ft_lstnew(malloc(sizeof(int)));
// 	*((int *)head->next->next->content) = 84;
// 	ft_lstclear(&head, de_content);
// 	// If correctly freed, head should be NULL
// 	if (!head)
// 	{
// 		printf("List cleared successfully!\n");
// 	}
// 	else
// 	{
// 		printf("List not cleared!\n");
// 	}
// 	return (0);
// }
