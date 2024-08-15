#include "minishell.h"

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
