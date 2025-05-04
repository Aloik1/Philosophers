/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:32:45 by aloiki            #+#    #+#             */
/*   Updated: 2025/05/04 19:18:32 by aloiki           ###   ########.fr       */
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

void	free_everything_and_exit(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->number_of_philosophers)
	{
		pthread_mutex_destroy(&philo->forks[i]);
		i++;
	}
	free(philo->forks);
	free(philo->philosophers);
	free(philo);
	exit(0);
}
