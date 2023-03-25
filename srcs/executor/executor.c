/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 21:41:24 by davgarci          #+#    #+#             */
/*   Updated: 2023/03/25 04:08:44 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_t_pipe_list2(t_pipe **head)
{
	t_pipe	*current;

	if (*head == NULL)
		return ;
	if ((*head)->next != *head)
		if (ft_strlen((*head)->next->pipe_parse[0]))
			clear_t_pipe_list2(&((*head)->next));
	current = *head;
	*head = (*head)->next;
	ft_free_matrix(current->pipe_parse);
	if (current->fd[0] != -1)
		close(current->fd[0]);
	if (current->fd[1] != -1)
		close(current->fd[1]);
	free(current);
}

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

static void	initialize(int *quotes, int *i, char *quote, int *words)
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

static int	pc_countwords(char *s, char c)
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

static int	pc_size_nextword(char *s, char c, int i)
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

static int	pc_save_word(char *str, char *s, char c, int i)
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
