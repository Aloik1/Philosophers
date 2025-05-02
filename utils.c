/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:32:45 by aloiki            #+#    #+#             */
/*   Updated: 2025/03/19 20:24:41 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	death_or_not(t_philo *philo, int i)
{
	int	eat_amount;

	//pthread_mutex_lock(&philo->philosophers[i].mutex);
	eat_amount = philo->number_of_times_each_philosopher_must_eat;
	if (eat_amount != -1 && philo->philosophers[i].times_ate >= eat_amount)
	{
		//pthread_mutex_unlock(&philo->philosophers[i].mutex);
		return (0);
	}
	else
	{
		printf("Time: %zu, Number %d died\n",
			time_milliseconds(philo->start_time), philo->philosophers[i].id);
		//pthread_mutex_unlock(&philo->philosophers[i].mutex);
		return (1);
	}
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
