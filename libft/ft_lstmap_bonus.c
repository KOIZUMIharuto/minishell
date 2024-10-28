/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 20:01:27 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/04 21:30:19 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_node;

	new_lst = NULL;
	if (!lst || !f || !del)
		return (NULL);
	while (lst)
	{
		new_node = ft_lstnew(f(lst->content));
		if (!new_node)
		{
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new_node);
		lst = lst->next;
	}
	return (new_lst);
}

// // 1. value*2
// void	*double_content(void *content)
// {
// 	int	*value;
// 	int	*doubled_value;

// 	value = (int *)content;
// 	doubled_value = malloc(sizeof(int));
// 	if (!doubled_value)
// 		return (NULL);
// 	*doubled_value = *value * 2;
// 	return (doubled_value);
// }

// // 2. del_value
// void	del_content(void *content)
// {
// 	free(content);
// }

// // 3. print_lst
// void	print_list(t_list *lst)
// {
// 	while (lst)
// 	{
// 		printf("%d -> ", *(int *)lst->content);
// 		lst = lst->next;
// 	}
// 	printf("NULL\n");
// }

// int	main(void)
// {
// 	t_list	*lst;
// 	t_list	*mapped_lst;

// 	int nbr1, nbr2, nbr3;
// 	nbr1 = 5, nbr2 = 10, nbr3 = 15;
// 	// Create a linked list: 5 -> 10 -> 15
// 	lst = ft_lstnew(&nbr1);
// 	ft_lstadd_back(&lst, ft_lstnew(&nbr2));
// 	ft_lstadd_back(&lst, ft_lstnew(&nbr3));
// 	printf("Original list:\n");
// 	print_list(lst);
// 	// Apply ft_lstmap to the list
// 	mapped_lst = ft_lstmap(lst, double_content, del_content);
// 	printf("Mapped list (values doubled):\n");
// 	print_list(mapped_lst);
// 	// Free the mapped list
// 	ft_lstclear(&mapped_lst, del_content);
// 	return (0);
// }
