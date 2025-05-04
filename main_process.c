/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 22:12:03 by aloiki            #+#    #+#             */
/*   Updated: 2025/05/04 15:23:09 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_philo	*init_philosophers(t_philo *philo, int i)
{
	philo->philosophers[i].id = i + 1;
	philo->philosophers[i].last_meal = time_milliseconds(philo->start_time);
	philo->philosophers[i].shared_philo = philo;
	philo->philosophers[i].times_ate = 0;
	philo->philosophers[i].times_slept = 0;
	philo->philosophers[i].times_thought = 0;
	return (philo);
}

static t_philo	*create_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->number_of_philosophers)
	{
		if (pthread_create(&philo->philosophers[i].thread,
				NULL, &routine, &philo->philosophers[i]))
		{
			printf("Error: pthread_create failed\n");
			return (philo);
		}
		i++;
	}
	if (pthread_create(&philo->monitor_death, NULL, &monitor_death, philo))
	{
		printf("Error: pthread_create failed\n");
		return (philo);
	}
	return (philo);
}

static t_philo	*init_mutex(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->number_of_philosophers)
	{
		philo = init_philosophers(philo, i);
		pthread_mutex_init(&philo->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&philo->philosophers->mutex, NULL);
	pthread_mutex_init(&philo->philosophers->meal_counter_mutex, NULL);
	pthread_mutex_init(&philo->philosophers->check_end_mutex, NULL);
	pthread_mutex_init(&philo->philosophers->print_mutex, NULL);
	return (philo);
}

t_philo	*main_process(t_philo *philo)
{
	int	i;

	philo = init_mutex(philo);
	philo = create_threads(philo);
	i = 0;
	while (i < philo->number_of_philosophers)
	{
		pthread_join(philo->philosophers[i].thread, NULL);
		i++;
	}
	pthread_join(philo->monitor_death, NULL);
	return (philo);
}
