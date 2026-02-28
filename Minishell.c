#include "minishell.h"

int	g_signal_status;

static int	run_pipeline_string(char *pipe_str, t_list *list)
{
	char	**res;
	t_cmd	*lst;
	int		status;

	res = ft_split(pipe_str, '|');
	if (!res)
		return (-1);
	lst = build_arr(res);
	if (!lst)
		return (free_str_array(res), -1);
	back_to_ascii(lst);
	if (is_heredoc(lst) && heredoc(lst, list))
		return (f_cmd(&lst), free_str_array(res), -1);
	expand(lst, list);
	remove_quotes(&lst);
	execute_pipeline(lst, list);
	status = exit_status(0, 0);
	f_cmd(&lst);
	free_str_array(res);
	return (status);
}

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
	char	**or_parts;
	char	**and_parts;
	int		i;
	int		j;
	int		status;
	int		success;
	struct termios	copy;

	p->str = add_space(p->temp);
	if (!p->str)
		return (free(p->temp), 1);
	mask_quoted_chars(p->str);
	if (handle_single_double(p->str))
		return (free(p->temp), free(p->str), 1);
	if (syn_error(p->str))
		return (exit_status(258, 1), free(p->temp), free(p->str), 1);
	or_parts = split_by_delim(p->str, " || ");
	if (!or_parts)
		return (free_parse(p), 1);
	if (or_parts[0] && !or_parts[1])
	{
		and_parts = split_by_delim(or_parts[0], " && ");
		if (and_parts && and_parts[0] && !and_parts[1])
		{
			p->res = ft_split(or_parts[0], '|');
			free_str_array(and_parts);
			free_str_array(or_parts);
			if (!p->res)
				return (free_parse(p), 1);
			*lst = build_arr(p->res);
			if (!*lst)
				return (free_parse(p), 1);
			back_to_ascii(*lst);
			if (is_heredoc(*lst) && heredoc(*lst, list))
				return (free_parse(p), f_cmd(lst), 1);
			expand(*lst, list);
			remove_quotes(lst);
			return (free_parse(p), 0);
		}
		if (and_parts)
			free_str_array(and_parts);
	}
	status = 0;
	g_signal_status = 1;
	tcgetattr(0, &copy);
	for (i = 0; or_parts[i]; i++)
	{
		and_parts = split_by_delim(or_parts[i], " && ");
		if (!and_parts)
			continue ;
		success = 1;
		for (j = 0; and_parts[j]; j++)
		{
			status = run_pipeline_string(and_parts[j], list);
			if (status < 0)
				return (free_str_array(and_parts), free_str_array(or_parts), tcsetattr(0, 0, &copy), g_signal_status = 0, (p->res = NULL, free_parse(p)), 1);
			if (status != 0)
			{
				success = 0;
				break ;
			}
		}
		free_str_array(and_parts);
		if (success)
			break ;
	}
	tcsetattr(0, 0, &copy);
	g_signal_status = 0;
	free_str_array(or_parts);
	p->res = NULL;
	free_parse(p);
	*lst = NULL;
	return (0);
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
		if (lst)
		{
			g_signal_status = 1;
			tcgetattr(0, &copy);
			execute_pipeline(lst, l);
			tcsetattr(0, 0, &copy);
			g_signal_status = 0;
			f_cmd(&lst);
		}
	}
	return (f_env(l->envs), free(l), 0);
}
