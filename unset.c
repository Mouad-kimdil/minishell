#include "minishell.h"

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
