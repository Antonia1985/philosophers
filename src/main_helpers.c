/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 19:07:50 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/10 19:07:54 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_threads(pthread_t threads[], int id_c)
{
	int	i;

	i = 0;
	while (i <= id_c)
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

void	initialize_mutexes(pthread_mutex_t *log_mutex,
		pthread_mutex_t *die_mutex, pthread_mutex_t *last_meal_mutex,
		pthread_mutex_t *stop_mutex)
{
	pthread_mutex_init(log_mutex, NULL);
	pthread_mutex_init(die_mutex, NULL);
	pthread_mutex_init(last_meal_mutex, NULL);
	pthread_mutex_init(stop_mutex, NULL);
}

void	clean_mutexs_before_exit(pthread_mutex_t *log_mutex,
		pthread_mutex_t *die_mutex, pthread_mutex_t *last_meal_mutex,
		pthread_mutex_t *stop_mutex)
{
	pthread_mutex_destroy(log_mutex);
	pthread_mutex_destroy(die_mutex);
	pthread_mutex_destroy(last_meal_mutex);
	pthread_mutex_destroy(stop_mutex);
}

void	clean_flags_before_exit(int *die_f, int *stop)
{
	if (die_f != NULL)
		free(die_f);
	if (stop != NULL)
		free(stop);
}
