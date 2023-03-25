/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:58:32 by davgarci          #+#    #+#             */
/*   Updated: 2023/03/25 17:01:38 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipes(void)
{
	int			i;
	t_pipe		*new_elem;
	char		**ret_split;

	i = 0;
	while (g_c.tokens[i])
	{
		ft_redirection_pipe(&g_c.tokens[i]);
		ret_split = ft_split(g_c.tokens[i], '>');
		free(g_c.tokens[i]);
		g_c.tokens[i] = ft_strdup(ret_split[0]);
		ft_free_matrix(ret_split);
		ret_split = ft_split(g_c.tokens[i], '<');
		free(g_c.tokens[i]);
		g_c.tokens[i] = ft_strdup(ret_split[0]);
		ft_free_matrix(ret_split);
		if (g_c.tokens[i + 1] == NULL)
			new_elem = new_pipe(g_c.tokens[i], 0, &g_c.pipe_list);
		else
			new_elem = new_pipe(g_c.tokens[i], 1, &g_c.pipe_list);
		ft_lstadd_back_pipe(&g_c.pipe_list, new_elem);
		i++;
	}
	return (1);
}

int	ft_filedescriptors(t_pipe *list_cmnd)
{
	if (g_c.redirection_out
		&& !list_cmnd->next && dup2(g_c.redirection_out, STDOUT_FILENO) < 0)
		return (0);
	if (g_c.redirection_in
		&& list_cmnd->read && dup2(g_c.redirection_in, STDIN_FILENO) < 0)
		return (0);
	if (list_cmnd->pipe == 1
		&& dup2(list_cmnd->fd[1], STDOUT_FILENO) < 0)
		return (0);
	if (list_cmnd->previous && list_cmnd->previous->pipe == 1
		&& dup2(list_cmnd->previous->fd[0], STDIN_FILENO) < 0)
		return (0);
	return (1);
}

int	ft_child_process(t_pipe *list_cmnd)
{
	if (!ft_filedescriptors(list_cmnd))
		return (0);
	g_c.cmd_exec_parsed = list_cmnd->pipe_parse;
	ft_exec(list_cmnd->pipe_parse);
	exit(1);
	return (1);
}

void	ft_close_fds(t_pipe *list_cmnd, int pipe_open)
{
	if (pipe_open)
	{
		close(list_cmnd->fd[1]);
		if (!list_cmnd->next || list_cmnd->pipe == 0)
			close(list_cmnd->fd[0]);
	}
	if (list_cmnd->previous && list_cmnd->previous->pipe == 1)
		close(list_cmnd->previous->fd[0]);
	if (g_c.redirection_out && !list_cmnd->next)
	{
		close(g_c.redirection_out);
		g_c.redirection_out = 0;
	}
	if (g_c.redirection_in && g_c.read)
	{
		close(g_c.redirection_in);
		g_c.redirection_in = 0;
		if (!g_c.redirection_out)
			g_c.read = 0;
		else
			g_c.read = 2;
	}
}

void	ft_pipe2(t_pipe *list_cmnd, int *pipe_open, int *ret, pid_t	pid)
{
	int		status;

	if (pid == 0)
		ft_child_process(list_cmnd);
	else
	{
		waitpid(pid, &status, 0);
		if (status != 0)
			status = 127;
		g_c.dolar_q = status;
		if ((g_c.read != 1) && g_c.redirection_out
			&& !list_cmnd->next && close(g_c.redirection_out))
		{
			g_c.read = 0;
			g_c.redirection_out = 0;
		}
		ft_close_fds(list_cmnd, *pipe_open);
		if (WIFEXITED(status))
			*ret = WEXITSTATUS(status);
		g_c.forking = 0;
	}
}
