/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 10:35:15 by aloiki            #+#    #+#             */
/*   Updated: 2025/05/04 19:25:32 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_philo	*init_philo(t_philo *philo, int argc, char **argv)
{
	int	i;

	philo = (t_philo *)malloc(sizeof(t_philo));
	if (malloc_failed(philo))
		return (NULL);
	philo->number_of_philosophers = (size_t)atoi(argv[1]);
	i = philo->number_of_philosophers;
	philo->time_to_die = (size_t)atoi(argv[2]);
	philo->time_to_eat = (size_t)atoi(argv[3]);
	philo->time_to_sleep = (size_t)atoi(argv[4]);
	philo->start_time = time_milliseconds(0);
	if (argc == 6)
		philo->number_of_times_each_philosopher_must_eat
			= (size_t)atoi(argv[5]);
	else
		philo->number_of_times_each_philosopher_must_eat = -1;
	philo->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * i);
	if (malloc_failed(philo->forks))
		return (NULL);
	philo->philosophers = (t_philosophers *)malloc(sizeof(t_philosophers) * i);
	if (malloc_failed(philo->philosophers))
		return (NULL);
	philo->check_end = 0;
	philo->all_have_eaten = 0;
	return (philo);
}

int	check_all_ate(t_philo *philo)
{
	pthread_mutex_lock(&philo->philosophers->meal_counter_mutex);
	if (philo->number_of_times_each_philosopher_must_eat != -1
		&& philo->all_have_eaten >= philo->number_of_philosophers)
	{
		pthread_mutex_lock(&philo->philosophers->print_mutex);
		printf("All philosophers have eaten %d times\n",
			philo->number_of_times_each_philosopher_must_eat);
		pthread_mutex_lock(&philo->philosophers->check_end_mutex);
		philo->check_end = 1;
		pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
		pthread_mutex_unlock(&philo->philosophers->print_mutex);
		pthread_mutex_unlock(&philo->philosophers->meal_counter_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->philosophers->meal_counter_mutex);
	return (0);
}

int	check_dead(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->number_of_philosophers)
	{
		pthread_mutex_lock(&philo->philosophers[i].mutex);
		if (time_milliseconds(philo->start_time)
			- philo->philosophers[i].last_meal
			> philo->time_to_die)
		{
			pthread_mutex_lock(&philo->philosophers->print_mutex);
			printf("Time: %zu, Number %d died\n",
				time_milliseconds(philo->start_time),
				philo->philosophers[i].id);
			pthread_mutex_lock(&philo->philosophers->check_end_mutex);
			philo->check_end = 1;
			pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
			pthread_mutex_unlock(&philo->philosophers->print_mutex);
			pthread_mutex_unlock(&philo->philosophers[i].mutex);
			return (1);
		}
		pthread_mutex_unlock(&philo->philosophers[i].mutex);
		i++;
	}
	return (0);
}

void	*monitor_death(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->philosophers->check_end_mutex);
		if (philo->check_end)
		{
			pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->philosophers->check_end_mutex);
		if (check_dead(philo) || check_all_ate(philo))
			break ;
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo	*philo;
	int		i;

	philo = NULL;
	if (!initial_checks(argc, argv))
		return (1);
	philo = init_philo(philo, argc, argv);
	if (!philo)
		return (1);
	philo = main_process(philo);
	i = 0;
	while (i < philo->number_of_philosophers)
	{
		pthread_mutex_destroy(&philo->forks[i]);
		i++;
	}
	free(philo->forks);
	free(philo->philosophers);
	free(philo);
	return (0);
}
