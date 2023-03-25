/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:58:08 by davgarci          #+#    #+#             */
/*   Updated: 2023/03/25 17:03:11 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe	*new_pipe(char *command, int have_pipe, t_pipe **pipes)
{
	t_pipe	*new;

	new = malloc(sizeof(t_pipe));
	if (!new)
		return (NULL);
	new->pipe_parse = ft_split(command, ' ');
	new->pipe = have_pipe;
	new->read = g_c.read;
	new->previous = ft_lstlast_pipe(pipes[0]);
	new->next = 0;
	return (new);
}

int	ft_redirection_pipe2(char **command)
{
	char	**ret;
	char	*trimmed;

	ret = ft_split_pc(*command, '<');
	trimmed = ft_strtrim(ret[1], " \"\'");
	g_c.redirection_in = open(trimmed, O_RDONLY, 0777);
	if (g_c.redirection_in == -1)
		return (0);
	free(trimmed);
	free(ret);
	g_c.read = 1;
	return (1);
}

int	free_else(char ***ret1, char **trimmed1)
{
	char	**ret_aux;
	char	*trimmed_aux;

	ret_aux = *ret1;
	trimmed_aux = *trimmed1;
	ft_free_matrix(ret_aux);
	free(trimmed_aux);
	return (0);
}

int	ft_redirection_pipe(char **command)
{
	char	**ret1;
	char	*trimmed1;

	ret1 = ft_split_pc(*command, '>');
	trimmed1 = ft_strtrim(ret1[1], " \"\'");
	if (ft_strchr(*command, '<'))
	{
		if (!ft_redirection_pipe2(command))
			return (0);
	}
	else if (ft_strnstr(*command, ">>", ft_strlen(*command))
		&& ret1 && trimmed1)
		g_c.redirection_out = open(trimmed1,
				O_WRONLY | O_APPEND | O_CREAT, 0777);
	else if (ft_strchr(*command, '>') && ret1 && trimmed1)
		g_c.redirection_out = open(trimmed1,
				O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else
		return (free_else(&ret1, &trimmed1));
	ft_free_matrix(ret1);
	free(trimmed1);
	return (1);
}

void	ft_lstclear2(t_pipe **lst)
{
	t_pipe	*tmp;

	while (lst && *lst)
	{
		tmp = (*lst)->next;
		free(lst);
		*lst = tmp;
	}
}
