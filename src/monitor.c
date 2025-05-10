/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 19:01:35 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/10 19:01:41 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor_threads(void *args)
{
	t_simulation	**sims;
	long			time_to_die;
	int				id_c;
	struct timeval	now;
	long			elapsed;
	int				i;
	int				all_philos_finished_eating;
	int				someone_died;
	long			timestamp;
	int				philos_done_this_round;

	sims = (t_simulation **)args;
	time_to_die = sims[0]->ph->time_to_die;
	id_c = sims[0]->total_ph;
	all_philos_finished_eating = 0;
	someone_died = 0;
	while (!someone_died && !all_philos_finished_eating)
	{
		i = 0;
		philos_done_this_round = 0;
		while (i < id_c && !someone_died && !all_philos_finished_eating)
		{
			gettimeofday(&now, NULL);
			pthread_mutex_lock(sims[i]->last_meal_mutex);
			if (!sims[i]->last_meal_time)
			{
				pthread_mutex_unlock(sims[i]->last_meal_mutex);
				i++;
				continue ;
			}
			elapsed = (now.tv_sec - sims[i]->last_meal_time->tv_sec) * 1000
				+ (now.tv_usec - sims[i]->last_meal_time->tv_usec) / 1000;
			pthread_mutex_unlock(sims[i]->last_meal_mutex);
			if (elapsed >= time_to_die)
			{
				pthread_mutex_lock(sims[i]->die_mutex);
				if (!(*(sims[i]->die_f)))
				{
					*(sims[i]->die_f) = 1;
					pthread_mutex_unlock(sims[i]->die_mutex);
					timestamp = (now.tv_sec - sims[i]->start.tv_sec) * 1000
						+ (now.tv_usec - sims[i]->start.tv_usec) / 1000;
					print_log(sims[i], " died3", timestamp);
					pthread_mutex_lock(sims[0]->stop_mutex);
					*(sims[0]->stop) = 1;
					pthread_mutex_unlock(sims[0]->stop_mutex);
					someone_died = 1;
				}
				else
				{
					pthread_mutex_unlock(sims[i]->die_mutex);
				}
				break ;
			}
			if (sims[i]->ph->repeat > 0
				&& (*(sims[i]->times) >= sims[i]->ph->repeat))
			{
				philos_done_this_round++;
			}
			i++;
		}
		if (philos_done_this_round == id_c)
		{
			pthread_mutex_lock(sims[0]->stop_mutex);
			*(sims[0]->stop) = 1;
			all_philos_finished_eating = 1;
			pthread_mutex_unlock(sims[0]->stop_mutex);
		}
		if (!someone_died && !all_philos_finished_eating)
			usleep(1000);
	}
	return (NULL);
}
