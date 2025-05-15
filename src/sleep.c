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

int	check_flags_to_exit(int *die_f, int *stop, pthread_mutex_t *die_mutex, pthread_mutex_t *stop_mutex)
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
	return(1);
}

long	get_elapsed(struct timeval now, pthread_mutex_t *last_meal_mutex, struct timeval *last_meal_time)
{
	long	elapsed;

	pthread_mutex_lock(last_meal_mutex);
		elapsed = (now.tv_sec - last_meal_time->tv_sec) * 1000
			+ (now.tv_usec - last_meal_time->tv_usec) / 1000;
		pthread_mutex_unlock(last_meal_mutex);
	return(elapsed);
}

long	get_timestamp(struct timeval now, struct timeval *start)
{
	long	timestamp;

    timestamp = (now.tv_sec - start->tv_sec) * 1000
        + (now.tv_usec - start->tv_usec) / 1000;
	return(timestamp);
}


long	get_time_waiting(struct timeval now, struct timeval start_waiting)
{
	long	time_waiting;

    time_waiting = (now.tv_sec - start_waiting.tv_sec) * 1000
        + (now.tv_usec - start_waiting.tv_usec) / 1000;
	return(time_waiting);
}

void	update_death_flags_and_log(t_simulation *sim, struct timeval now)
{
	long	timestamp;

	timestamp = get_timestamp(now, &sim->start);
	pthread_mutex_lock(sim->die_mutex);
	*(sim->die_f) = 1;
	pthread_mutex_unlock(sim->die_mutex);
	
	pthread_mutex_lock(sim->stop_mutex);
	*(sim->stop) = 1;
	pthread_mutex_unlock(sim->stop_mutex);
    
	print_log(sim, " died1", timestamp);
}

int	log_death1(t_simulation *sim, struct timeval now)
{
	int				someone_died;	
	long			timestamp;

    someone_died = 0;
    pthread_mutex_lock(sim->die_mutex);
    if (!(*(sim->die_f)))
    {
        *(sim->die_f) = 1;
        pthread_mutex_unlock(sim->die_mutex);
        timestamp = get_timestamp(now, &sim->start);
        print_log(sim, " died1", timestamp);
        pthread_mutex_lock(sim->stop_mutex);
        *(sim->stop) = 1;
        pthread_mutex_unlock(sim->stop_mutex);
        someone_died = 1;
    }
    else
    {
        pthread_mutex_unlock(sim->die_mutex);
    }
    return (someone_died);
}

int	check_if_sbdy_died1(t_simulation *sim, int *someone_died, int *philos_done_this_round)
{
    struct timeval	now;
	long			elapsed;
    int time_to_die = sim->ph->time_to_die;

    gettimeofday(&now, NULL);
    elapsed = get_elapsed(now, sim->last_meal_mutex, sim->last_meal_time);
    if (elapsed >= time_to_die)
    {      
       *someone_died = log_death1(sim, now);
        return(0) ;
    }
    if (sim->ph->repeat > 0 && ((*sim->times) >= sim->ph->repeat))
    {
       (*philos_done_this_round)++;
    }
    return(1) ;
}

void sleep_in_ms(t_simulation *sim, int ms) {
	long time_waiting;
	struct timeval start_waiting;
    struct timeval now;

    gettimeofday(&start_waiting, NULL);
    while (1) {
        gettimeofday(&now, NULL);
        if (!check_flags_to_exit(sim->die_f, sim->stop, sim->die_mutex, sim->stop_mutex)) 
            break;
		if(!check_if_sbdy_died1(sim, sim->die_f, sim->stop))
            break;
		time_waiting = get_time_waiting(now, start_waiting);
		if (time_waiting >= ms)
            break;
        usleep(100);
    }
}
