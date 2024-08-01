/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 04:01:04 by mkimdil           #+#    #+#             */
/*   Updated: 2024/08/01 05:31:11 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_status;

void	secure_path(t_list *list)
{
	char	*path;
	char	*pwd;
	t_env	*new_env;

	path = "/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.";
	pwd = getcwd(NULL, 0);
	new_env = NULL;
	if (!pwd)
		return ;
	new_env = ft_lstnew("PWD", pwd);
	if (new_env)
		ft_lstadd_back(&list->envs, new_env);
	new_env = ft_lstnew("SHLVL", "1");
	if (new_env)
		ft_lstadd_back(&list->envs, new_env);
	new_env = ft_lstnew("_", "/usr/bin/env");
	if (new_env)
		ft_lstadd_back(&list->envs, new_env);
	new_env = ft_lstnew("PATH", path);
	if (new_env)
		ft_lstadd_back(&list->envs, new_env);
	free(pwd);
}

void	f_env(t_env *envs)
{
	t_env	*next;

	while (envs)
	{
		next = envs->next;
		free(envs->name);
		free(envs->value);
		free(envs);
		envs = next;
	}
	envs = NULL;
}

int	tty_error(t_parse *p)
{
	if (!p->temp || !isatty(0))
		return (1);
	return (0);
}

int	parsing(t_cmd **lst, t_parse *p, t_list *list)
{
	p->temp = readline("Minishell-$ ");
	if (!ft_strlen(p->temp) || is_blank(p->temp))
		return (free(p->temp), 1);
	add_history(p->temp);
	if (!ft_strlen(p->temp) || is_blank(p->temp))
		return (free(p->temp), 1);
	p->str = add_space(p->temp);
	if (!p->str)
		return (free(p->temp), 1);
	if (syn_error(p->str))
		return (free(p->temp), free(p->str), 1);
	change_to_garb(p->str);
	if (handle_single_double(p->str))
		return (free(p->temp), free(p->str), 1);
	p->res = ft_split(p->str, '|');
	if (!p->res)
		return (free(p->temp), free(p->str), 1);
	*lst = build_arr(p->res);
	if (!*lst)
		return (free(p->temp), free(p->str), free_all(p->res), 1);
	back_to_ascii(*lst);
	if (is_heredoc(*lst))
		heredoc(*lst, list);
	expand(*lst, list);
	return (remove_qoutes(lst), free_parse(p), 0);
}

int	main(int ac, char **av, char **env)
{
	t_parse			p;
	t_cmd			*lst;
	t_list			*l;
	struct termios	copy;

	if (ac != 1)
		return (1);
	(1) && ((void)av, g_signal_status = 0, l = malloc(sizeof(t_list)), 0);
	(1) && (l->envs = env_init(env), 0);
	if (!l->envs)
		secure_path(l);
	while (1)
	{
		if (tty_error(&p))
			return (f_env(l->envs), free(l), free(lst), put_fd("exit\n", 2), 0);
		(1) && (rl_catch_signals = 0, check_signals(), 0);
		if (parsing(&lst, &p, l))
			continue ;
		(1) && (g_signal_status = 1, tcgetattr(0, &copy), ex(lst, l), 0);
		(1) && (tcsetattr(0, 0, &copy), g_signal_status = 0, f_cmd(&lst), 0);
	}
	return (f_env(l->envs), free(l), 0);
}
