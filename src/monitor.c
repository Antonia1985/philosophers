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


void	update_philos_finished(t_simulation *sim, int *all_philos_finished_eating)
{
	pthread_mutex_lock(sim->stop_mutex);
	*(sim->stop) = 1;
	*all_philos_finished_eating = 1;
	pthread_mutex_unlock(sim->stop_mutex);
}

int	log_death(t_simulation *sim, struct timeval	now)
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
        print_log(sim, " died3", timestamp);
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

int	check_if_sbdy_died(t_simulation *sim, int *someone_died)
{
    struct timeval	now;
	long			elapsed;
    int time_to_die = sim->ph->time_to_die;

    gettimeofday(&now, NULL);
    elapsed = get_elapsed(now, sim->last_meal_mutex, sim->last_meal_time);
    if (elapsed >= time_to_die)
    {
       *someone_died = log_death(sim, now);
        return(0) ;
    }
    return(1) ;
}

void	update_philos_who_finished_all_rounds(t_simulation *sim, int *philos_done_this_round)
{
    if (sim->ph->repeat > 0 && (*(sim->times) >= sim->ph->repeat))
    {
       (*philos_done_this_round)++;
    }
}

// int check_sims_times_equals_repeat(t_simulation **sims, int num_sims) {
//     for (int i = 0; i < num_sims; i++) {
//         if (*(sims[i]->times) <= sims[i]->ph->repeat) {
//             return(0) ;
//         }
//     }
//     return(1) ;
// }

// if (check_sims_times_equals_repeat(sims, sims[0]->total_ph))	
//             update_philos_finished(sims[0], &all_philos_finished_eating);

void	*monitor_threads(void *args)
{
	t_simulation	**sims;
	int				i;
	int				all_philos_finished_eating;
	int				someone_died;
	int				philos_done_this_round;

	sims = (t_simulation **)args;
	all_philos_finished_eating = 0;
	someone_died = 0;
    philos_done_this_round = 0;
	while (!someone_died && !all_philos_finished_eating)
	{        
		i = 0;
		
		while (i < sims[0]->total_ph && !someone_died && !all_philos_finished_eating)
		{           
            t_simulation *sim = sims[i];

            if(check_die_f(sim->die_mutex, sim->die_f))
            {
                someone_died = 1;
                break;
            }
            if (!check_if_sbdy_died(sim, &someone_died))
                break ;
            
            update_philos_who_finished_all_rounds(sim, &philos_done_this_round);
			i++;
		}
		if (philos_done_this_round == sims[0]->total_ph)	
            update_philos_finished(sims[0], &all_philos_finished_eating);
		if (!someone_died && !all_philos_finished_eating)
			usleep(1000);
	}
	return (NULL);
}