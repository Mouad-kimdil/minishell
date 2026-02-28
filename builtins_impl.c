#include "minishell.h"

void	home_function(char *home, t_list *list)
{
	home = my_getenv("HOME", list);
	if (!home)
	{
		printf("cd: HOME not set\n");
		exit_status(1, 1);
		return ;
	}
	if (chdir(home) == -1)
	{
		printf("cd: %s: No such file or directory\n", home);
		exit_status(1, 1);
	}
	update_pwd(list);
	exit_status(0, 1);
}

void	error_function(char *home, t_list *list)
{
	home = my_getenv("HOME", list);
	if (!home)
		printf("cd: HOME not set\n");
	if (chdir(home) == -1)
		printf("cd: %s: No such file or directory\n", home);
	printf("cd: error retrieving current directory: ");
	printf("getcwd: cannot access parent directories: ");
	printf ("No such file or directory\n");
	exit_status(1, 1);
	update_pwd(list);
}

t_env	*find_env(t_env *envs, char *key)
{
	while (envs)
	{
		if (ft_strcmp(envs->name, key) == 0)
			return (envs);
		envs = envs->next;
	}
	return (NULL);
}

void	update_oldpwd(t_list *list, char *old_pwd)
{
	t_env	*oldpwd_env;
	char	*new_value;

	oldpwd_env = find_env(list->envs, "OLDPWD");
	if (oldpwd_env)
	{
		free(oldpwd_env->value);
		if (!old_pwd)
			new_value = getcwd(NULL, 0);
		else
			new_value = ft_strdup(old_pwd);
		oldpwd_env->value = new_value;
	}
	else
	{
		if (!old_pwd)
			old_pwd = getcwd(NULL, 0);
		oldpwd_env = ft_lstnew("OLDPWD", old_pwd);
		ft_lstadd_back(&list->envs, oldpwd_env);
	}
}

int	cd(char **argv, t_list *list)
{
	int		i;
	char	*home;
	char	*old_pwd;

	i = 1;
	old_pwd = getcwd(NULL, 0);
	home = NULL;
	while (argv[i])
		i++;
	if (i == 1)
		return (home_function(home, list), free(old_pwd), 0);
	if (chdir(argv[1]) == -1)
	{
		if (!ft_strcmp(argv[1], ".."))
			return (error_function(home, list), free(old_pwd), 0);
		else if (!ft_strcmp(argv[1], "\0"))
			return (exit_status(0, 1), free(old_pwd), 0);
		printf("cd: %s: No such file or directory\n", argv[1]);
		free(old_pwd);
		return (exit_status(1, 1), 1);
	}
	update_pwd(list);
	update_oldpwd(list, old_pwd);
	return (free(old_pwd), exit_status(0, 1), 0);
}

