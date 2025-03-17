/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 10:35:15 by aloiki            #+#    #+#             */
/*   Updated: 2025/03/17 23:04:23 by aloiki           ###   ########.fr       */
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
	return (philo);
}

void	*monitor_death(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	while (1)
	{
		i = 0;
		while (i < philo->number_of_philosophers)
		{
			pthread_mutex_lock(&philo->philosophers[i].mutex);
			if (time_milliseconds(philo->start_time)
				- philo->philosophers[i].last_meal
				> philo->time_to_die)
				death_or_not(philo, i);
			pthread_mutex_unlock(&philo->philosophers[i].mutex);
			i++;
		}
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo	*philo;
	int		i;

	philo = NULL;
	if (argc != 5 && argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
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
