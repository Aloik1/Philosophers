/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aloiki <aloiki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 10:35:55 by aloiki            #+#    #+#             */
/*   Updated: 2025/05/04 19:18:29 by aloiki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>  // For pthread functions
# include <stdio.h>    // For printf
# include <stdlib.h>   // For malloc, free
# include <string.h>   // For memset
# include <sys/time.h> // For gettimeofday
# include <unistd.h>   // For write, usleep

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
	pthread_mutex_t	meal_counter_mutex;
	pthread_mutex_t	check_end_mutex;
	pthread_mutex_t	print_mutex;
}					t_philosophers;

typedef struct s_philo
{
	pthread_mutex_t	*forks;
	pthread_t		monitor_death;
	t_philosophers	*philosophers;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				check_end;
	int				number_of_times_each_philosopher_must_eat;
	int				number_of_philosophers;
	int				all_have_eaten;
}					t_philo;

// Main functions
void				*monitor_death(void *arg);
void				*routine(void *arg);
t_philo				*main_process(t_philo *philo);

// Utils
int					malloc_failed(void *ptr);
int					check_dead(t_philo *philo);
int					check_all_ate(t_philo *philo);
size_t				time_milliseconds(size_t start_time);
void				free_everything_and_exit(t_philo *philo);
int					check_stop(t_philo *philo);
void				left_right(t_philo *philo, t_philosophers *philosophers,
						size_t left_fork, size_t right_fork);
void				right_left(t_philo *philo, t_philosophers *philosophers,
						size_t left_fork, size_t right_fork);
void				eating(t_philo *philo, t_philosophers *philosophers,
						size_t left_fork, size_t right_fork);

#endif