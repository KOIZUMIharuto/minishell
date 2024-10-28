/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 19:43:44 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/04 21:30:19 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}

// void	increase(void *content)
// {
// 	*(int *)content += 1;
// }

// int	main(void)
// {
// 	t_list	*lst;
// 	t_list	*tmp;

// 	int nbr1, nbr2, nbr3;
// 	nbr1 = 5, nbr2 = 10, nbr3 = 15;
// 	// Create a linked list: 5 -> 10 -> 15
// 	lst = ft_lstnew(&nbr1);
// 	lst->next = ft_lstnew(&nbr2);
// 	lst->next->next = ft_lstnew(&nbr3);
// 	// Apply 'increase' function on each node's content using ft_lstiter
// 	ft_lstiter(lst, increase);
// 	// Print the modified content of the list
// 	while (lst)
// 	{
// 		printf("%d -> ", *(int *)lst->content);
// 		lst = lst->next;
// 	}
// 	printf("NULL\n");
// 	while (lst)
// 	{
// 		tmp = lst;
// 		lst = lst->next;
// 		free(tmp);
// 	}
// 	return (0);
// }
