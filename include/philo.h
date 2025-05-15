/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:39:16 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/01 15:39:33 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philosopher
{
	int				id;
	int				time_to_sleep;
	int				time_to_eat;
	int				time_to_die;
	int				repeat;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

}					t_philo;

typedef struct s_simulation
{
	t_philo			*ph;
	struct timeval	start;
	int				*die_f;
	int				*stop;
	int				*times;
	int				total_ph;
	struct timeval	*last_meal_time;
	pthread_mutex_t	*die_mutex;
	pthread_mutex_t	*log_mutex;
	pthread_mutex_t	*last_meal_mutex;
	pthread_mutex_t	*stop_mutex;
	pthread_mutex_t	times_mutex;

}					t_simulation;

typedef struct s_context
{
	int				ac;
	char			**av;
	int				id_c;
	struct timeval	start;
	int				*die_f;
	int				*stop;
	t_philo			*ph;
	t_simulation	*sim;
	t_simulation	**sims;
	pthread_t		*threads;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	die_mutex;
	pthread_mutex_t	last_meal_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	*forks;
}					t_context;

int					main(int ac, char **av);

void				join_threads(pthread_t threads[], int id_c);
void				destroy_fork_mutexs(pthread_mutex_t forks[], int id_c);
void				initialize_mutexes(pthread_mutex_t *log_mutex,
						pthread_mutex_t *die_mutex,
						pthread_mutex_t *last_meal_mutex,
						pthread_mutex_t *stop_mutex);
void				destroy_mutexs_before_exit(pthread_mutex_t *log_mutex,
						pthread_mutex_t *die_mutex,
						pthread_mutex_t *last_meal_mutex,
						pthread_mutex_t *stop_mutex);
void				clean_flags_before_exit(int *die_f, int *stop);

int					ft_atoi(const char *nptr);
long				ft_atol(const char *nptr);

void				*monitor_threads(void *args);

void				unlock_forks(pthread_mutex_t *mutex1,
						pthread_mutex_t *mutex2);
void				print_log(t_simulation *sim, const char *msg,
						long timestamp);
int					try_log_action_or_die(t_simulation *sim, const char *msg,
						pthread_mutex_t *mutex1, pthread_mutex_t *mutex2);

void				*task(void *args);
int					check_stop(pthread_mutex_t *stop_mutex, int * stop);
int					check_die_f(pthread_mutex_t *die_mutex, int * die_f);
void				sleep_in_ms(t_simulation *sim, int ms);
int					philo_takes_fork(t_simulation *sim);
int					philo_eats(t_simulation *sim);
int					philo_sleeps(t_simulation *sim);
int					philo_thinks(t_simulation *sim);
int					*die_flag_initialize();
int					*stop_flag_initialize();
pthread_mutex_t		*forks_initilizer(t_context *ctx);
t_simulation		*simulation_initializer(t_context *ctx);
void				simulation_add_mutexes(t_simulation *sim,
						pthread_mutex_t *log_mutex, pthread_mutex_t *die_mutex,
						pthread_mutex_t *stop_mutex);
struct timeval		*last_meal_initializer(t_simulation *sim,
						struct timeval start);
int					*times_initializer(t_simulation *sim);
t_philo				*philosopher_initializer(int ac, char **av, int i,
						pthread_mutex_t forks[]);
int					check_if_sbdy_died(t_simulation *sim, int*die);
int					check_flags_to_exit(int *die_f, int *stop, pthread_mutex_t *die_mutex, pthread_mutex_t *stop_mutex);
long				get_timestamp(struct timeval now, struct timeval *start);
long				get_elapsed(struct timeval now, pthread_mutex_t *last_meal_mutex, struct timeval *last_meal_time);
void				update_death_flags_and_log(t_simulation *sim, struct timeval now);
#endif
