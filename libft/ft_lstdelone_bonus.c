/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 11:40:15 by syonekur          #+#    #+#             */
/*   Updated: 2023/10/10 22:58:46 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	if (lst)
	{
		del(lst->content);
		free(lst);
	}
}

// void	del_content(void *content)
// {
// 	free(content);
// }

// int	main(void)
// {
// 	t_list	*tmp;
// 	t_list	*head;
// 	t_list	*current;

// 	head = ft_lstnew(strdup("One"));
// 	head->next = ft_lstnew(strdup("Two"));
// 	head->next->next = ft_lstnew(strdup("Three"));
// 	printf("Before deletation\n");
// 	current = head;
// 	while (current)
// 	{
// 		printf("%s\n", (char *)current->content);
// 		current = current->next;
// 	}
// 	ft_lstdelone(head->next, del_content);
// 	head->next = head->next->next;
// 	printf("After deletion\n");
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
// 		del_content(tmp->content);
// 		free(tmp);
// 	}
// 	return (0);
// }
