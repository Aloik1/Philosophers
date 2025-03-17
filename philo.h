/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 10:35:55 by aloiki            #+#    #+#             */
/*   Updated: 2025/03/17 14:49:32 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <string.h>     // For memset
#include <stdio.h>      // For printf
#include <stdlib.h>     // For malloc, free
#include <unistd.h>     // For write, usleep
#include <sys/time.h>   // For gettimeofday
#include <pthread.h>    // For pthread functions

typedef struct s_philosophers
{
	int id;
	size_t last_meal;
	pthread_t thread;
	struct s_philo *shared_philo;
	int	times_ate;
	int	times_slept;
	int	times_thought;
	pthread_mutex_t mutex;
} t_philosophers; 

typedef struct s_philo
{
	int number_of_philosophers;
	size_t time_to_die;
	size_t time_to_eat;
	size_t time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
	pthread_t monitor_death;
	pthread_mutex_t *forks;
	t_philosophers *philosophers;
	size_t start_time; // Add start_time field
} t_philo;

#endif