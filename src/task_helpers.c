#include "philo.h"

void    clean_after_death(pthread_mutex_t *mutex1, pthread_mutex_t *mutex2)
{      
    if(mutex1)  
        pthread_mutex_unlock(mutex1);
    if(mutex2)
        pthread_mutex_unlock(mutex2);
    
}

void    print_log(t_simulation *sim, const char * msg, long timestamp)
{   
    pthread_mutex_lock(sim->log_mutex);
    printf("%li Philosopher no %i %s\n", timestamp, sim->ph->id, msg);
    pthread_mutex_unlock(sim->log_mutex);
}

void    get_timestamps(t_simulation *sim, long *elapsed, long *timestamp)
{
    struct timeval now;    
    
    gettimeofday(&now, NULL);
    *timestamp = (now.tv_sec - sim->start.tv_sec) * 1000 
                + (now.tv_usec - sim->start.tv_usec) / 1000;
    *elapsed = (now.tv_sec - sim->last_meal_time->tv_sec) * 1000 
                + (now.tv_usec - sim->last_meal_time->tv_usec) / 1000;    

}

int   try_log_action_or_die(t_simulation *sim, const char * msg, 
                                pthread_mutex_t *mutex1, pthread_mutex_t *mutex2)
{
    long elapsed;
    long timestamp;

    pthread_mutex_lock(sim->die_mutex);
    get_timestamps(sim, &elapsed, &timestamp);
    if (*(sim->die_f))
    {
        pthread_mutex_unlock(sim->die_mutex);
        return (0);
    }
    if (elapsed > sim->ph->time_to_die)
    {
        *(sim->die_f) = 1;
        pthread_mutex_unlock(sim->die_mutex);
        print_log(sim, " died", timestamp);
        clean_after_death(mutex1, mutex2);
        return (0);
    }
    pthread_mutex_unlock(sim->die_mutex);
    print_log(sim, msg, timestamp);
    return (1);
}