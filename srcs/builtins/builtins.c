/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 15:57:51 by davgarci          #+#    #+#             */
/*   Updated: 2023/03/24 21:03:12 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtins(char **commands)
{
	//char	**commands;

	//commands = ft_split(str, SPACE);
	if (!commands)
		return (1);
	else if (ft_strcmp(commands[0], "pwd") == 0)
	{
		ft_putstr_fd("builtins pwd\n", 2);
		return (ft_pwd(commands));
	}
	else if (ft_strcmp(commands[0], "env") == 0)
		return (ft_env(g_c.env, commands));
	else if (ft_strcmp(commands[0], "cd") == 0)
	{
		ft_putstr_fd("builtins cd\n", 2);
		return (ft_cd(g_c.env, commands));
	}
	else if (ft_strcmp(commands[0], "echo") == 0)
	{
		ft_putstr_fd("builtins echo\n", 2);
		return (ft_echo(commands));
	}
	else if (ft_strcmp(commands[0], "export") == 0)
		return (ft_export(g_c.env, commands));
	else if (ft_strcmp(commands[0], "unset") == 0)
		return (ft_unset(g_c.env, commands));
	else
	{
		//free(commands); // No hay que hacerlo pq si no hemos conseguido builtins, lo va a hacer el execve
		return (42);
	}
}
