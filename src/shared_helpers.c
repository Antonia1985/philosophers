/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:50:57 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/15 16:51:00 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_elapsed(struct timeval now, pthread_mutex_t *last_meal_mutex,
		struct timeval *last_meal_time)
{
	long	elapsed;

	pthread_mutex_lock(last_meal_mutex);
	elapsed = (now.tv_sec - last_meal_time->tv_sec) * 1000 + (now.tv_usec
			- last_meal_time->tv_usec) / 1000;
	pthread_mutex_unlock(last_meal_mutex);
	return (elapsed);
}

long	get_timestamp(struct timeval now, struct timeval *start)
{
	long	timestamp;

	timestamp = (now.tv_sec - start->tv_sec) * 1000 + (now.tv_usec
			- start->tv_usec) / 1000;
	return (timestamp);
}

int	check_flags_to_exit(int *die_f, int *stop, pthread_mutex_t *die_mutex,
		pthread_mutex_t *stop_mutex)
{
	pthread_mutex_lock(die_mutex);
	if (*(die_f))
	{
		pthread_mutex_unlock(die_mutex);
		return (0);
	}
	pthread_mutex_unlock(die_mutex);
	pthread_mutex_lock(stop_mutex);
	if (*(stop))
	{
		pthread_mutex_unlock(stop_mutex);
		return (0);
	}
	pthread_mutex_unlock(stop_mutex);
	return (1);
}
