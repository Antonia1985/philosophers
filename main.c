/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:41:40 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/01 15:41:49 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_threads(pthread_t threads[], int id_c)
{
	int	i;

	i = 0;
	while (i <= id_c)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

void	destroy_mutexs(pthread_mutex_t forks[], int id_c)
{
	int	i;

	i = 0;
	while (i < id_c)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}

void	initialize_mutexes(pthread_mutex_t forks[], pthread_mutex_t *log_mutex,	
	pthread_mutex_t *die_mutex, pthread_mutex_t *last_meal_mutex, pthread_mutex_t *stop_mutex, int id_c)
{
	pthread_mutex_init(log_mutex, NULL);
	pthread_mutex_init(die_mutex, NULL);
	pthread_mutex_init(last_meal_mutex, NULL);
	pthread_mutex_init(stop_mutex, NULL);
	forks_initilizer(forks, id_c);
}

void	clean_before_exit(pthread_mutex_t *log_mutex,
		pthread_mutex_t *die_mutex, pthread_mutex_t *last_meal_mutex, pthread_mutex_t *stop_mutex, int *die_f, int *stop)
{
	free(die_f);
	free(stop);
	pthread_mutex_destroy(log_mutex);
	pthread_mutex_destroy(die_mutex);
	pthread_mutex_destroy(last_meal_mutex);
	pthread_mutex_destroy(stop_mutex);
}

void*	monitor_threads(void *args)
{
	t_simulation **sims = (t_simulation **)args;
	long time_to_die = sims[0]->ph->time_to_die;
	int id_c = sims[0]->total_ph;

    struct timeval now;
    long elapsed;
    int i;
	
	int all_philos_finished_eating = 0;
	
    int someone_died = 0;
	
	
    while (!someone_died && !all_philos_finished_eating)
    {
        i = 0;
		int philos_done_this_round = 0; // Counter for philos who met repeat count
		
		while (i < id_c && !someone_died && !all_philos_finished_eating)
		{	
			gettimeofday(&now, NULL);
			pthread_mutex_lock(sims[i]->last_meal_mutex);           
			if (!sims[i]->last_meal_time) 
			{
				pthread_mutex_unlock(sims[i]->last_meal_mutex);
				i++;
				continue;
			}
			elapsed = (now.tv_sec - sims[i]->last_meal_time->tv_sec) * 1000 +
					(now.tv_usec - sims[i]->last_meal_time->tv_usec) / 1000;
			pthread_mutex_unlock(sims[i]->last_meal_mutex);
			
			if (elapsed >= time_to_die)
			{
				pthread_mutex_lock(sims[i]->die_mutex);
			
				if (!(*(sims[i]->die_f)))
				{
					*(sims[i]->die_f) = 1; 
					pthread_mutex_unlock(sims[i]->die_mutex);

					long timestamp = (now.tv_sec - sims[i]->start.tv_sec) * 1000 +
									(now.tv_usec - sims[i]->start.tv_usec) / 1000;
					
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
				break;
			}	
			if (sims[i]->ph->repeat > 0 && (*(sims[i]->times) >= sims[i]->ph->repeat))
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
		if (!someone_died && !all_philos_finished_eating )
			usleep(1000);      
    }
	return(NULL);
}

int	main(int ac, char **av)
{
	if (ac > 6 || ac < 5)
	{
		printf("Usage ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	int i;
	int id_c;
	struct timeval start;
	int *die_f;
	int *stop;
		
	i = 0;
	id_c = ft_atoi(av[1]);
	gettimeofday(&start, NULL);
	pthread_mutex_t forks[id_c];
	pthread_mutex_t log_mutex;
	pthread_mutex_t die_mutex;
	pthread_mutex_t last_meal_mutex;
	pthread_mutex_t stop_mutex;
	pthread_t *threads = malloc(sizeof(pthread_t) * (id_c+1));
	if (!threads) return(1);
	t_simulation *sims[id_c];
	
	die_f = die_flag_initialize();
	if (!die_f)
		return (1);
	stop = stop_flag_initialize();
	if (!stop)
		return (1);
	
	initialize_mutexes(forks, &log_mutex, &die_mutex, &last_meal_mutex, &stop_mutex, id_c);
	while (i <= id_c)
	{
		
		
		if (i < id_c)
		{
			t_philo *ph = philosopher_initializer(ac, av, i, forks);
			if (!ph)
				return (1);
			t_simulation *sim = simulation_initializer(ph, die_f, stop, start, id_c, &last_meal_mutex);
			if (!sim)
				return (1);
			simulation_add_mutexes(sim, &log_mutex, &die_mutex, &stop_mutex);
			sims[i] = sim;
			int	rem3 = ph->id % 3;			
			if (id_c %2 ==1 && id_c >= 3)
				usleep(rem3 * 500);
			pthread_create(&threads[i], NULL, task, sim);
		}
		else
		{			
			pthread_create(&threads[i], NULL, monitor_threads, (void *)sims);
		}		
		usleep(100);
		i++;
	}
	join_threads(threads, id_c);
	destroy_mutexs(forks, id_c);
	clean_before_exit(&log_mutex, &die_mutex, &last_meal_mutex, &stop_mutex, die_f, stop);
	free(threads);
	i = 0;
	while(i < id_c)
	{
		free(sims[i]->times);
		free(sims[i]->last_meal_time);
		free(sims[i]->ph);
		free(sims[i]);
		i++;
	}
	return (0);
}