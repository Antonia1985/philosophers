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

int	sleep_in_ms(t_simulation *sim, int ms)
{
	long			start_of_waiting;
	long			current;
    long			elapsed;
	struct timeval	now;

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

		if (elapsed > sim->ph->time_to_die)
		{
			long timestamp = (now.tv_sec - sim->start.tv_sec) * 1000
					+ (now.tv_usec - sim->start.tv_usec) / 1000;

			pthread_mutex_lock(sim->die_mutex);			
			*(sim->die_f) = 1;
			pthread_mutex_unlock(sim->die_mutex);

			pthread_mutex_lock(sim->stop_mutex);
			*(sim->stop) = 1;			
			pthread_mutex_unlock(sim->stop_mutex);

			print_log(sim, " died1", timestamp);
			return (0);
		}
		
    	current = (now.tv_sec - sim->start.tv_sec) * 1000
			+ (now.tv_usec - sim->start.tv_usec) / 1000;
	}
	return (1);
}

int	philo_takes_fork(t_simulation *sim)
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
	return (1);
}

int	philo_eats(t_simulation *sim)
{
	pthread_mutex_lock(sim->last_meal_mutex);
    gettimeofday(sim->last_meal_time, NULL);
	pthread_mutex_unlock(sim->last_meal_mutex);
	if (!try_log_action_or_die(sim, " is eating", NULL, NULL))
		return (0);
	//usleep(sim->ph->time_to_eat * 1000);
	sleep_in_ms(sim, sim->ph->time_to_eat);
	pthread_mutex_unlock(sim->ph->left_fork);
	pthread_mutex_unlock(sim->ph->right_fork);
	return (1);
}

int	philo_sleeps(t_simulation *sim)
{
	if (!try_log_action_or_die(sim, "is sleeping", sim->log_mutex, NULL))
		return (0);
	//usleep(sim->ph->time_to_sleep * 1000);
	sleep_in_ms(sim, sim->ph->time_to_sleep);
	return (1);
}

int philo_thinks(t_simulation *sim)
{
	if (!try_log_action_or_die(sim, "is thinking", sim->log_mutex, NULL))
		return (0);
    return 1;
}

void	*task(void *args)
{
	t_simulation *sim;
	int i;

	sim = (t_simulation *)args;
	i = 0;
	while (((sim->ph->repeat == 0) || i < sim->ph->repeat))
	{				
		*(sim->times) = i+1;
		pthread_mutex_lock(sim->die_mutex);
		if (*(sim->die_f) || *(sim->stop))
		{
			pthread_mutex_unlock(sim->die_mutex);
			break ;
		}
		pthread_mutex_unlock(sim->die_mutex);
		if (sim->total_ph == 1)
		{
			try_log_action_or_die(sim, "has taken the LEFT fork",
				NULL, NULL);
			while (1)
			{
					pthread_mutex_lock(sim->die_mutex);
					int should_stop = (*(sim->die_f) || *(sim->stop));
					pthread_mutex_unlock(sim->die_mutex);
					if (should_stop)
						break;
					usleep(1000);
			}				
			break ;
		}
		if (!philo_takes_fork(sim))
			break ;
		if (!philo_eats(sim))
			break ;
		if (!philo_sleeps(sim))
			break ;
		if (!philo_thinks(sim))
			break ;
		i++;
	}
	return (NULL);
}
