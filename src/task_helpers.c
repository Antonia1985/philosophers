/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:47:00 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/01 15:47:10 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_after_death(pthread_mutex_t *mutex1, pthread_mutex_t *mutex2)
{
	if (mutex1)
		pthread_mutex_unlock(mutex1);
	if (mutex2)
		pthread_mutex_unlock(mutex2);
}

void	print_log(t_simulation *sim, const char *msg, long timestamp)
{
	pthread_mutex_lock(sim->log_mutex);
	printf("%li %i%s\n", timestamp, sim->ph->id, msg);
	pthread_mutex_unlock(sim->log_mutex);
}

int	try_log_action_or_die(t_simulation *sim, const char *msg,
		pthread_mutex_t *mutex1, pthread_mutex_t *mutex2)
{
	long			elapsed;
	long			timestamp;
	struct timeval	now;

	gettimeofday(&now, NULL);
	elapsed = get_elapsed(now, sim->last_meal_mutex, sim->last_meal_time);
	if (!check_flags_to_exit(sim->die_f, sim->stop, sim->die_mutex,
			sim->stop_mutex))
	{
		clean_after_death(mutex1, mutex2);
		return (0);
	}
	timestamp = get_timestamp(now, &sim->start);
	if (elapsed >= sim->ph->time_to_die)
	{
		update_death_flags_and_log(sim, now);
		clean_after_death(mutex1, mutex2);
		return (0);
	}
	print_log(sim, msg, timestamp);
	return (1);
}
