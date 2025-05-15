/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   task_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 21:27:38 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/10 21:27:41 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_takes_fork(t_simulation *sim)
{
	if (sim->total_ph %2 == 1 && sim->total_ph >=3)
	{
		int				rem3;	
		rem3 = sim->ph->id % 3;
		if (sim->total_ph % 2 == 1 && sim->total_ph >= 3)
			usleep(rem3 * 500);
		
		pthread_mutex_lock(sim->ph->left_fork);
		if (!try_log_action_or_die(sim, " has taken the LEFT fork",
				sim->ph->left_fork, NULL))
			return (0);
		pthread_mutex_lock(sim->ph->right_fork);
		if (!try_log_action_or_die(sim, " has taken the RIGHT fork",
				sim->ph->left_fork, sim->ph->right_fork))
			return (0);
	}
	else
	{
		if (sim->ph->id % 2 == 0) 
		{
			pthread_mutex_lock(sim->ph->left_fork);
			if (!try_log_action_or_die(sim, " has taken the LEFT fork",
					sim->ph->left_fork, NULL))
				return (0);
			pthread_mutex_lock(sim->ph->right_fork);
			if (!try_log_action_or_die(sim, " has taken the RIGHT fork",
					sim->ph->left_fork, sim->ph->right_fork))
				return (0);
		}
		else
		{
			pthread_mutex_lock(sim->ph->right_fork);
			if (!try_log_action_or_die(sim, " has taken the RIGHT fork",
					sim->ph->right_fork, NULL))
				return (0);
			pthread_mutex_lock(sim->ph->left_fork);
			if (!try_log_action_or_die(sim, " has taken the LEFT fork",
					sim->ph->left_fork, sim->ph->right_fork))
				return (0);
		}
	}	
	return (1);
}

int	philo_eats(t_simulation *sim)
{
	pthread_mutex_lock(sim->last_meal_mutex);
	gettimeofday(sim->last_meal_time, NULL);
	pthread_mutex_unlock(sim->last_meal_mutex);

	if (!try_log_action_or_die(sim, " is eating", NULL, NULL))
		return (0);
    sleep_in_ms(sim, sim->ph->time_to_eat);
	
	pthread_mutex_unlock(sim->ph->left_fork);
	pthread_mutex_unlock(sim->ph->right_fork);
	return (1);
}

int	philo_sleeps(t_simulation *sim)
{
	if (!try_log_action_or_die(sim, "is sleeping", sim->log_mutex, NULL))
		return (0);
	sleep_in_ms(sim, sim->ph->time_to_sleep);
	return (1);
}

int	philo_thinks(t_simulation *sim)
{
	if (!try_log_action_or_die(sim, "is thinking", sim->log_mutex, NULL))
		return (0);

	//pthread_mutex_lock(&sim->times_mutex);
	//(*sim->times)++;
	//pthread_mutex_unlock(&sim->times_mutex);

	return (1);
}
