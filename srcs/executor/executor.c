/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 21:41:24 by davgarci          #+#    #+#             */
/*   Updated: 2023/03/25 17:01:20 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pipe(t_pipe *list_cmnd, int *pipe_open, int *ret)
{
	pid_t	pid;

	if ((list_cmnd->pipe == 1 || (list_cmnd->previous
				&& list_cmnd->previous->pipe == 1)))
	{
		*pipe_open = 1;
		if (pipe(list_cmnd->fd))
			return (0);
	}
	g_c.forking = 1;
	if (list_cmnd->pipe == 1 || builtins(list_cmnd->pipe_parse) == 42)
	{
		pid = fork();
		if (pid < 0)
			return (0);
		ft_pipe2(list_cmnd, pipe_open, ret, pid);
	}
	return (1);
}

int	is_builtin(char *str)
{
	if ((!ft_strcmp(str, "pwd") || !ft_strcmp(str, "env")
			|| !ft_strcmp(str, "cd") || !ft_strcmp(str, "echo")
			|| !ft_strcmp(str, "export") || !ft_strcmp(str, "unset")))
		return (1);
	return (0);
}

int	only_builtins(t_pipe *head)
{
	while (head)
	{
		if (!ft_strlen(head->pipe_parse[0]))
			return (1);
		if (!is_builtin(head->pipe_parse[0]))
			return (0);
		head = head->next;
	}
	return (1);
}

void	free_list(t_pipe **head)
{
	t_pipe	*aux;

	aux = *head;
	if (aux->next == g_c.pipe_list)
	{
		if (ft_strlen(aux->pipe_parse[0]) != 0)
			ft_free_matrix(aux->pipe_parse);
		free(aux);
	}
	else if (only_builtins(aux))
	{
		while (aux && aux->pipe_parse)
		{
			ft_free_matrix(aux->pipe_parse);
			free(aux);
			aux = aux->next;
		}
	}
	else
		clear_t_pipe_list(&aux);
}

void	pipas_handler(void)
{
	int		ret;
	int		pipe_open;
	t_pipe	*head;

	create_pipes();
	head = g_c.pipe_list;
	while (g_c.pipe_list)
	{
		ret = EXIT_FAILURE;
		pipe_open = 0;
		if (!(ft_pipe(g_c.pipe_list, &pipe_open, &ret)))
			return ;
		g_c.pipe_list = g_c.pipe_list->next;
	}
	free_list(&head);
	return ;
}
