/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 23:12:13 by syonekur          #+#    #+#             */
/*   Updated: 2023/10/02 19:32:54 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!lst)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		ft_lstlast(*lst)->next = new;
	}
}

// int	main(void)
// {
// 	t_list	*head;
// 	t_list	*current;
// 	t_list	*tmp;

// 	head = NULL;
// 	ft_lstadd_back(&head, ft_lstnew("One"));
// 	ft_lstadd_back(&head, ft_lstnew("Two"));
// 	ft_lstadd_back(&head, ft_lstnew("Three"));

// 	current = head;
// 	while (current)
// 	{
// 		printf("%s\n", (char *)current->content);
// 		current = current->next;
// 	}
// 	while (head)
// 	{
// 		tmp = head;
// 		head = head->next;
// 		free(tmp);
// 	}
// 	return (0);
// }
