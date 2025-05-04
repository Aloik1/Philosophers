/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:14:39 by aloiki            #+#    #+#             */
/*   Updated: 2025/05/04 19:17:50 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	left_right(t_philo *philo, t_philosophers *philosophers,
		size_t left_fork, size_t right_fork)
{
	pthread_mutex_lock(&philo->forks[left_fork]);
	pthread_mutex_lock(&philo->philosophers->check_end_mutex);
	if (!philo->check_end)
		printf("Time: %lu, Number %d has taken a fork\n",
			time_milliseconds(philo->start_time), philosophers->id);
	pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
	pthread_mutex_lock(&philo->forks[right_fork]);
	pthread_mutex_lock(&philo->philosophers->check_end_mutex);
	if (!philo->check_end)
		printf("Time: %lu, Number %d has taken a fork\n",
			time_milliseconds(philo->start_time), philosophers->id);
	pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
	return ;
}

void	right_left(t_philo *philo, t_philosophers *philosophers,
		size_t left_fork, size_t right_fork)
{
	pthread_mutex_lock(&philo->forks[right_fork]);
	pthread_mutex_lock(&philo->philosophers->check_end_mutex);
	if (!philo->check_end)
		printf("Time: %lu, Number %d has taken a fork\n",
			time_milliseconds(philo->start_time), philosophers->id);
	pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
	pthread_mutex_lock(&philo->forks[left_fork]);
	pthread_mutex_lock(&philo->philosophers->check_end_mutex);
	if (!philo->check_end)
		printf("Time: %lu, Number %d has taken a fork\n",
			time_milliseconds(philo->start_time), philosophers->id);
	pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
}

void	eating(t_philo *philo, t_philosophers *philosophers, size_t left_fork,
		size_t right_fork)
{
	pthread_mutex_lock(&philosophers->mutex);
	philosophers->last_meal = time_milliseconds(philo->start_time);
	philosophers->times_ate++;
	pthread_mutex_unlock(&philosophers->mutex);
	if (philo->number_of_times_each_philosopher_must_eat != -1
		&& philosophers->times_ate
		== philo->number_of_times_each_philosopher_must_eat)
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
	return ;
}
