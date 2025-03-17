/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 10:35:55 by aloiki            #+#    #+#             */
/*   Updated: 2025/03/17 15:34:34 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>     // For memset
# include <stdio.h>      // For printf
# include <stdlib.h>     // For malloc, free
# include <unistd.h>     // For write, usleep
# include <sys/time.h>   // For gettimeofday
# include <pthread.h>    // For pthread functions

typedef struct s_philosophers
{
	struct s_philo	*shared_philo;
	pthread_t		thread;
	size_t			last_meal;
	int				id;
	int				times_ate;
	int				times_slept;
	int				times_thought;
	pthread_mutex_t	mutex;
}	t_philosophers;

typedef struct s_philo
{
	pthread_mutex_t	*forks;
	pthread_t		monitor_death;
	t_philosophers	*philosophers;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				number_of_times_each_philosopher_must_eat;
	int				number_of_philosophers;
}	t_philo;

#endif