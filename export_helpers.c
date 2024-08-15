#include "minishell.h"

void	swap_the_nodes(t_env *tmp1, t_env *tmp2)
{
	char	*temp_name;
	char	*temp_value;
	int		temp_index;

	temp_name = tmp1->name;
	temp_value = tmp1->value;
	temp_index = tmp1->index;
	tmp1->name = tmp2->name;
	tmp1->value = tmp2->value;
	tmp1->index = tmp2->index;
	tmp2->name = temp_name;
	tmp2->value = temp_value;
	tmp2->index = temp_index;
}

void	sort_env(t_env *env)
{
	t_env	*tmp1;
	t_env	*tmp2;

	tmp1 = env;
	while (tmp1)
	{
		tmp2 = tmp1->next;
		while (tmp2)
		{
			if (ft_strcmp(tmp1->name, tmp2->name) > 0)
				swap_the_nodes(tmp1, tmp2);
			tmp2 = tmp2->next;
		}
		tmp1 = tmp1->next;
	}
}

void	index_env(t_env *env)
{
	t_env	*tmp;
	t_env	*tmp2;

	tmp = env;
	while (tmp)
	{
		tmp->index = 0;
		tmp = tmp->next;
	}
	tmp = env;
	while (tmp)
	{
		tmp2 = env;
		while (tmp2)
		{
			if (ft_strcmp(tmp->name, tmp2->name) > 0)
				tmp->index++;
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	sort_env(env);
}

char	*get_name(char *str)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '=') || (str[i] == '+' && str[i + 1] == '='))
			break ;
		i++;
	}
	result = malloc(i + 1);
	if (!result)
		return (NULL);
	while (j < i)
	{
		result[j] = str[j];
		j++;
	}
	result[i] = '\0';
	return (result);
}

char	*get_value(char *str)
{
	char	*find;

	find = ft_strchr(str, '=');
	if (!find)
		return (NULL);
	return (find + 1);
}
