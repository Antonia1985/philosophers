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
		usleep(100);
		pthread_mutex_lock(sim->die_mutex);
		if (*(sim->die_f))
		{
			pthread_mutex_unlock(sim->die_mutex);
			return (0);
		}
        gettimeofday(&now, NULL);
        	gettimeofday(&now, NULL);
		elapsed = (now.tv_sec - sim->last_meal_time->tv_sec) * 1000
				+ (now.tv_usec - sim->last_meal_time->tv_usec) / 1000;
		if (elapsed > sim->ph->time_to_die)
		{
			long timestamp = (now.tv_sec - sim->start.tv_sec) * 1000
					+ (now.tv_usec - sim->start.tv_usec) / 1000;
			*(sim->die_f) = 1;
			pthread_mutex_unlock(sim->die_mutex);
			print_log(sim, " died", timestamp);
			return (0);
		}
		pthread_mutex_unlock(sim->die_mutex);
    	current = (now.tv_sec - sim->start.tv_sec) * 1000
			+ (now.tv_usec - sim->start.tv_usec) / 1000;
	}
	return (1);
}

void	check_elapsed(t_simulation *sim, pthread_mutex_t *mutex1,
		pthread_mutex_t *mutex2)
{
	struct timeval	now;
	long			elapsed;
	long			timestamp;

	gettimeofday(&now, NULL);
	get_timestamps(sim, &elapsed, &timestamp);
	if (elapsed > sim->ph->time_to_die)
	{
		*(sim->die_f) = 1;
		pthread_mutex_unlock(sim->die_mutex);
		print_log(sim, " died in check", timestamp);
		clean_after_death(mutex1, mutex2);
	}    
}

int	philo_takes_forks(t_simulation *sim)
{
	pthread_mutex_t *first;
	pthread_mutex_t *second;

	if (sim->ph->left_fork < sim->ph->right_fork)
	{
		first = sim->ph->left_fork;
		second = sim->ph->right_fork;
	}
	else
	{
		first = sim->ph->right_fork;
		second = sim->ph->left_fork;
	}
    check_elapsed(sim, NULL, NULL);  // Time check after 1st fork
	pthread_mutex_lock(first);
	if (!try_log_action_or_die(sim, " has taken a fork", first, NULL))
		return (0);

	check_elapsed(sim, first, NULL);  // Time check after 1st fork

	pthread_mutex_lock(second);
	if (!try_log_action_or_die(sim, " has taken a fork", first, second))
		return (0);

	check_elapsed(sim, first, second);  // Time check after 2nd fork

	return (1);
}

/*int	philo_takes_L_fork(t_simulation *sim)
{
	check_elapsed(sim, sim->ph->left_fork, NULL);
	if (sim->ph->id % 2 == 0 && *(sim->first_run) == 1) // && sim->total_ph > 3)
	{
		*(sim->first_run) = 0;
		usleep(50);
	}
	pthread_mutex_lock(sim->ph->left_fork);
	if (!try_log_action_or_die(sim, " has taken the LEFT fork",
			sim->ph->left_fork, NULL))
		return (0);

    pthread_mutex_lock(sim->ph->right_fork);
	if (!try_log_action_or_die(sim, " has taken the RIGHT fork",
			sim->ph->left_fork, sim->ph->right_fork))
		return (0);
	
	return (1);
}*/

/*int	philo_takes_R_fork(t_simulation *sim)
{
	if (sim->ph->id % 2 == 0 && *(sim->first_run) == 1)
	{
		*(sim->first_run) = 0;
		usleep(50);
	}
	pthread_mutex_lock(sim->ph->right_fork);
	if (!try_log_action_or_die(sim, " has taken the RIGHT fork",
			sim->ph->left_fork, sim->ph->right_fork))
		return (0);
	return (1);
}*/

int	philo_eats(t_simulation *sim)
{
    gettimeofday(sim->last_meal_time, NULL);
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

int philo_thinks(t_simulation *sim)
{
    check_elapsed(sim, NULL, NULL);

    long start, now, elapsed;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    start = (tv.tv_sec - sim->start.tv_sec) * 1000 + (tv.tv_usec - sim->start.tv_usec) / 1000;
    now = start;

    if (!try_log_action_or_die(sim, "is thinking", sim->log_mutex, NULL))
        return (0);

    while (now - start < 10) // think for 10ms to simulate activity
    {
        usleep(100); // allow other threads
        pthread_mutex_lock(sim->die_mutex);
        if (*(sim->die_f))
        {
            pthread_mutex_unlock(sim->die_mutex);
            return 0;
        }
        gettimeofday(&tv, NULL);
        elapsed = (tv.tv_sec - sim->last_meal_time->tv_sec) * 1000 + (tv.tv_usec - sim->last_meal_time->tv_usec) / 1000;
        now = (tv.tv_sec - sim->start.tv_sec) * 1000 + (tv.tv_usec - sim->start.tv_usec) / 1000;

        if (elapsed > sim->ph->time_to_die)
        {
            *(sim->die_f) = 1;
            pthread_mutex_unlock(sim->die_mutex);
            print_log(sim, " died", now);
            return 0;
        }
        pthread_mutex_unlock(sim->die_mutex);
    }
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
		pthread_mutex_lock(sim->die_mutex);
		if (*(sim->die_f))
		{
			pthread_mutex_unlock(sim->die_mutex);
			break ;
		}
		pthread_mutex_unlock(sim->die_mutex);
		if (sim->total_ph == 1)
		{
			pthread_mutex_lock(sim->ph->left_fork);
			try_log_action_or_die(sim, "has taken the LEFT fork",
				sim->ph->left_fork, NULL);
			usleep(sim->ph->time_to_die * 1000);
			try_log_action_or_die(sim, "died", sim->ph->left_fork, NULL);
			break ;
		}
		check_elapsed(sim, NULL, NULL);
		if (!philo_takes_forks(sim))
        	break;
        //if (!philo_takes_L_fork(sim))
		//	break ;
		//if (!philo_takes_R_fork(sim))
		//	break ;
		if (!philo_eats(sim))
			break ;
		if (!philo_sleeps(sim))
			break ;
		if (!philo_thinks(sim))
			break ;
		i++;
	}
	free(sim->last_meal_time);
	free(sim->ph);
	free(sim);
	return (NULL);
}
