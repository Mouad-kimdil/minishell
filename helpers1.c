#include "minishell.h"

void	env_to_char_array_helper(t_env *current, char **envp)
{
	int	i;

	i = 0;
	while (current)
	{
		if (current->value)
		{
			envp[i] = ft_strjoin_with_sep(current->name, current->value, '=');
			if (!envp[i])
				return ;
			i++;
		}
		current = current->next;
	}
	envp[i] = NULL;
}

char	**env_to_char_array(t_env *head)
{
	int		i;
	t_env	*current;
	char	**envp;

	i = 0;
	current = head;
	i = env_size(head);
	if (i == 0)
		return (NULL);
	envp = malloc((i + 1) * sizeof(char *));
	if (!envp)
		return (NULL);
	current = head;
	env_to_char_array_helper(current, envp);
	envp[i] = NULL;
	return (envp);
}
