/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syonekur <syonekur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 13:51:56 by syonekur          #+#    #+#             */
/*   Updated: 2024/07/03 21:05:49 by syonekur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	cnt_words(char *s, char sep)
{
	int	cnt;

	cnt = 0;
	while (*s)
	{
		while (*s && (*s == sep))
			s++;
		if (*s)
		{
			cnt++;
			while (*s && (*s != sep))
				s++;
		}
	}
	return (cnt);
}

static char	*free_words(char **words, int i)
{
	while (i >= 0)
	{
		free(words[i]);
		i--;
	}
	free(words);
	return (0);
}

static char	*next_word(char *s, char sep)
{
	while (*s && *s == sep)
		s++;
	return (s);
}

static size_t	word_len(char *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

char	**ft_split(char *s, char c)
{
	char	**result;
	int		i;
	int		word_cnt;

	if (s == NULL)
		return (NULL);
	word_cnt = cnt_words(s, c);
	result = malloc((word_cnt + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (i < word_cnt)
	{
		s = next_word(s, c);
		result[i] = ft_substr(s, 0, word_len(s, c));
		if (!result[i])
		{
			free_words(result, i - 1);
			return (NULL);
		}
		s += word_len(s, c);
		i++;
	}
	result[i] = NULL;
	return (result);
}

// int	main(void)
// {
// 	char	**tab;

// 	// char 	*s1;
// 	// char		set;
// 	// char		**splited;
// 	// s1 = "Hexllo Worlxd,Hxello evxeryoxne!";
// 	// set = 'x';
// 	tab = ft_split("  tripouille  42  ", ' ');
// 	// tab = ft_split(s1, set);
// 	// printf("%d\n", cnt_words(s1, set));
// 	for (int i = 0; tab[i]; i++)
// 	{
// 		printf("tab:%s\n", tab[i]);
// 		free(tab[i]);
// 	}
// 	free(tab);
// 	return (0);
// }
