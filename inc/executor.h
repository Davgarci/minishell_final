/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 10:25:30 by psegura-          #+#    #+#             */
/*   Updated: 2023/03/25 17:08:01 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

# define SPACE		' '
# define NONE		42
# define BUILTIN	1
# define STDLIB		2
# define OTHER		0

/*___CMD_TYPE___*/
int			cmd_is_builtin(char *cmd);
int			what_cmd(char *argv);

/*___EXEC_CMD___*/
void		ft_exec(char **cmd);

/*___FIND_COMMAND___*/
char		*check_path(void);
char		*only_path(char *cmd);
void		cmd_not_found(char *cmd);
void		exit_failure(char *err_msg, char **to_free, int flag);

#endif
