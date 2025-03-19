/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:36:40 by aloiki            #+#    #+#             */
/*   Updated: 2025/03/19 19:06:46 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// static void	take_left_right_forks(t_philo *philo, t_philosophers *philosophers,
// 		size_t left_fork, size_t right_fork)
// {
// 	pthread_mutex_lock(&philo->forks[left_fork]);
// 	printf("Time: %lu, Number %d has taken left fork\n",
// 		time_milliseconds(philo->start_time), philosophers->id);
// 	pthread_mutex_lock(&philo->forks[right_fork]);
// 	printf("Time: %lu, Number %d has taken right fork\n",
// 		time_milliseconds(philo->start_time), philosophers->id);
// }

// static void	take_right_left_forks(t_philo *philo, t_philosophers *philosophers,
// 		size_t left_fork, size_t right_fork)
// {
// 	pthread_mutex_lock(&philo->forks[right_fork]);
// 	printf("Time: %lu, Number %d has taken right fork\n",
// 		time_milliseconds(philo->start_time), philosophers->id);
// 	pthread_mutex_lock(&philo->forks[left_fork]);
// 	printf("Time: %lu, Number %d has taken left fork\n",
// 		time_milliseconds(philo->start_time), philosophers->id);
// }

static void	eating(t_philo *philo, t_philosophers *philosophers,
		size_t left_fork, size_t right_fork)
{
	if (left_fork < right_fork)
	{
		pthread_mutex_lock(&philo->forks[left_fork]);
		printf("Time: %lu, Number %d has taken left fork\n",
			time_milliseconds(philo->start_time), philosophers->id);
		pthread_mutex_lock(&philo->forks[right_fork]);
		printf("Time: %lu, Number %d has taken right fork\n",
			time_milliseconds(philo->start_time), philosophers->id);
		//take_left_right_forks(philo, philosophers, left_fork, right_fork);
	}
	else
	{
		pthread_mutex_lock(&philo->forks[right_fork]);
		printf("Time: %lu, Number %d has taken right fork\n",
			time_milliseconds(philo->start_time), philosophers->id);
		pthread_mutex_lock(&philo->forks[left_fork]);
		printf("Time: %lu, Number %d has taken left fork\n",
			time_milliseconds(philo->start_time), philosophers->id);
		//take_right_left_forks(philo, philosophers, left_fork, right_fork);
	}
	pthread_mutex_lock(&philosophers->mutex);
	philosophers->last_meal = time_milliseconds(philo->start_time);
	philosophers->times_ate++;
	pthread_mutex_unlock(&philosophers->mutex);
	printf("Time: %lu, Number %d is eating\n",
		time_milliseconds(philo->start_time), philosophers->id);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->forks[left_fork]);
	pthread_mutex_unlock(&philo->forks[right_fork]);
	pthread_mutex_lock(&philosophers->mutex);
	philosophers->last_meal = time_milliseconds(philo->start_time);
	pthread_mutex_unlock(&philosophers->mutex);
}

static int	conditions(t_philo *philo, t_philosophers *philosophers,
		size_t left_fork, size_t right_fork)
{
	//pthread_mutex_lock(&philosophers->mutex);
	
	if ((philosophers->times_ate == philosophers->times_thought)
		&& (philosophers->times_ate == philosophers->times_slept))
	{	
		eating(philo, philosophers, left_fork, right_fork);
	}
	if (philosophers->times_slept < philosophers->times_ate
		|| philosophers->times_slept < philosophers->times_thought)
	{
		pthread_mutex_lock(&philosophers->mutex);
		philosophers->times_slept++;
		pthread_mutex_unlock(&philosophers->mutex);
		printf("Time: %lu, Number %d is sleeping\n",
			time_milliseconds(philo->start_time), philosophers->id);
		usleep(philo->time_to_sleep * 1000);
	}
	if (philosophers->times_thought < philosophers->times_ate
		&& philosophers->times_thought < philosophers->times_slept)
	{
		printf("Time: %lu, Number %d is thinking\n",
			time_milliseconds(philo->start_time), philosophers->id);
		pthread_mutex_lock(&philosophers->mutex);
		philosophers->times_thought++;
		pthread_mutex_unlock(&philosophers->mutex);
		usleep(1000);
	}
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
	while (1)
	{
		conditions(philo, philosophers, left_fork, right_fork);
		if (philo->number_of_times_each_philosopher_must_eat != -1
			&& philosophers->times_ate
			>= philo->number_of_times_each_philosopher_must_eat)
		{
			printf("%lu %d has eaten %d times\n",
				time_milliseconds(philo->start_time),
				philosophers->id,
				philo->number_of_times_each_philosopher_must_eat);
			break ;
		}
	}
	return (NULL);
}
