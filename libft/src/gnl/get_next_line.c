/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlundaev <vlundaev@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:51:43 by vlundaev          #+#    #+#             */
/*   Updated: 2025/07/21 18:13:02 by vlundaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	read_buffer(int fd, char *buffer, char **line);
static char	*extract_line(char *buffer, char *line);
static void	shift_buffer(char *buffer);

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = ft_strdup("");
	if (!line)
		return (NULL);
	while (1)
	{
		if (!buffer[0])
		{
			if (!read_buffer(fd, buffer, &line))
				return (line);
		}
		line = extract_line(buffer, line);
		if (!line)
			return (NULL);
		if (ft_strchr(buffer, '\n'))
		{
			shift_buffer(buffer);
			return (line);
		}
		buffer[0] = '\0';
	}
}

static int	read_buffer(int fd, char *buffer, char **line)
{
	int	bytes_read;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read <= 0)
	{
		if (!(*line)[0])
		{
			free(*line);
			*line = NULL;
		}
		return (0);
	}
	buffer[bytes_read] = '\0';
	return (1);
}

static char	*extract_line(char *buffer, char *line)
{
	char	*joined;
	char	*newline;
	char	*temp;

	newline = ft_strchr(buffer, '\n');
	if (newline)
		temp = ft_substr(buffer, 0, newline - buffer + 1);
	else
		temp = ft_strdup(buffer);
	if (!temp)
		return (NULL);
	joined = ft_strjoin(line, temp);
	free(line);
	free(temp);
	return (joined);
}

static void	shift_buffer(char *buffer)
{
	int	nl;
	int	i;

	i = 0;
	nl = ft_strchr(buffer, '\n') - buffer + 1;
	while (buffer[nl + i])
	{
		buffer[i] = buffer[nl + i];
		i++;
	}
	buffer[i] = '\0';
}
