/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:09:15 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:08:32 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char c, char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char *s1, char *set)
{
	char	*start;
	char	*end;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = s1;
	end = s1 + ft_strlen(s1) - 1;
	while (*start && is_set(*start, set))
	{
		start++;
	}
	while (end > start && is_set(*end, set))
		end--;
	return (ft_substr(start, 0, end - start + 1));
}

// int	main(void)
// {
// 	char 	*s1;
// 	char 	*set;
// 	char		*trimmed;

// 	s1 = "yzxHello Worldxxz";
// 	set = "xyz";
// 	trimmed = ft_strtrim(s1, set);
// 	printf("%s\n", trimmed);
// 	free(trimmed);
// 	return (0);
// }
