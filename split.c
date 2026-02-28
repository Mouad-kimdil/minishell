#include "minishell.h"

int	countword(char *s, char c)
{
	int	count;

	count = 0;
	if (!s)
		return (0);
	while (*s)
	{
		if (*s != c)
		{
			count++;
			while (*s && (*s != c))
				s++;
		}
		else
			s++;
	}
	return (count);
}

char	*ft_strndup(char *src, int n)
{
	int		i;
	char	*result;

	result = (char *)malloc(n + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (src[i] && i < n)
	{
		result[i] = src[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

void	ft_free(char **ptr, int i)
{
	while (i >= 0)
	{
		free(ptr[i]);
		i--;
	}
	free(ptr);
}

char	**ft_help(char *s, char c, int len, char **final)
{
	char	*start;
	int		i;

	len = countword(s, c);
	final = (char **)malloc((len + 1) * sizeof(char *));
	if (!final)
		return (NULL);
	i = 0;
	while (i < len)
	{
		while (*s == c)
			s++;
		start = (char *)s;
		while (*s && *s != c)
			s++;
		final[i] = ft_strndup(start, s - start);
		if (!final[i])
		{
			ft_free(final, i);
			return (NULL);
		}
		i++;
	}
	final[i] = NULL;
	return (final);
}

char	**ft_split(char *s, char c)
{
	int		len;
	char	**final;

	final = NULL;
	len = 0;
	return (ft_help(s, c, len, final));
}

static int	find_delim(char *s, char *delim)
{
	int	i;
	int	j;
	int	len;

	len = ft_strlen(delim);
	if (!len || !s)
		return (-1);
	i = 0;
	while (s[i])
	{
		j = 0;
		while (delim[j] && s[i + j] == delim[j])
			j++;
		if (delim[j] == '\0')
			return (i);
		i++;
	}
	return (-1);
}

static int	count_delim(char *s, char *delim)
{
	int	count;
	int	len;
	int	pos;
	int	off;

	len = ft_strlen(delim);
	if (!len || !s)
		return (0);
	count = 0;
	pos = 0;
	while (s[pos])
	{
		off = find_delim(s + pos, delim);
		if (off < 0)
			break ;
		count++;
		pos += off + len;
	}
	return (count);
}

char	**split_by_delim(char *s, char *delim)
{
	char	**out;
	int		n;
	int		k;
	int		pos;
	int		off;
	int		seglen;
	int		len;

	if (!s || !delim)
		return (NULL);
	len = ft_strlen(delim);
	n = count_delim(s, delim);
	out = malloc((n + 2) * sizeof(char *));
	if (!out)
		return (NULL);
	k = 0;
	pos = 0;
	while (k <= n)
	{
		off = find_delim(s + pos, delim);
		if (off < 0)
			seglen = ft_strlen(s + pos);
		else
			seglen = off;
		out[k] = ft_strndup(s + pos, seglen);
		if (!out[k])
		{
			ft_free(out, k - 1);
			return (NULL);
		}
		k++;
		pos += seglen;
		if (off >= 0)
			pos += len;
	}
	out[k] = NULL;
	return (out);
}

int	is_whitespace(int c)
{
	return (c == ' ' || c == '\t');
}

int	countword_2(char *s)
{
	int	count;

	count = 0;
	if (!s)
		return (0);
	while (*s)
	{
		if (!is_whitespace(*s))
		{
			count++;
			while (*s && !is_whitespace(*s))
				s++;
		}
		else
			s++;
	}
	return (count);
}

char	**ft_help_2(char *s, int len, char **final)
{
	char	*start;
	int		i;

	len = countword_2(s);
	final = (char **)malloc((len + 1) * sizeof(char *));
	if (!final)
		return (NULL);
	i = 0;
	while (i < len)
	{
		while (is_whitespace(*s))
			s++;
		start = (char *)s;
		while (*s && !is_whitespace(*s))
			s++;
		final[i] = ft_strndup(start, s - start);
		if (!final[i])
		{
			ft_free(final, i);
			return (NULL);
		}
		i++;
	}
	final[i] = NULL;
	return (final);
}

char	**ft_split_2(char *s)
{
	int		len;
	char	**final;

	final = NULL;
	len = 0;
	return (ft_help_2(s, len, final));
}
