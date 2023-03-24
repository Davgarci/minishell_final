/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgarci <davgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 20:41:16 by psegura-          #+#    #+#             */
/*   Updated: 2023/03/24 01:46:23 by davgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*__HEADER FILES__*/
# include "minishell.h"
# include "libs.h"

/*__READLINE______*/
# include "readline.h"

/*__TOKENIZER_____*/
# include "tokenizer.h"

/*__INTRO_________*/
# include "intro.h"

/*__EXECUTOR______*/
# include "executor.h"

/*__BUILTINS______*/
# include "builtins.h"

/*__UTILS_________*/
# include "utils.h"

/*__ERROR_MESSAGES_________*/
# include "error_messages.h"

/*__LIBFT_________*/
# include "../libft/inc/libft.h"

# define READ_END	0
# define WRITE_END	1

/*
	pipe_parse = pipa y antes de redireccion spliteado por espacios
	pipe = 0 si es ultima pipa / 1 si no es la ultima
	read = 1 si pilla imput en redireccion / 0 en el resto / 2 en no sabemos
*/

typedef struct s_pipe
{
	char			**pipe_parse;
	int				pipe;
	int				read;
	int				fd[2];
	struct s_pipe	*next;
	struct s_pipe	*previous;
}					t_pipe;

typedef struct s_cosas {
	char	**env;
	char	**tokens;
	int		tok_count;
	int		flag;
	int		pipas;
	int		dolar_q;
	int		fd[2];
	int		redirection_out;
	int		redirection_in;
	int		read;
	int		forking;
	char	**cmd_exec_parsed;
	int		status_last_command;
	t_pipe	*pipe_list;
}	t_cosas;

t_cosas	g_c;

#endif
