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

void	update_philos_finished(t_simulation *sim, int *all_philos_finished)
{
	pthread_mutex_lock(sim->stop_mutex);
	*(sim->stop) = 1;
	*all_philos_finished = 1;
	pthread_mutex_unlock(sim->stop_mutex);
}

int	log_death(t_simulation *sim, struct timeval now)
{
	int		someone_died;
	long	timestamp;

	someone_died = 0;
	pthread_mutex_lock(sim->die_mutex);
	if (!(*(sim->die_f)))
	{
		*(sim->die_f) = 1;
		pthread_mutex_unlock(sim->die_mutex);
		timestamp = get_timestamp(now, &sim->start);
		print_log(sim, " died", timestamp);
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
	int				time_to_die;

	time_to_die = sim->ph->time_to_die;
	gettimeofday(&now, NULL);
	elapsed = get_elapsed(now, sim->last_meal_mutex, sim->last_meal_time);
	if (elapsed >= time_to_die)
	{
		*someone_died = log_death(sim, now);
		return (0);
	}
	return (1);
}

int	check_if_alive(t_simulation *current_sim, int *someone_died,
		int *philos_done_c, int *all_philos_finished)
{
	t_simulation	*sim;

	sim = current_sim;
	if (check_die_f(sim->die_mutex, sim->die_f))
	{
		*someone_died = 1;
		return (0);
	}
	if (check_stop(sim->stop_mutex, sim->stop))
	{
		*all_philos_finished = 1;
		return (0);
	}
	if (!check_if_sbdy_died(sim, someone_died))
		return (0);
	pthread_mutex_lock(&sim->times_mutex);
	if (sim->ph->repeat > 0 && ((*sim->times) >= sim->ph->repeat) )
		//&& !sim->marked_done)
	{
		(*philos_done_c)++;
		//sim->marked_done = 1;
		/*pthread_mutex_lock(sim->log_mutex);
		printf("Philosopher no %i finished (%i / %i). philos_done_c=%i",//\tstop = %i, die_f= %i\n", 
			sim->ph->id, (*sim->times), sim->ph->repeat, *philos_done_c);//, *sim->stop, *sim->die_f);
		
		pthread_mutex_unlock(sim->log_mutex);*/
	}
	pthread_mutex_unlock(&sim->times_mutex);
	return (1);
}

void	*monitor_threads(void *args)
{
	t_simulation	**sims;
	int				i;
	int				all_philos_finished;
	int				someone_died;
	int				philos_done_c;

	sims = (t_simulation **)args;
	all_philos_finished = 0;
	someone_died = 0;
	philos_done_c = 0;
	while (!someone_died && !all_philos_finished)
	{
		i = 0;
		while (i < sims[0]->total_ph && !someone_died && !all_philos_finished)
		{
			if (!check_if_alive(sims[i], &someone_died, &philos_done_c, &all_philos_finished))
				break ;
			i++;
		}
		if (philos_done_c == sims[0]->total_ph)
			update_philos_finished(sims[0], &all_philos_finished);
		if (!someone_died && !all_philos_finished)
			usleep(1000);
	}
	return (NULL);
}
