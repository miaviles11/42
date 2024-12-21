/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: miaviles <miaviles@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 17:05:19 by miaviles          #+#    #+#             */
/*   Updated: 2024/12/20 17:23:13 by miaviles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	validate_pid(char *pid_str)
{
	int	i;

	i = 0;
	while (pid_str[i])
	{
		if (!ft_isdigit(pid_str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	send_char(int pid, unsigned char c, int delay)
{
	int	bit;

	bit = 8;
	while (bit--)
	{
		if ((c >> bit) & 1)
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		usleep(delay);
	}
}

static void	send_message(int pid, char *message)
{
	int	delay;
	int	factor;

	factor = 2;
	delay = 1000 + (ft_strlen(message) * factor);
	while (*message)
		send_char(pid, *message++, delay);
	send_char(pid, '\0', delay);
}

int	main(int argc, char **argv)
{
	int	pid;

	if (argc != 3 || !ft_strlen(argv[2]) || !validate_pid(argv[1]))
	{
		ft_putstr_fd("\x1b[31mERROR!\x1b[0m\n", 2);
		ft_putstr_fd("Formato: ./client [server_pid] \"Mensaje\"\n", 2);
		return (1);
	}
	pid = ft_atoi(argv[1]);
	ft_printf("Enviando mensaje al servidor (PID: %d)...\n", pid);
	send_message(pid, argv[2]);
	ft_printf("Mensaje enviado correctamente.\n");
	return (0);
}
