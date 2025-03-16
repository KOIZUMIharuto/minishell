/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shiori <shiori@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:58:47 by shiori            #+#    #+#             */
/*   Updated: 2025/03/16 14:25:56 by shiori           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strjoin3(char *s1, char *s2, char *s3)
{
    size_t len1 = ft_strlen(s1);
    size_t len2 = ft_strlen(s2);
    size_t len3 = ft_strlen(s3);
    char *result = malloc(len1 + len2 + len3 + 1);
    
    if (!result)
        return NULL;
    
    ft_strlcpy(result, s1, len1 + 1);
    ft_strlcat(result, s2, len1 + len2 + 1);
    ft_strlcat(result, s3, len1 + len2 + len3 + 1);
    
    return result;
}