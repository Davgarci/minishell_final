/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 16:55:15 by psegura-          #+#    #+#             */
/*   Updated: 2023/03/25 04:15:23 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_exit(const char *command_buf)
{
	return (!ft_strcmp(command_buf, "exit")
		|| !ft_strcmp(command_buf, "quit") || !ft_strcmp(command_buf, "e")
		|| !ft_strcmp(command_buf, "q"));
}

void	expand_while(void)
{
	g_c.tok_count = 0;
	while (g_c.tokens[g_c.tok_count])
	{
		g_c.tokens[g_c.tok_count] = expan_token2(g_c.tokens[g_c.tok_count],
				g_c.env);
		if (token_is_symbol(g_c.tokens[g_c.tok_count]))
			g_c.pipas++;
		g_c.tok_count++;
	}
	g_c.tokens[g_c.tok_count] = NULL;
}

int	ft_continue(char *command_buf)
{
	if (!check_quotes(command_buf))
	{
		printf("The quotes are not closed!\n");
		return (1);
	}
	g_c.tokens = malloc(sizeof(char *) * (count_tokens(command_buf) + 1));
	store_tokens(command_buf, g_c.tokens);
	if (double_symbols(g_c.tokens) == 1)
	{
		printf("Invalid symbols!\n");
		return (1);
	}
	return (0);
}

void	next_funcionality(void)
{
	expand_while();
	pipas_handler();
}

void	readline_create(void)
{
	char	*command_buf;
	char	*line;

	while (1)
	{
		line = readline(PROMPT);
		command_buf = ft_strtrim(line, " ");
		free(line);
		if (!command_buf)
			exit(0);
		if (!*command_buf)
			continue ;
		if (read_exit(command_buf))
			break ;
		if (ft_strlen(command_buf) > 0)
			add_history(command_buf);
		if (ft_continue(command_buf))
			continue ;
		g_c.tokens = tokens_to_pipas(g_c.tokens);
		next_funcionality();
		ft_free_matrix(g_c.tokens);
		free(command_buf);
	}
}
