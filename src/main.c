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
	while (i < id_c)
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

void monitor_threads(t_simulation *sims[], long time_to_die, int id_c)
{
    struct timeval now;
    long elapsed;
    int i;
    int someone_died = 0; // Flag to break outer loop

    // Use a flag to control the loop instead of while(1)
    while (!someone_died)
    {
        i = 0;
        while (i < id_c)
        {
            // Get current time *inside* the loop for accuracy
            gettimeofday(&now, NULL);

            pthread_mutex_lock(sims[i]->last_meal_mutex);
            // Add a check for NULL pointer just in case
            if (!sims[i]->last_meal_time) {
                pthread_mutex_unlock(sims[i]->last_meal_mutex);
                i++;
                continue; // Skip if something went wrong with this philo's data
            }
            elapsed = (now.tv_sec - sims[i]->last_meal_time->tv_sec) * 1000 +
                      (now.tv_usec - sims[i]->last_meal_time->tv_usec) / 1000;
            pthread_mutex_unlock(sims[i]->last_meal_mutex);

            // Check for death
            if (elapsed >= time_to_die)
            {
                pthread_mutex_lock(sims[i]->die_mutex);
                // Check *if* death hasn't been flagged yet to avoid multiple death messages/actions
                if (!(*(sims[i]->die_f)))
                {
                    *(sims[i]->die_f) = 1; // Set shared flag FIRST

                    // Calculate timestamp for accurate death reporting
                    long timestamp = (now.tv_sec - sims[i]->start.tv_sec) * 1000 +
                                     (now.tv_usec - sims[i]->start.tv_usec) / 1000;

                    // Unlock die_mutex BEFORE printing (print_log locks log_mutex)
                    pthread_mutex_unlock(sims[i]->die_mutex);

                    // Monitor prints the official death message
                    print_log(sims[i], " died", timestamp);

                    // Signal ALL threads to stop using the stop flag
                    // (Assuming 'stop' is a shared flag protected by 'stop_mutex')
                    // You might need to loop through all sims again to set their stop flags
                    // if 'stop' is per-philosopher, or set a single shared 'stop' flag.
                    // Let's assume a shared stop flag for simplicity:
                     pthread_mutex_lock(sims[0]->stop_mutex); // Lock using any sim's mutex pointer (they point to the same shared mutex)
                     *(sims[0]->stop) = 1; // Set the shared stop flag
                     pthread_mutex_unlock(sims[0]->stop_mutex);


                    someone_died = 1; // Set flag to exit the monitor's loops
                    break; // Exit the inner loop (checking philosophers)
                }
                else
                {
                    // Death already flagged by this or another check, just unlock
                    pthread_mutex_unlock(sims[i]->die_mutex);
                }
            }
            // Only increment if no one died in this iteration
            if (!someone_died) {
                i++;
            }
        } // End inner loop (while i < id_c)

        // If someone died, the inner loop broke, and the outer loop condition (while !someone_died) will fail next

        // If no one died after checking everyone, sleep briefly
        if (!someone_died) {
            usleep(1000); // Check roughly every 1ms
        }
    } // End outer loop (while !someone_died)
    // Monitor function will now return after death detection
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
	pthread_t threads[id_c];
	t_simulation *sims[id_c];
	
	die_f = die_flag_initialize();
	if (!die_f)
		return (1);
	stop = stop_flag_initialize();
	if (!stop)
		return (1);
	initialize_mutexes(forks, &log_mutex, &die_mutex, &last_meal_mutex, &stop_mutex, id_c);
	while (i < id_c)
	{
		t_philo *ph = philosopher_initializer(ac, av, i, forks);
		if (!ph)
			return (1);
		t_simulation *sim = simulation_initializer(ph, die_f, stop, start, id_c, &last_meal_mutex);
		if (!sim)
			return (1);
		simulation_add_mutexes(sim, &log_mutex, &die_mutex, &stop_mutex);
		sims[i] = sim;
		pthread_create(&threads[i], NULL, task, sim);
		usleep(100);
		i++;
	}
	monitor_threads(sims, ft_atol(av[2]), id_c);
	join_threads(threads, id_c);
	destroy_mutexs(forks, id_c);
	clean_before_exit(&log_mutex, &die_mutex, &last_meal_mutex, &stop_mutex, die_f, stop);

	return (0);
}
