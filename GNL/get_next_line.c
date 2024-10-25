/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:00:18 by miaviles          #+#    #+#             */
/*   Updated: 2024/10/25 16:52:21 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_and_append(int fd, char *remainder, char *buffer)
{
	ssize_t	bytes_read;
	char	*temp;

	bytes_read = 1;
	while (!ft_strchr(remainder, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(remainder);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		temp = remainder;
		remainder = ft_strjoin(temp, buffer);
		free(temp);
		if (!remainder)
			return (NULL);
	}
	return (remainder);
}

static char	*read_and_fill_buffer(int fd, char *remainder, char *buffer)
{
	if (!remainder)
	{
		remainder = (char *)malloc(1);
		remainder[0] = '\0';
		if (!remainder)
			return (NULL);
	}
	remainder = read_and_append(fd, remainder, buffer);
	return (remainder);
}

static char	*extract_line(char *remainder)
{
	char	*line;
	size_t	len;
	size_t	full_len;

	if (!remainder || *remainder == '\0')
		return (NULL);
	len = 0;
	while (remainder[len] && remainder[len] != '\n')
		len++;
	full_len = len;
	if (remainder[len] == '\n')
		full_len += 1;
	line = ft_substr(remainder, 0, full_len);
	if (!line)
		return (NULL);
	return (line);
}

static char	*update_remainder(char *remainder)
{
	char	*tmp;
	char	*updated_remainder;

	tmp = ft_strchr(remainder, '\n');
	if (tmp)
	{
		updated_remainder = ft_strdup(tmp + 1);
		if (!updated_remainder)
		{
			free(remainder);
			return (NULL);
		}
	}
	else
		updated_remainder = NULL;
	free(remainder);
	return (updated_remainder);
}

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*buffer;
	char		*line ;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	remainder = read_and_fill_buffer(fd, remainder, buffer);
	free(buffer);
	if (!remainder)
		return (NULL);
	line = extract_line(remainder);
	remainder = update_remainder(remainder);
	return (line);
}

/*int	main(void)
{
	int		fd;
	char	*line;

	fd = open("hola.txt", O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}*/
