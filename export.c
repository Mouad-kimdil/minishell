/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkimdil <mkimdil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 11:38:59 by aboukdid          #+#    #+#             */
/*   Updated: 2024/05/25 21:09:03 by mkimdil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export(t_env *env, int outfile)
{
	int		size;
	int		i;

	i = 0;
	size = env_size(env);
	index_env(env);
	while (i < size)
	{
		if (env && env->index == i)
		{
			write(outfile, "declare -x ", 11);
			write(outfile, env->name, ft_strlen(env->name));
			if (env->value)
			{
				write(outfile, "=\"", 2);
				write(outfile, env->value, ft_strlen(env->value));
				write(outfile, "\"", 1);
			}
			write(outfile, "\n", 1);
			i++;
		}
		env = env->next;
	}
}

int	is_valid_to_export(char *str)
{
	int	i;

	i = 0;
	if (!is_lower(*str) && !is_upper(*str) && *str != '_')
		return (-1);
	while (str[i])
	{
		if (str[i] == '=')
			return (i + 1);
		if (str[i] == '+' && str[i + 1] == '=')
			return (i + 2);
		if (!(is_upper(str[i]) || is_lower(str[i])
				|| is_number(str[i]) || str[i] == '_'))
			return (-1);
		i++;
	}
	return (0);
}

void	checking_and_add(int is_valid, char *argv, t_list *list)
{
	char	*name;
	char	*value;
	int		is_modified;

	name = get_name(argv);
	value = get_value(argv);
	is_valid = ft_strlen(name);
	if (!(*argv + is_valid))
		is_modified = 1;
	else if (*(argv + is_valid) == '+')
		is_modified = update_the_value(name, value, list);
	else
		is_modified = add_the_value(name, value, list);
	if (is_modified == 1)
		add_env(&list->envs, name, value);
}

void	export(char **argv, t_list *list, int outfile)
{
	int		is_valid;
	t_env	*env;

	if (!*(argv + 1))
	{
		env = list->envs;
		print_export(env, outfile);
		return ;
	}
	argv++;
	while (*argv)
	{
		is_valid = is_valid_to_export(*argv);
		if (is_valid == -1)
			printf("minishell: export: `%s': not a valid identifier\n", *argv);
		else
			checking_and_add(is_valid, *argv, list);
		argv++;
	}
}
