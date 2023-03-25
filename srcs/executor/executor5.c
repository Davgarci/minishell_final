/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 16:58:51 by davgarci          #+#    #+#             */
/*   Updated: 2023/03/25 17:01:57 by davgarci         ###   ########.fr       */
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
