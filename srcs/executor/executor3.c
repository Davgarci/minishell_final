/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:58:38 by davgarci          #+#    #+#             */
/*   Updated: 2023/03/25 17:14:35 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pc_save_word2(int *quotes, char *quote, char s)
{
	if (*quotes && s == *quote)
	{
		*quote = 0;
		*quotes = 0;
	}
	else if ((s == '\"' || s == '\'') && !(*quotes))
	{
		*quote = s;
		*quotes = 1;
	}
}

int	pc_save_word(char *str, char *s, char c, int i)
{
	int		j;
	int		quotes;
	char	quote;

	quotes = 0;
	quote = 0;
	j = 0;
	while (s[i] == c)
		i++;
	while (s[i] && ((s[i] != c && !quotes) || quotes))
	{
		pc_save_word2(&quotes, &quote, s[i]);
		str[j] = s[i];
		j++;
		i++;
	}
	str[j] = '\0';
	return (i);
}

char	**ft_split_pc(char *s, char c)
{
	int		j;
	char	**tabla;
	int		i;
	int		max;

	if (s == NULL)
		return (NULL);
	i = pc_countwords(s, c);
	tabla = (char **)malloc(sizeof(char *) * (i + 1));
	if (!tabla)
		return (NULL);
	tabla[i] = NULL;
	max = i;
	i = 0;
	j = 0;
	while (j < max)
	{
		tabla[j] = malloc(sizeof(char) * (pc_size_nextword(s, c, i) + 1));
		if (!tabla[j])
			return (NULL);
		i = pc_save_word(tabla[j], s, c, i);
		j++;
	}
	return (tabla);
}

t_pipe	*ft_lstlast_pipe(t_pipe *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back_pipe(t_pipe **alst, t_pipe *newitem)
{
	t_pipe	*item;

	if (!*alst && newitem)
		*alst = newitem;
	else if (*alst && newitem)
	{
		item = *alst;
		while (item->next)
			item = item->next;
		item->next = newitem;
	}
}