int	check_if_flag(char *argv)
{
	int	i;

	i = 0;
	if (!ft_strcmp(argv, "-n"))
		return (0);
	if (argv[0] != '-')
		return (1);
	i++;
	while (argv[i])
	{
		if (argv[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	print_arguments(char **argv, int start)
{
	int	i;

	i = start;
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	return (1);
}

int	checking_null(char **argv)
{
	if (*argv == NULL)
	{
		write(1, "\n", 1);
		return (1);
	}
	return (0);
}

int	echo(char **argv)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (checking_null(argv))
		return (0);
	if (ft_strcmp(argv[i], "-") == 0)
	{
		write(1, argv[i], ft_strlen(argv[i]));
		write(1, " ", 1);
		i++;
	}
	while (argv[i] && !check_if_flag(argv[i]))
	{
		flag = 1;
		i++;
	}
	if (print_arguments(argv, i) == 0)
		return (0);
	if (!flag)
		write(1, "\n", 1);
	return (0);
}

void	env_split_helper(char *s, char c, char **result, int *i)
{
	while (s[*i] && s[*i] != c)
	{
		result[0][*i] = s[*i];
		(*i)++;
	}
	result[0][*i] = '\0';
	if (s[*i])
	{
		result[1] = ft_strdup(s + *i + 1);
		if (!result[1])
			return ;
	}
	else
		result[1] = NULL;
	result[2] = NULL;
}

char	**env_split(char *s, char c)
{
	char	**result;
	int		i;

	if (!s)
		return (NULL);
	result = malloc(sizeof(char *) * 3);
	if (!result)
		return (NULL);
	i = 0;
	while (s[i] && s[i] != c)
		i++;
	result[0] = malloc(sizeof(char) * (i + 1));
	if (!result[0])
		return (free(result), NULL);
	i = 0;
	env_split_helper(s, c, result, &i);
	return (result);
}

t_env	*env_init(char **envp)
{
	t_env	*head;
	int		i;
	char	**sp;
	t_env	*new;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		sp = env_split(envp[i], '=');
		if (!sp)
			break ;
		if (sp[1] != NULL)
		{
			new = ft_lstnew(sp[0], sp[1]);
			if (new == NULL)
				break ;
			ft_lstadd_back(&head, new);
		}
		free_str_array(sp);
		i++;
	}
	return (head);
}

void	env(char **argv, t_list *list)
{
	t_env	*env;

	(void)argv;
	env = list->envs;
	while (env)
	{
		if (env->value)
		{
			write(1, env->name, ft_strlen(env->name));
			write(1, "=", 1);
			write(1, env->value, ft_strlen(env->value));
			write(1, "\n", 1);
		}
		env = env->next;
	}
	exit_status(0, 1);
}

void	env_print_to_fd(char **argv, t_env *list, int outfile)
{
	(void) *argv;
	while (list)
	{
		if (list->value)
		{
			write(outfile, list->name, ft_strlen(list->name));
			write(outfile, "=", 1);
			write(outfile, list->value, ft_strlen(list->value));
			write(outfile, "\n", 1);
		}
		list = list->next;
	}
}

int	special_case(char c)
{
	return (is_ascii(c) || is_number(c) || c == '_');
}

int	args_len(char **arg)
{
	int	i;

	if (!arg || !*arg)
		return (0);
	i = 0;
	while (arg[i])
		i++;
	return (i);
}

int	is_all_num(char *s)
{
	while (*s == ' ')
		s++;
	if (*s == '-' || *s == '+')
		s++;
	if (*s == '\0')
		return (0);
	while (*s)
	{
		if (!is_number(*s) || *s == ' ')
			break ;
		s++;
	}
	while (*s && *s == ' ')
		s++;
	if (*s == '\0')
		return (1);
	return (0);
}

int	exit_functions_help(char *argv1, int state)
{
	if (state == -1)
	{
		exit_status(256 + ft_atoi(argv1), 1);
		exit(256 + ft_atoi(argv1));
	}
	else if (state == 1)
	{
		exit_status(ft_atoi(argv1) % 256, 1);
		exit(ft_atoi(argv1) % 256);
	}
	else
	{
		exit_status(ft_atoi(argv1), 1);
		exit(ft_atoi(argv1));
	}
}

int	exit_function(char **argv)
{
	printf("exit\n");
	if (argv[1] == NULL)
		exit(exit_status(0, 0));
	if (is_all_num(argv[1]))
	{
		if (argv[2] == NULL)
		{
			if (ft_atoi(argv[1]) < 0)
				exit_functions_help(argv[1], -1);
			else if (ft_atoi(argv[1]) > 255)
				exit_functions_help(argv[1], 1);
			else
				exit_functions_help(argv[1], 0);
		}
		else
			return (printf("too many arguments\n"), 1);
	}
	printf("%s: %s: numeric argument required\n", argv[0], argv[1]);
	return (exit_status(255, 1), exit(255), 0);
}

void	pwd(char **args, t_list *list)
{
	char	*pwdir;
	char	*tmp;
	int		allocated;

	(void)args;
	allocated = 0;
	pwdir = my_getenv("PWD", list);
	if (!pwdir)
	{
		pwdir = getcwd(NULL, 0);
		allocated = 1;
		tmp = my_getenv("OLDPWD", list);
		if (!pwdir)
		{
			if (tmp)
				printf("%s\n", tmp);
			exit_status(1, 1);
			return ;
		}
	}
	write(1, pwdir, ft_strlen(pwdir));
	write(1, "\n", 1);
	if (allocated)
		free(pwdir);
	exit_status(0, 1);
}

void	error_open(char *str)
{
	perror(str);
	return ;
}

void	free_env(char *name, t_env **envps)
{
	t_env	*prev;
	t_env	*tmp;

	tmp = *envps;
	prev = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
		{
			if (!ft_strcmp(tmp->name, "_"))
				return ;
			if (!prev)
				*envps = tmp->next;
			else
				prev->next = tmp->next;
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	is_valid_variable_name(char *name)
{
	int	i;

	i = 0;
	if (!name || name[0] == '\0' || is_number(name[0]))
		return (0);
	while (name[i])
	{
		if (!(is_number(name[i]) || is_lower(name[i])
				|| is_upper(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	check_is_unsetable(char *name)
{
	if (!is_valid_variable_name(name))
	{
		printf("minishell: unset: `%s': not a valid identifier\n", name);
		exit_status(1, 1);
		return (1);
	}
	return (0);
}

int	unset(char **argv, t_env **envps)
{
	int	i;

	i = 1;
	if (*(argv + 1) == NULL)
		return (0);
	while (argv[i])
	{
		if (check_is_unsetable(argv[i]))
			return (0);
		free_env(argv[i], envps);
		i++;
	}
	exit_status(0, 1);
	return (0);
}
