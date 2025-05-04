/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:32:45 by aloiki            #+#    #+#             */
/*   Updated: 2025/05/04 19:35:52 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_stop(t_philo *philo)
{
	int	stop;

	pthread_mutex_lock(&philo->philosophers->check_end_mutex);
	stop = philo->check_end;
	pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
	return (stop);
}

int	malloc_failed(void *ptr)
{
	if (!ptr)
	{
		printf("Error: Malloc failed\n");
		return (1);
	}
	return (0);
}

size_t	time_milliseconds(size_t start_time)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - start_time);
}

int	is_positive_integer(const char *str)
{
	int		i;
	long	num;

	i = 0;
	num = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		num = num * 10 + (str[i] - '0');
		if (num > INT_MAX)
			return (0);
		i++;
	}
	return (num > 0);
}

int	initial_checks(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf("Error: Required input:\n arg 1: ./philo\n arg 2: "
			"number_of_philosophers\n arg 3: "
			"time_to_die\n arg 4: "
			"time_to_eat time_to_sleep\n "
			"arg 5: [number_of_times_each_philosopher_must_eat]\n ");
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		if (!is_positive_integer(argv[i]))
		{
			printf("Error: Invalid input \"%s\". "
				"All arguments must be positive integers.\n",
				argv[i]);
			return (0);
		}
		i++;
	}
	return (1);
}
