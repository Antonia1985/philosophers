#include "philo.h"

int philo_takes_forks(t_simulation *sim)
{
    if (sim->ph->id%2 == 0)
    {
        pthread_mutex_lock(sim->ph->left_fork);
        if (!try_log_action_or_die(sim, " has taken the LEFT fork",
                    sim->ph->left_fork, NULL))
            return(0);
        pthread_mutex_lock(sim->ph->right_fork);
        if (!try_log_action_or_die(sim, " has taken the RIGHT fork",
                sim->ph->left_fork, sim->ph->right_fork))
            return(0);
    }
    else
    {
        pthread_mutex_lock(sim->ph->right_fork);
        if (!try_log_action_or_die(sim, " has taken the RIGHT fork",
                sim->ph->left_fork, sim->ph->right_fork))
            return(0);
        pthread_mutex_lock(sim->ph->left_fork);
        if (!try_log_action_or_die(sim, " has taken the LEFT fork",
                    sim->ph->left_fork, NULL))
            return(0);
    }    
    return(1);
}

int philo_eats(t_simulation *sim)
{
    if (!try_log_action_or_die(sim, " is eating", NULL, NULL))
            return(0);
    gettimeofday(sim->last_meal_time, NULL);
    usleep(sim->ph->time_to_eat * 1000);
    pthread_mutex_unlock(sim->ph->left_fork);
    pthread_mutex_unlock(sim->ph->right_fork);
    return(1);
}

int philo_sleeps_and_thinks(t_simulation *sim)
{
    if (!try_log_action_or_die(sim, "is sleeping", sim->log_mutex, NULL))
        return(0);
    usleep(sim->ph->time_to_sleep * 1000);
    if (!try_log_action_or_die(sim, "is thinking", sim->log_mutex, NULL))
        return(0);
    return(1);
}

void	*task(void *args)
{
    t_simulation	*sim;
    int				i;

    sim = (t_simulation *)args;
    i = 0;
    if (sim->ph->id % 2 == 0)
		usleep(100);
    while (((sim->ph->repeat == 0) || i < sim->ph->repeat))
    {
        if(sim->total_ph == 1)
        {
            pthread_mutex_lock(sim->ph->left_fork);
            try_log_action_or_die(sim, "has taken a fork", sim->ph->left_fork, NULL);
            usleep(sim->ph->time_to_die * 1000);
            try_log_action_or_die(sim, "died", sim->ph->left_fork, NULL);
            break;
        }
        if(!philo_takes_forks(sim))
            break;
        if (!philo_eats(sim))
            break;        
        if (!philo_sleeps_and_thinks(sim))
            break;
        i++;
    }
    free(sim->last_meal_time);
	free(sim->ph);
	free(sim);
    return (NULL);
}

