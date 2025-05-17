/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:46:18 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/01 15:46:25 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo(t_simulation *sim)
{
	try_log_action_or_die(sim, "has taken a fork", NULL, NULL);
	while (1)
	{
		pthread_mutex_lock(sim->die_mutex);
		if (*(sim->die_f))
		{
			pthread_mutex_unlock(sim->die_mutex);
			break ;
		}
		pthread_mutex_unlock(sim->die_mutex);
		usleep(1000);
	}
}

int	check_stop(pthread_mutex_t *stop_mutex, int *stop)
{
	int	stoped;

	stoped = 0;
	pthread_mutex_lock(stop_mutex);
	if (*(stop))
		stoped = 1;
	pthread_mutex_unlock(stop_mutex);
	return (stoped);
}

int	check_die_f(pthread_mutex_t *die_mutex, int *die_f)
{
	int	died;

	died = 0;
	pthread_mutex_lock(die_mutex);
	if (*(die_f))
		died = 1;
	pthread_mutex_unlock(die_mutex);
	return (died);
}

int	routine(t_simulation *sim)
{
	if (check_stop(sim->stop_mutex, sim->stop) || check_die_f(sim->die_mutex,
			sim->die_f))
		return (0);
	if (sim->total_ph == 1)
	{
		one_philo(sim);
		return (0);
	}
	if (!philo_takes_fork(sim))
		return (0);
	if (!philo_eats(sim))
		return (0);
	if (!philo_sleeps(sim))
		return (0);
	if (!philo_thinks(sim))
		return (0);
	pthread_mutex_lock(&sim->times_mutex);
	(*sim->times)++;
	pthread_mutex_unlock(&sim->times_mutex);
	return (1);
}

void	*task(void *args)
{
	t_simulation	*sim;
	int				rem3;

	sim = (t_simulation *)args;
	rem3 = sim->ph->id % 3;
	if (sim->total_ph % 2 == 1 && sim->total_ph >= 3)
		usleep(rem3 * 500);
	while ((sim->ph->repeat == 0) || *sim->times < sim->ph->repeat)
	{
		if (!routine(sim))
			break ;
	}
	return (NULL);
}
