/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:36:40 by aloiki            #+#    #+#             */
/*   Updated: 2025/05/04 19:17:27 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	forking(t_philo *philo, t_philosophers *philosophers,
		size_t left_fork, size_t right_fork)
{
	if (left_fork < right_fork)
	{
		if (check_stop(philo))
			return ;
		left_right(philo, philosophers, left_fork, right_fork);
	}
	else
	{
		if (check_stop(philo))
			return ;
		right_left(philo, philosophers, left_fork, right_fork);
	}
	if (check_stop(philo))
	{
		pthread_mutex_unlock(&philo->forks[left_fork]);
		pthread_mutex_unlock(&philo->forks[right_fork]);
		return ;
	}
	eating(philo, philosophers, left_fork, right_fork);
}

static void	thinking(t_philo *philo, t_philosophers *philosophers)
{
	if (!check_stop(philo))
		printf("Time: %lu, Number %d is thinking\n",
			time_milliseconds(philo->start_time), philosophers->id);
	pthread_mutex_lock(&philosophers->mutex);
	philosophers->times_thought++;
	pthread_mutex_unlock(&philosophers->mutex);
	usleep(1000);
	return ;
}

static int	conditions(t_philo *philo, t_philosophers *philosophers,
		size_t left_fork, size_t right_fork)
{
	if (check_stop(philo))
		return (1);
	if ((philosophers->times_ate == philosophers->times_thought)
		&& (philosophers->times_ate == philosophers->times_slept))
		forking(philo, philosophers, left_fork, right_fork);
	else if (philosophers->times_slept < philosophers->times_ate
		|| philosophers->times_slept < philosophers->times_thought)
	{
		pthread_mutex_lock(&philosophers->mutex);
		philosophers->times_slept++;
		pthread_mutex_unlock(&philosophers->mutex);
		if (!check_stop(philo))
			printf("Time: %lu, Number %d is sleeping\n",
				time_milliseconds(philo->start_time), philosophers->id);
		usleep(philo->time_to_sleep * 1000);
	}
	else if (philosophers->times_thought < philosophers->times_ate
		&& philosophers->times_thought < philosophers->times_slept)
		thinking(philo, philosophers);
	return (0);
}

void	*routine(void *arg)
{
	t_philosophers	*philosophers;
	t_philo			*philo;
	size_t			left_fork;
	size_t			right_fork;

	philosophers = (t_philosophers *)arg;
	philo = philosophers->shared_philo;
	left_fork = philosophers->id - 1;
	right_fork = philosophers->id % philo->number_of_philosophers;
	if (philo->number_of_philosophers == 1)
	{
		if (check_stop(philo))
			return (NULL);
		printf("Time: %lu, Number %d has taken left fork\n",
			time_milliseconds(philo->start_time), philosophers->id);
		usleep(philo->time_to_die * 1000);
	}
	while (!check_stop(philo))
	{
		conditions(philo, philosophers, left_fork, right_fork);
	}
	return (NULL);
}
