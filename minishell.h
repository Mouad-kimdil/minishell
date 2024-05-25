/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 14:50:37 by mkimdil           #+#    #+#             */
/*   Updated: 2024/05/25 22:16:05 by mkimdil          ###   ########.fr       */
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
# include <fcntl.h>

# define	GREEN  "\033[0;34m"
# define	NC  "\033[0m"

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


/* tis is an example of the struct i need */
typedef struct s_cmd
{
	char			*cmd;
	char			**argv;
	int				infile;
	int				outfile;
	struct s_cmd	*next;
}					t_cmd;

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
int		ft_strnstr(char *str, char *to_find);
t_env	*env_init(char **envp);
int		is_number(char c);
int		is_upper(char c);
int		is_lower(char c);
int		is_ascii(char c);

/*************BUILT in**/
int		echo(char **argv, int outfile);
int		cd(char **argv, t_list *list);
int		exit_function(char **argv);
void	env_1(char **argv, t_env *list, int outfile);
void	env(char **argv, t_list *list, int outfile);
int		unset(char **argv, t_env **envps);
void	pwd(char **args, t_list *list, int outfile);
void	export(char **argv, t_list *list, int outfile);

// parsing func
int		parsing(char *line, char **envp, t_cmd *lst);
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
char	*single_quotes(char *cmd, char *curr, int *j);
char	*double_quotes(char *cmd, char *curr, int *j, t_env *env);
char	*dollar_sign(char *cmd, char *curr, int *j, t_env *env);
char	*normal_char(char *cmd, char *curr, int *j);
char	*special_case_1(char *cmd, char *curr, int *j, t_env *env);
char	*arith_exp(char *cmd, char *current, int *j);
int		evaluate_expression(char *expr);
char	*my_strtok(char *str, char *delim);

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

#endif
