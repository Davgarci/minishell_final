/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:58:46 by davgarci          #+#    #+#             */
/*   Updated: 2023/03/25 17:14:26 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_t_pipe_list(t_pipe **head)
{
	t_pipe	*current;

	if (*head == NULL)
		return ;
	if ((*head)->next != *head)
		clear_t_pipe_list(&((*head)->next));
	current = *head;
	*head = (*head)->next;
	ft_free_matrix(current->pipe_parse);
	if (current->fd[0] != -1)
		close(current->fd[0]);
	if (current->fd[1] != -1)
		close(current->fd[1]);
	free(current);
}

void	initialize(int *quotes, int *i, char *quote, int *words)
{
	*i = 0;
	*quote = 0;
	*quotes = 0;
	*words = 0;
}

void	pc_countwords_2(char s, int *quotes, char *quote)
{
	if ((s == '\"' || s == '\'') && !(*quotes))
	{
		*quote = s;
		*quotes = 1;
	}
	else if (*quotes && s == *quote)
	{
		*quote = 0;
		*quotes = 0;
	}
}

int	pc_countwords(char *s, char c)
{
	int		i;
	int		words;
	int		quotes;
	char	quote;

	initialize(&quotes, &i, &quote, &words);
	while (s[i])
	{
		if (s[i] == c)
		{
			i++;
			continue ;
		}
		words++;
		while (s[i] && ((s[i] != c && !quotes) || quotes))
		{
			pc_countwords_2(s[i], &quotes, &quote);
			i++;
		}
	}
	return (words);
}

int	pc_size_nextword(char *s, char c, int i)
{
	int		counter;
	int		quotes;
	char	quote;

	quotes = 0;
	quote = 0;
	while (s[i] == c && s[i])
		i++;
	counter = 0;
	while (s[i] && ((s[i] != c && !quotes) || quotes))
	{
		if (quotes && s[i] == quote)
		{
			quote = 0;
			quotes = 0;
		}
		else if ((s[i] == '\"' || s[i] == '\'') && !quotes)
		{
			quote = s[i];
			quotes = 1;
		}
		i++;
		counter++;
	}
	return (counter);
}
