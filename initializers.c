/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:41:11 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/01 15:41:28 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*philosopher_initializer(int ac, char **av, int i,
		pthread_mutex_t forks[])
{
	t_philo	*ph;
	int		id_c;

	id_c = ft_atoi(av[1]);
	ph = malloc(sizeof(t_philo));
	if (!ph)
	{
		printf("Failed to allocate memmory for philosopher");
		return (NULL);
	}
	ph->time_to_die = ft_atoi(av[2]);
	ph->time_to_eat = ft_atoi(av[3]);
	ph->time_to_sleep = ft_atoi(av[4]);
	ph->repeat = 0;
	if (ac == 6)
		ph->repeat = ft_atoi(av[5]);
	;
	ph->id = i + 1;
	ph->left_fork = &forks[i];
	if (id_c == 1)
		ph->right_fork = NULL;
	else
		ph->right_fork = &forks[(i + 1) % id_c];
	return (ph);
}

int	*die_flag_initialize(void)
{
	int	*die_f;

	die_f = malloc(sizeof(int));
	if (!die_f)
	{
		printf("Failed to allocate memmory for die_f flag");
		return (NULL);
	}
	*die_f = 0;
	return (die_f);
}

int	*stop_flag_initialize(void)
{
	int	*stop;

	stop = malloc(sizeof(int));
	if (!stop)
	{
		printf("Failed to allocate memmory for stop flag");
		return (NULL);
	}
	*stop = 0;
	return (stop);
}

void	forks_initilizer(pthread_mutex_t forks[], int n)
{
	int	j;

	j = 0;
	while (j < n)
	{
		pthread_mutex_init(&forks[j], NULL);
		j++;
	}
}

t_simulation	*simulation_initializer(t_philo *ph, int *die_f, int *stop,
		struct timeval start, int id_c, pthread_mutex_t *last_meal_mutex)
{	
	t_simulation	*sim;
	sim = malloc(sizeof(t_simulation));
	if (!sim)
	{
		printf("Failed to allocate memmory for sim");
		free(ph);
		return (NULL);
	}
	sim->stop = stop;
	sim->die_f = die_f;
	sim->start = start;
	sim->ph = ph;
	sim->total_ph = id_c;
	sim->last_meal_mutex = last_meal_mutex; 
	sim->last_meal_time = last_meal_initializer(sim, start);
	if (!sim->last_meal_time)
		return (NULL);
	sim->times = malloc(sizeof(int));
	if(!sim->times)
	{
		printf("Failed to allocate memmory for sim->times");
		free(ph);
		free(sim->times);
		return (NULL);
	}
	*sim->times = 1;
	return (sim);
}

void	simulation_add_mutexes(t_simulation *sim, pthread_mutex_t *log_mutex,
		pthread_mutex_t *die_mutex, pthread_mutex_t *stop_mutex)
{
	sim->log_mutex = log_mutex;
	sim->die_mutex = die_mutex;
    sim->stop_mutex = stop_mutex;
}

struct timeval	*last_meal_initializer(t_simulation *sim, struct timeval start)
{
	sim->last_meal_time = malloc(sizeof(struct timeval));
	if (!sim->last_meal_time)
	{
		printf("Failed to allocate memmory for sim->last_meal_time");
		free(sim->ph);
		free(sim);
		return (NULL);
	}
	pthread_mutex_lock(sim->last_meal_mutex);
	*(sim->last_meal_time) = start;
	pthread_mutex_unlock(sim->last_meal_mutex);
	return (sim->last_meal_time);
}