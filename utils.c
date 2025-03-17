/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:32:45 by aloiki            #+#    #+#             */
/*   Updated: 2025/03/17 23:11:34 by aloiki           ###   ########.fr       */
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

void	death_or_not(t_philo *philo, int i)
{
	int	eat_amount;

	eat_amount = philo->number_of_times_each_philosopher_must_eat;
	if (philo->number_of_times_each_philosopher_must_eat != -1
		&& philo->philosophers[i].times_ate >= eat_amount)
		return ;
	else
	{
		printf("Time: %zu, Number %d died\n",
			time_milliseconds(philo->start_time), philo->philosophers[i].id);
		pthread_mutex_unlock(&philo->philosophers[i].mutex);
		exit (0);
	}
}
