/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:36:40 by aloiki            #+#    #+#             */
/*   Updated: 2025/05/04 18:31:30 by aloiki           ###   ########.fr       */
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

static void	eating(t_philo *philo, t_philosophers *philosophers,
		size_t left_fork, size_t right_fork)
{
	if (left_fork < right_fork)
	{
		if (check_stop(philo))
			return ;
		pthread_mutex_lock(&philo->forks[left_fork]);
		pthread_mutex_lock(&philo->philosophers->check_end_mutex);
		if (!philo->check_end)
			printf("Time: %lu, Number %d has taken left fork\n",
				time_milliseconds(philo->start_time), philosophers->id);
		pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
		pthread_mutex_lock(&philo->forks[right_fork]);
		pthread_mutex_lock(&philo->philosophers->check_end_mutex);
		if (!philo->check_end)
			printf("Time: %lu, Number %d has taken right fork\n",
				time_milliseconds(philo->start_time), philosophers->id);
		pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
	}
	else
	{
		if (check_stop(philo))
			return ;
		pthread_mutex_lock(&philo->forks[right_fork]);
		pthread_mutex_lock(&philo->philosophers->check_end_mutex);		
		if (!philo->check_end)
			printf("Time: %lu, Number %d has taken right fork\n",
				time_milliseconds(philo->start_time), philosophers->id);
		pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
		pthread_mutex_lock(&philo->forks[left_fork]);
		pthread_mutex_lock(&philo->philosophers->check_end_mutex);
		if (!philo->check_end)
			printf("Time: %lu, Number %d has taken left fork\n",
				time_milliseconds(philo->start_time), philosophers->id);
		pthread_mutex_unlock(&philo->philosophers->check_end_mutex);}
	if (check_stop(philo))
	{
		pthread_mutex_unlock(&philo->forks[left_fork]);
		pthread_mutex_unlock(&philo->forks[right_fork]);
		return ;
	}
	pthread_mutex_lock(&philosophers->mutex);
	philosophers->last_meal = time_milliseconds(philo->start_time);
	philosophers->times_ate++;
	pthread_mutex_unlock(&philosophers->mutex);
	if (philo->number_of_times_each_philosopher_must_eat != -1
		&& philosophers->times_ate == philo->number_of_times_each_philosopher_must_eat)
	{
		pthread_mutex_lock(&philo->philosophers->meal_counter_mutex);
		philo->all_have_eaten++;
		pthread_mutex_unlock(&philo->philosophers->meal_counter_mutex);
	}
	pthread_mutex_lock(&philosophers->print_mutex);
	printf("Time: %lu, Number %d is eating\n",
		time_milliseconds(philo->start_time), philosophers->id);
	pthread_mutex_unlock(&philosophers->print_mutex);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(&philo->forks[left_fork]);
	pthread_mutex_unlock(&philo->forks[right_fork]);
}

static int	conditions(t_philo *philo, t_philosophers *philosophers,
		size_t left_fork, size_t right_fork)
{
	if (check_stop(philo))
		return (1);
	if ((philosophers->times_ate == philosophers->times_thought)
		&& (philosophers->times_ate == philosophers->times_slept))
	{
		eating(philo, philosophers, left_fork, right_fork);
		return (1);
	}
	if (philosophers->times_slept < philosophers->times_ate
		|| philosophers->times_slept < philosophers->times_thought)
	{
		pthread_mutex_lock(&philosophers->mutex);
		philosophers->times_slept++;
		pthread_mutex_unlock(&philosophers->mutex);
		if (!check_stop(philo))
			printf("Time: %lu, Number %d is sleeping\n",
				time_milliseconds(philo->start_time), philosophers->id);
		usleep(philo->time_to_sleep * 1000);
		return (1);
	}
	if (philosophers->times_thought < philosophers->times_ate
		&& philosophers->times_thought < philosophers->times_slept)
	{
		if (!check_stop(philo))
			printf("Time: %lu, Number %d is thinking\n",
				time_milliseconds(philo->start_time), philosophers->id);
		pthread_mutex_lock(&philosophers->mutex);
		philosophers->times_thought++;
		pthread_mutex_unlock(&philosophers->mutex);
		usleep(1000);
		return (1);
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
