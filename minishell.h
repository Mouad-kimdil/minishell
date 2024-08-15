/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:50:37 by mkimdil           #+#    #+#             */
/*   Updated: 2024/08/14 23:48:04 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <fcntl.h>
# include <errno.h>

# define BUFFER_SIZE 33

extern int	g_signal_status;

typedef struct s_append
{
	char	**temp;
	int		i;
	int		j;
	int		k;
	int		total_len;
}	t_append;

typedef struct s_parse
{
	char	*temp;
	char	*str;
	char	**res;
}	t_parse;

typedef struct s_expand
{
	int		len;
	int		tr;
	int		in_single_quote;
	int		in_double_quote;
	char	*cmd;
	char	*temp;
	char	*current;
	char	*name;
	char	*value;
}	t_expand;

typedef struct s_exp
{
	char			**splited;
	struct s_exp	*next;
}	t_exp;

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
	char			**del;
	int				fl1;
	int				fd;
	int				inf;
	int				outfile;
	int				in_quote;
	int				is_heredoc;
	int				ambiguous;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_execute
{
	int	fd[2];
	int	id;
	int	fd_int;
	int	fd_out;
}			t_execute;

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
int		echo(char **argv);
int		cd(char **argv, t_list *list);
int		exit_function(char **argv);
void	env_1(char **argv, t_env *list, int outfile);
void	env(char **argv, t_list *list);
int		unset(char **argv, t_env **envps);
void	pwd(char **args, t_list *list);
void	export(char **argv, t_list *list);
int		syn_error(char *line);
int		syn_err_chars(int c);
int		last_check(int c);
int		is_red(int c);
int		double_red(char *s);
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
void	ex(t_cmd *node, t_list *list);
void	home_function(char *home, t_list *list);
void	error_function(char *home, t_list *list);
int		check_if_flag(char *argv);
char	**env_split(char *s, char c);
char	*my_getenv(char *name, t_list *list);
void	update_env(char *name, char *value, t_list *list);
void	update_pwd(t_list *list);
t_env	*ft_lstnew(char *name, char *value);
void	ft_lstadd_back(t_env **lst, t_env *new);
char	*ft_substr(char *s, int start, int len);
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
void	fr(char **str);
void	f_cmd(t_cmd **lst);
char	*command(char *my_argv, char **envr);
int		env_size(t_env *env);
char	*get_name(char *str);
char	*get_value(char *str);
void	index_env(t_env *env);
int		update_the_value(char *name, char *value, t_list *list);
int		add_the_value(char *name, char *value, t_list *list);
void	add_env(t_env **env, char *name, char *value);
void	remove_qoutes(t_cmd **lst);
int		is_heredoc(t_cmd *lst);
char	*creat_heroc(t_cmd *lst);
void	perferm_heredoc_help(int fd, char *exp);
int		perferm_heredoc(t_cmd *lst, char *del, t_list *env);
void	her_sin(int sig);
void	get_del(t_cmd *lst);
int		get_del_size(t_cmd *lst);
int		exit_status(int status, int mode);
void	ft_putnbr_fd(int n, int fd);
void	ft_putchar_fd(char c, int fd);
char	*ft_strcat(char *dest, char *src);
char	*ft_strncpy(char *dest, char *src, unsigned int n);
int		ft_isalnum(int c);
char	*ft_strncat(char *dest, char *src, unsigned int nb);
int		split_stlen(char **str);
int		checking_error(t_cmd *node, int index);
void	new_array(t_cmd *node, int *index, int j);
int		checking_ambigious(t_cmd *node);
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
int		ft_isspace(char str);
int		check_is_ambg(char *expanded);
int		is_blank(char *str);
int		check_line(char **res);
void	ft_add_back(t_exp **lst, t_exp *new);
t_exp	*last_node(t_exp *lst);
t_exp	*ft_new_node(char *str);
char	*unquote(char *input);
int		count_double(char *input);
int		count_single(char *input);
void	put_fd(char *s, int fd);
char	*handle_dollar_sign(char *curr, char *cmd, int *j, t_list *envp);
char	*handle_other_cases(char *curr, char *cmd, int *j);
char	*expand_here_cmd(char *temp, t_list *envp);
char	*expand_heredoc(char *temp, t_list *envp, int in);
char	*get_env_value(char *name, t_env *env);
int		special_case(char c);
void	expand_with_space(t_cmd *lst, char *expanded, int *i);
void	expand_without_space(t_cmd *lst, int *tr, int *i, char *expanded);
void	expand_helper(t_cmd *lst, t_list *envp, int *i, int *tr);
void	handle_special_case(t_expand *exp, int *j, t_cmd *lst, t_list *envp);
char	*expand_cmd(t_cmd *lst, t_list *envp, int i);
void	expand(t_cmd *lst, t_list *envp);
void	add_back(t_cmd **lst, t_cmd *new);
t_cmd	*get_last(t_cmd *lst);
t_cmd	*ft_new(char *cmd);
void	remove_qoutes(t_cmd **lst);
void	process_argv(char **argv, int in);
void	remove_quotes_from_arg(char *arg, int in);
void	remove_quotes_from_arg_helper(char	*arg, char qoutes, int *tr);
void	free_list(t_list *list);
int		args_len(char **arg);
char	**join_args(char **s1, char *expanded);
char	**join_args_help(char **s1, char **splited, char **res, int i);
int		special_case(char c);
void	free_env(char *name, t_env **envps);
void	free_exp(t_expand *exp);
char	**ft_split_2(char *s);
char	**ft_help_2(char *s, int len, char **final);
int		countword_2(char *s);
int		is_whitespace(int c);
char	*duplicate(char *str);
t_env	*find_env(t_env *envs, char *key);
int		heredoc(t_cmd *l, t_list *env);
void	last_case(t_expand *exp, int *j);
int		parsing(t_cmd **lst, t_parse *p, t_list *list);
int		tty_error(t_parse *p);
void	secure_path(t_list *list);
void	f_env(t_env *envs);
void	free_parse(t_parse *p);
void	dolar_dolar_case(t_expand *exp, int *j);
int		expand_cases(char c);
void	numeric_expand(t_cmd *lst, t_expand *exp, int *j);
void	handle_double_quote(t_expand *exp, int *j, t_cmd *lst, t_list *envp);
void	check_expanded(t_cmd *lst, int *i, int *tr);
void	remove_null_values(char **argv, int size);
char	**append(char **argv, int len);
void	handle_single_quote(t_expand *exp, int *j);
void	noex_single(t_expand *exp, int *j);
char	*expand_export(t_cmd *lst, t_list *envp, int i);
char	*get_env_value_2(char *name, t_env *env);
void	my_free(void *ptr);

#endif
