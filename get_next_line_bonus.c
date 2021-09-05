/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrbrune <chrbrune@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/04 19:41:14 by chrbrune          #+#    #+#             */
/*   Updated: 2021/09/05 12:48:41 by chrbrune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static void	free_ptr(char **ptr)
{
	free(*ptr);
	*ptr = NULL;
}

static char	*extract_line(char **save)
{
	int		i;
	char	*line;
	char	*temp_free;

	i = 0;
	while ((*save)[i] != '\0' && (*save)[i] != '\n')
		i++;
	temp_free = *save;
	line = ft_substr(temp_free, 0, i + 1);
	*save = ft_strdup(&(*save)[i + 1]);
	free_ptr(&temp_free);
	return (line);
}

static int	read_file(int fd, char **buffer, char **save)
{
	int		bytes_read;
	char	*temp_free;

	bytes_read = 1;
	while (!ft_strchr(*save, '\n') && bytes_read)
	{
		bytes_read = read(fd, *buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (bytes_read);
		(*buffer)[bytes_read] = '\0';
		temp_free = *save;
		*save = ft_strjoin(temp_free, *buffer);
		free_ptr(&temp_free);
	}
	return (bytes_read);
}

static char	*get_line(int fd, char **buffer, char **save)
{
	int		bytes_read;
	char	*temp_free;

	if (ft_strchr(*save, '\n'))
		return (extract_line(save));
	bytes_read = read_file(fd, buffer, save);
	if ((bytes_read == 0 || bytes_read == -1) && !**save)
	{
		free_ptr(save);
		return (NULL);
	}
	if (ft_strchr(*save, '\n'))
		return (extract_line(save));
	if (!ft_strchr(*save, '\n') && **save)
	{
		temp_free = ft_strdup(*save);
		free_ptr(save);
		return (temp_free);
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*save[OPEN_MAX + 1];
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > OPEN_MAX)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (!save[fd])
		save[fd] = ft_strdup("");
	line = get_line(fd, &buffer, &save[fd]);
	free_ptr(&buffer);
	return (line);
}
