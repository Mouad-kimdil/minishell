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

void	print_list(t_cmd *lst)
{
	while (lst)
	{
		int i = 0;
		while (lst->argv[i])
		{
			printf("lst->argv[%d]: %s\n", i, lst->argv[i]);
			i++;
		}
		lst = lst->next;
	}
}

int	parsing(t_cmd **lst, t_parse *p, t_list *list)
{
	p->str = add_space(p->temp);
	if (!p->str)
		return (free(p->temp), 1);
	change_to_garb(p->str);
	if (handle_single_double(p->str))
		return (free(p->temp), free(p->str), 1);
	if (syn_error(p->str))
		return (exit_status(258, 1), free(p->temp), free(p->str), 1);
	p->res = ft_split(p->str, '|');
	if (!p->res)
		return (free(p->temp), free(p->str), 1);
	*lst = build_arr(p->res);
	if (!*lst)
		return (free(p->temp), free(p->str), fr(p->res), 1);
	back_to_ascii(*lst);
	if (is_heredoc(*lst))
		if (heredoc(*lst, list))
			return (free_parse(p), f_cmd(lst), 1);
	expand(*lst, list);
	remove_qoutes(lst);
	return (free_parse(p), 0);
}

int	main(int ac, char **av, char **env)
{
	t_parse			p;
	t_cmd			*lst;
	t_list			*l;
	struct termios	copy;
	(void)			av;

	if (ac != 1)
		return (1);
	g_signal_status = 0;
	l = malloc(sizeof(t_list));
	if (!l)
		return (1);
	l->envs = env_init(env);
	if (!l->envs)
		secure_path(l);
	while (1)
	{
		rl_catch_signals = 0;
		check_signals();
		p.temp = readline("Minishell-$ ");
		if (!p.temp || !isatty(0))
			return (f_env(l->envs), free(l), put_fd("exit\n", 2), exit_status(0, 0));
		if (!ft_strlen(p.temp) || is_blank(p.temp))
		{
			free(p.temp);
			continue ;
		}
		add_history(p.temp);
		if (parsing(&lst, &p, l))
			continue ;
		g_signal_status = 1;
		tcgetattr(0, &copy);
		ex(lst, l);
		tcsetattr(0, 0, &copy);
		g_signal_status = 0;
		f_cmd(&lst);
	}
	return (f_env(l->envs), free(l), 0);
}
