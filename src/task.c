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
	try_log_action_or_die(sim, "has taken the LEFT fork", NULL, NULL);
	while (1)
	{
		pthread_mutex_lock(sim->stop_mutex);
		if (*(sim->stop))
		{
			pthread_mutex_unlock(sim->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(sim->stop_mutex);
		usleep(1000);
	}
	return ;
}

int	routine(t_simulation *sim)
{
	pthread_mutex_lock(sim->die_mutex);
	if (*(sim->die_f) || *(sim->stop))
	{
		pthread_mutex_unlock(sim->die_mutex);
		return (0);
	}
	pthread_mutex_unlock(sim->die_mutex);
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
	return (1);
}

void	*task(void *args)
{
	t_simulation	*sim;
	int				i;
	int				rem3;

	sim = (t_simulation *)args;
	rem3 = sim->ph->id % 3;
	if (sim->total_ph % 2 == 1 && sim->total_ph >= 3)
		usleep(rem3 * 1000);
	i = 0;
	while (((sim->ph->repeat == 0) || i < sim->ph->repeat))
	{
		*(sim->times) = i + 1;
		if (!routine(sim))
			break ;
		i++;
	}
	return (NULL);
}
