/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:50:37 by mkimdil           #+#    #+#             */
/*   Updated: 2024/07/16 20:04:59 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	$VAR == ALPHA NUM + _
*/

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <fcntl.h>

# define GREEN  "\033[0;34m"
# define NC  "\033[0m"
# define BUFFER_SIZE 33

int	g_signal_status;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				index;
	struct s_env	*next;
}				t_env;

typedef struct s_list
{
	t_env	*envs;
}				t_list;

typedef struct s_cmd
{
	char			*cmd;
	char			**argv;
	char			**delim;
	int				fd;
	int				infile;
	int				outfile;
	int				is_heredoc;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_execute
{
	int	fd[2];
	int	id;
	int	fd_int;
	int	fd_out;
}			t_execute;

typedef struct s_help
{
	char	*exp;
	char	*name;
	char	*end;
	char	*start;
	char	*dollar;
	char	*var_value;
}				t_help;

typedef struct s_gc
{
	void		*ptr;
	struct s_gc	*next;
}	t_gc;

char	**ft_split(char *s, char c);
char	**ft_help(char *s, char c, int len, char **final);
void	ft_free(char **ptr, int i);
char	*ft_strndup(char *src, int n);
int		countword(char *s, char c);
int		ft_strncmp(char *s1, char *s2, int len);
char	*ft_strdup(char *str);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strlen(char *str);
char	*ft_strchr(char *s, int c);
int		ft_strsearch(char *s, int c);
int		ft_strnstr(char *str, char *to_find);
t_env	*env_init(char **envp);
int		is_number(char c);
int		is_upper(char c);
int		is_lower(char c);
int		is_ascii(char c);
int		echo(char **argv, int outfile);
int		cd(char **argv, t_list *list);
int		exit_function(char **argv);
void	env_1(char **argv, t_env *list, int outfile);
void	env(char **argv, t_list *list, int outfile);
int		unset(char **argv, t_env **envps);
void	pwd(char **args, t_list *list, int outfile);
void	export(char **argv, t_list *list, int outfile);
int		syn_error(char *line);
int		syn_error2(char **res);
int		syn_error3(char **res);
void	free_array(char **arr);
char	*add_space(char *line);
void	second_case(char *str, char *line, int *i, int *j);
int		count_num_of_special(char *line);
int		is_special_char(char c);
int		handle_single_double(char *line);
void	change_to_garb(char *line);
t_cmd	*build_arr(char **res);
int		ft_strcpy(char *dest, char *src);
t_cmd	*new_list(void *cmd);
void	back_to_ascii(t_cmd *lst);
char	*nops_strdup(char *str);
void	expand(t_cmd *lst, t_list *envp);
void	execution(t_cmd *node, t_list *list);
void	home_function(char *home, t_list *list);
void	old_pwd_function(char *home, t_list *list);
void	error_function(char *home, t_list *list);
int		check_if_flag(char *argv);
char	**env_split(char *s, char c);
char	*my_getenv(char *name, t_list *list);
void	update_env(char *name, char *value, t_list *list);
void	update_pwd(char *path, t_list *list);
t_env	*ft_lstnew(char *name, char *value);
void	ft_lstadd_back(t_env **lst, t_env *new);
int		special_case(char c);
char	*ft_substr(char *s, int start, int len);
char	*expand_cmd(t_cmd *lst, t_list *envp, int i);
int		ft_strcmp(char *s1, char *s2);
char	*ft_substr(char *s, int start, int len);
char	*ft_itoa(int nb);
int		ft_len(long nb);
int		ft_atoi(char *str);
char	*ft_strjoin_with_sep(char *s1, char *s2, char sep);
char	**get_path(char **envr);
int		redirect_out(t_cmd *node, int *index, int flags);
int		redirect_out_append(t_cmd *node, int *index, int flags);
int		redirect_in(t_cmd *node, int *index, int flags);
int		check_for_redirection(t_cmd *node);
void	msg_error(char *str);
void	close_files(t_cmd *node);
void	safe_pipe(int fd[2]);
int		safe_fork(void);
int		is_builtin(t_cmd *cmd, t_list *list);
void	env_to_char_array_helper(t_env *current, char **envp);
char	**env_to_char_array(t_env *head);
void	error_open(char *str);
void	free_all(char **str);
char	*command(char *my_argv, char **envr);
int		env_size(t_env *env);
char	*get_name(char *str);
char	*get_value(char *str);
void	index_env(t_env *env);
int		update_the_value(char *name, char *value, t_list *list);
int		add_the_value(char *name, char *value, t_list *list);
void	add_env(t_env **env, char *name, char *value);
void	remove_qoutes(t_cmd **lst);
int		check_end(char *line);
char	**handle_expand(t_cmd *lst);
int		is_heredoc(t_cmd *lst);
void	heredoc(t_cmd *lst);
char	*creat_heroc(t_cmd *lst);
void	perferm_heredoc(t_cmd *lst, char *delim);
void	her_sin(int sig);
void	get_delim(t_cmd *lst);
int		get_delim_size(t_cmd *lst);
int		ex_st(int status, int mode);
void	ft_putnbr_fd(int n, int fd);
void	ft_putchar_fd(char c, int fd);
char	*expand_variables(char *input);
char	*ft_strcat(char *dest, char *src);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
int		ft_isalnum(int c);
char	*ft_strncat(char *dest, char *src, unsigned int nb);
void	build_arr_help(t_cmd **lst, char *res);
int		split_stlen(char **str);
int		checking_error(t_cmd *node, int index);
void	new_array(t_cmd *node, int *index, int j);
int		checking_ambigious(t_cmd *node, int *index);
int		split_stlen(char **str);
int		checking_error(t_cmd *node, int index);
int		is_builtin(t_cmd *cmd, t_list *list);
int		checkbuiltin(t_cmd *cmd);
int		check_if_built(t_cmd *node, t_list *list, t_execute *exec);
void	close_all(t_cmd *node, t_execute *exec);
void	hand_l_command(t_cmd *node, t_list *list, t_execute *exec, char **envr);
void	my_dup1(t_cmd *node, t_execute *exec);
void	my_dup2(t_cmd *node);
void	function_sigint(int sig);
void	function_sigwuit(int sig);
void	check_signals(void);
void	remove_qoutes(t_cmd **lst);
int		count_argv(t_cmd *node);
void	handling_my_argv(t_cmd *node);
int		ft_isspace(char str);
int		is_blank(char *str);
#endif
