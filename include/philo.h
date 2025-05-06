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

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_philosopher{
    int id;
    int time_to_sleep;
    int time_to_eat;
    int time_to_die;
    int repeat;
    pthread_mutex_t* left_fork;
    pthread_mutex_t* right_fork;
    
}t_philo;

typedef struct s_simulation{
    t_philo *ph;
    struct timeval start;
    int *die_f;
    int *stop;
    //int *first_run;
    int total_ph;
    struct timeval *last_meal_time;    
    pthread_mutex_t	*die_mutex;
    pthread_mutex_t *log_mutex;
    pthread_mutex_t *last_meal_mutex;
    pthread_mutex_t *stop_mutex;

}t_simulation;

int     main(int ac, char **av);

int     ft_atoi(const char *nptr);
long	ft_atol(const char *nptr);

void    clean_after_death(pthread_mutex_t *mutex1, pthread_mutex_t *mutex2);
void    print_log(t_simulation *sim, const char * msg, long timestamp);
void    get_timestamps(t_simulation *sim, long *elapsed, long *timestamp);
int   try_log_action_or_die(t_simulation *sim, const char * msg, pthread_mutex_t *mutex1, pthread_mutex_t *mutex2);

void	*task(void *args);

int     *die_flag_initialize();
int     *stop_flag_initialize();
void    forks_initilizer( pthread_mutex_t forks[], int n);
t_simulation *simulation_initializer(t_philo *ph, int *die_f, int *stop, struct timeval start, int id_c, pthread_mutex_t *last_meal_mutex);
void    simulation_add_mutexes(t_simulation *sim, pthread_mutex_t *log_mutex, pthread_mutex_t *die_mutex, pthread_mutex_t *stop_mutex);
struct timeval  *last_meal_initializer(t_simulation *sim, struct timeval start);
t_philo    *philosopher_initializer(int ac, char **av, int i, pthread_mutex_t forks[]);
void    clean_before_exit(pthread_mutex_t *log_mutex, pthread_mutex_t *die_mutex, pthread_mutex_t *last_meal_mutex, pthread_mutex_t *stop_mutex,int *die_f, int *stop);

#endif
