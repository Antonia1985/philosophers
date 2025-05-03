/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:41:40 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/01 15:41:49 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_threads(pthread_t threads[], int id_c)
{
	int	i;

	i = 0;
	while (i < id_c)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

void	destroy_mutexs(pthread_mutex_t forks[], int id_c)
{
	int	i;

	i = 0;
	while (i < id_c)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void	initialize_mutexes(pthread_mutex_t forks[], pthread_mutex_t *log_mutex,
		pthread_mutex_t *die_mutex, int id_c)
{
	pthread_mutex_init(log_mutex, NULL);
	pthread_mutex_init(die_mutex, NULL);
	forks_initilizer(forks, id_c);
}

void	clean_before_exit(pthread_mutex_t *log_mutex,
		pthread_mutex_t *die_mutex, int *die_f)
{
	free(die_f);
	pthread_mutex_destroy(log_mutex);
	pthread_mutex_destroy(die_mutex);
}

int	main(int ac, char **av)
{
	if (ac > 6 || ac < 5)
	{
		printf("Usage ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	int i;
	int id_c;
	struct timeval start;
	int *die_f;

	i = 0;
	id_c = ft_atoi(av[1]);
	gettimeofday(&start, NULL);
	pthread_mutex_t forks[id_c];
	pthread_mutex_t log_mutex;
	pthread_mutex_t die_mutex;
	pthread_t threads[id_c];
	// pthread_t monitor;
	die_f = die_flag_initialize();
	if (!die_f)
		return (1);
	initialize_mutexes(forks, &log_mutex, &die_mutex, id_c);
	while (i < id_c)
	{
		t_philo *ph = philosopher_initializer(ac, av, i, forks);
		if (!ph)
			return (1);
		t_simulation *sim = simulation_initializer(ph, die_f, start, id_c);
		if (!sim)
			return (1);
		simulation_add_mutexes(sim, &log_mutex, &die_mutex);
		pthread_create(&threads[i], NULL, task, sim);
		usleep(100);
		i++;
	}
	// pthread_create(&monitor, NULL, monitor_death, NULL);
	join_threads(threads, id_c);
	destroy_mutexs(forks, id_c);
	clean_before_exit(&log_mutex, &die_mutex, die_f);

	return (0);
}
