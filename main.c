/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 10:35:15 by aloiki            #+#    #+#             */
/*   Updated: 2025/03/17 15:25:24 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t time_milliseconds(size_t start_time)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000 + tv.tv_usec / 1000) - start_time);
}

void   *monitor_death(void *arg)
{
	int i;
	t_philo *philo;
	philo = (t_philo *)arg;

	while (1)
	{
		i = 0;
		while (i < philo->number_of_philosophers)
		{
			if (time_milliseconds(philo->start_time) - philo->philosophers[i].last_meal > philo->time_to_die)
			{
				if (philo->number_of_times_each_philosopher_must_eat != -1 && philo->philosophers[i].times_ate >= philo->number_of_times_each_philosopher_must_eat)
				{
					i++;
					continue ;
				}
				else
				{
					printf("Time: %zu, Number %d died\n", time_milliseconds(philo->start_time), philo->philosophers[i].id);
					exit (0);
				}
			}
			i++;
		}
	}
	return NULL;
}

void	*routine(void *arg)
{
	t_philosophers *philosophers;
	t_philo *philo;
	size_t	left_fork;
	size_t	right_fork;
	
	philosophers = (t_philosophers *)arg;
	philo = philosophers->shared_philo;
	
	left_fork = philosophers->id - 1;
	right_fork = philosophers->id % philo->number_of_philosophers;
	while (1)
	{
		// Thinking
		if ((philosophers->times_ate == philosophers->times_thought) && (philosophers->times_ate == philosophers->times_slept))
		{
			if (left_fork < right_fork)
			{
				pthread_mutex_lock(&philo->forks[left_fork]);
				pthread_mutex_lock(&philo->forks[right_fork]);
			}
			else
			{
				pthread_mutex_lock(&philo->forks[right_fork]);
				pthread_mutex_lock(&philo->forks[left_fork]);
			}
				// Eating
			pthread_mutex_lock(&philosophers->mutex);
			philosophers->last_meal = time_milliseconds(philo->start_time); // Update last meal time
			philosophers->times_ate++;
			pthread_mutex_unlock(&philosophers->mutex);
			printf("Time: %lu, Number %d is eating\n", time_milliseconds(philo->start_time), philosophers->id);
			usleep(philo->time_to_eat * 1000);
			pthread_mutex_unlock(&philo->forks[left_fork]);
			pthread_mutex_unlock(&philo->forks[right_fork]);
		}
		// Eating
		if (philosophers->times_thought < philosophers->times_ate || philosophers->times_thought < philosophers->times_slept)
		{
			printf("Time: %lu, Number %d is thinking\n", time_milliseconds(philo->start_time), philosophers->id);
			pthread_mutex_lock(&philosophers->mutex);
			philosophers->times_thought++;
			pthread_mutex_unlock(&philosophers->mutex);
			usleep(200);
		}
		if (philosophers->times_slept < philosophers->times_ate && philosophers->times_slept < philosophers->times_thought)
		{
			pthread_mutex_lock(&philosophers->mutex);
			philosophers->times_slept++;
			pthread_mutex_unlock(&philosophers->mutex);
			printf("Time: %lu, Number %d is sleeping\n", time_milliseconds(philo->start_time), philosophers->id);
			usleep(philo->time_to_sleep * 1000);
		}
		if (philo->number_of_times_each_philosopher_must_eat != -1 && philosophers->times_ate >= philo->number_of_times_each_philosopher_must_eat)
		{
			printf("%lu %d has eaten %d times\n", time_milliseconds(philo->start_time), philosophers->id, philo->number_of_times_each_philosopher_must_eat);
			break ;
		}
	}
	return NULL;
}


int	main(int argc, char **argv)
{
	t_philo	*philo;
	int	i;

	if (argc != 5 && argc != 6) // 6th argument is optional
	{
		printf("Error: Wrong number of arguments\n");
		return (1);
	}
	philo = (t_philo *)malloc(sizeof(t_philo)); // Allocate memory and init philo struct
	if (!philo)
	{
		printf("Error: Malloc failed\n");
		return (1);
	}
	philo->number_of_philosophers = (size_t)atoi(argv[1]);
	philo->time_to_die = (size_t)atoi(argv[2]);
	philo->time_to_eat = (size_t)atoi(argv[3]);
	philo->time_to_sleep = (size_t)atoi(argv[4]);
	philo->start_time = time_milliseconds(0);
	if (argc == 6)
		philo->number_of_times_each_philosopher_must_eat = (size_t)atoi(argv[5]);
	else
		philo->number_of_times_each_philosopher_must_eat = -1;
	philo->philosophers = (t_philosophers *)malloc(sizeof(t_philosophers) * philo->number_of_philosophers);
	if (!philo->philosophers)
	{
		printf("Error: Malloc failed\n");
		return (1);
	}
	// Allocate memory for forks (mutex)
	philo->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * philo->number_of_philosophers);
	if (!philo->forks)
	{
		printf("Error: Malloc failed\n");
		return (1);
	}
	// Fill the forks array with mutexes and create threads
	i = 0;
	while (i < philo->number_of_philosophers)
	{
		philo->philosophers[i].id = i + 1;
		philo->philosophers[i].last_meal = time_milliseconds(philo->start_time);
		philo->philosophers[i].shared_philo = philo;
		philo->philosophers[i].times_ate = 0;
		philo->philosophers[i].times_slept = 0;
		philo->philosophers[i].times_thought = 0;
		
		pthread_mutex_init(&philo->forks[i], NULL);
		pthread_mutex_init(&philo->philosophers[i].mutex, NULL);
		i++;
	}
	i = 0;
	while (i < philo->number_of_philosophers)
	{
		if (pthread_create(&philo->philosophers[i].thread, NULL, &routine, &philo->philosophers[i]))
		{
			printf("Error: pthread_create failed\n");
			return (1);
		}
		i++;
	}
	if (pthread_create(&philo->monitor_death, NULL, &monitor_death, philo))
	{
		printf("Error: pthread_create failed\n");
		return (1);
	}
	i = 0;
	while (i < philo->number_of_philosophers)
	{
		pthread_join(philo->philosophers[i].thread, NULL);
		i++;
	}
	// Free memory
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
