/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 19:56:46 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/10 19:56:51 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	sleep_in_ms(t_simulation *sim, int ms)
{
	long			start_of_waiting;
	long			current;
	long			elapsed;
	struct timeval	now;
	long			timestamp;

	gettimeofday(&now, NULL);
	start_of_waiting = (now.tv_sec - sim->start.tv_sec) * 1000 + (now.tv_usec
			- sim->start.tv_usec) / 1000;
	current = start_of_waiting;
	while (ms > (current - start_of_waiting))
	{
		usleep(250);
		pthread_mutex_lock(sim->die_mutex);
		pthread_mutex_lock(sim->stop_mutex);
		if (*(sim->die_f) || *(sim->stop))
		{
			pthread_mutex_unlock(sim->die_mutex);
			pthread_mutex_unlock(sim->stop_mutex);
			return (0);
		}
		pthread_mutex_unlock(sim->die_mutex);
		pthread_mutex_unlock(sim->stop_mutex);
		gettimeofday(&now, NULL);
		pthread_mutex_lock(sim->last_meal_mutex);
		elapsed = (now.tv_sec - sim->last_meal_time->tv_sec) * 1000
			+ (now.tv_usec - sim->last_meal_time->tv_usec) / 1000;
		pthread_mutex_unlock(sim->last_meal_mutex);
		if (elapsed >= sim->ph->time_to_die)
		{
			timestamp = (now.tv_sec - sim->start.tv_sec) * 1000 + (now.tv_usec
					- sim->start.tv_usec) / 1000;
			pthread_mutex_lock(sim->die_mutex);
			*(sim->die_f) = 1;
			pthread_mutex_unlock(sim->die_mutex);
			pthread_mutex_lock(sim->stop_mutex);
			*(sim->stop) = 1;
			pthread_mutex_unlock(sim->stop_mutex);
			print_log(sim, " died1", timestamp);
			return (0);
		}
		current = (now.tv_sec - sim->start.tv_sec) * 1000 + (now.tv_usec
				- sim->start.tv_usec) / 1000;
	}
	return (1);
}
