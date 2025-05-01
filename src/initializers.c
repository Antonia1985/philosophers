#include "philo.h"

t_philo    *philosopher_initializer(int ac, char **av, int i, pthread_mutex_t forks[])
{
    t_philo *ph;
    int id_c;

    id_c = ft_atoi(av[1]);   
    ph = malloc(sizeof(t_philo));    
    if (!ph)
    {
        printf("Failed to allocate memmory for philosopher");
        return (NULL);
    }   
    ph->time_to_die = ft_atoi(av[2]);
    ph->time_to_eat = ft_atoi(av[3]);
    ph->time_to_sleep = ft_atoi(av[4]);
    ph->repeat = 0;    
    if (ac == 6)
        ph->repeat =  ft_atoi(av[5]);;
    ph->id = i+1;
    ph->left_fork = &forks[i];
    if (id_c == 1)
        ph->right_fork = NULL;
    else
        ph->right_fork = &forks[(i+1) % id_c];
    return ph;
}


t_philo    *philosopher_initializer1(int time_to_die, int time_to_eat, int time_to_sleep, int repeat)
{
    t_philo *ph;

    ph = malloc(sizeof(t_philo));
    if (!ph)
    {
        printf("Failed to allocate memmory for philosopher");
        return (NULL);
    }   
    ph->time_to_die = time_to_die;
    ph->time_to_eat = time_to_eat;
    ph->time_to_sleep = time_to_sleep;
    ph->repeat = 0;
    if (repeat != 0)
        ph->repeat =  repeat;
    return ph;
}

void    philosopher_initializer2(t_philo *ph,int i, pthread_mutex_t forks[], int id_c)
{
    ph->id = i+1;
    ph->left_fork = &forks[i];
    if (id_c == 1)
        ph->right_fork = NULL;
    else
        ph->right_fork = &forks[(i+1) % id_c];
    
}

int *die_flag_initialize()
{
    int *die_f = malloc(sizeof(int));
    if (!die_f)
    {
        printf("Failed to allocate memmory for die_f");
        return NULL;
    }
    *die_f = 0;
    return die_f;
}

void    forks_initilizer(pthread_mutex_t forks[], int n)
{
    int j;

    j = 0;
    while(j < n)
    {
        pthread_mutex_init(&forks[j], NULL);
        j++;
    }
}

t_simulation    *simulation_initializer(t_philo *ph, int *die_f, struct timeval start, int id_c)
{   
    t_simulation *sim = malloc(sizeof(t_simulation));
    if (!sim)
    {
        printf("Failed to allocate memmory for sim");
        free(ph);
        return (NULL);
    }
    sim->die_f = die_f;
    sim->start = start;
    sim->ph = ph;
    sim->total_ph = id_c;
    sim->last_meal_time = last_meal_initializer(sim, start);
    if (!sim->last_meal_time)
        return (NULL);
    return (sim);
}

void    simulation_add_mutexes(t_simulation *sim, pthread_mutex_t *log_mutex, pthread_mutex_t *die_mutex)
{
    sim->log_mutex = log_mutex;
    sim->die_mutex = die_mutex;    
}

struct timeval  *last_meal_initializer(t_simulation *sim, struct timeval start)
{
    sim->last_meal_time = malloc(sizeof(struct timeval));
    if (!sim->last_meal_time)
    {
        printf("Failed to allocate memmory for sim->last_meal_time");
        free(sim->ph);
        free(sim);
        return (NULL);
    }   
    *(sim->last_meal_time) = start;
    return (sim->last_meal_time);
}
