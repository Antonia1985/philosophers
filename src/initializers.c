/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 15:41:11 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/01 15:41:28 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*philosopher_initializer(int ac, char **av, int i,
		pthread_mutex_t forks[])
{
	t_philo	*ph;
	int		id_c;

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
		ph->repeat = ft_atoi(av[5]);
	ph->id = i + 1;
	ph->left_fork = &forks[i];
	if (id_c == 1)
		ph->right_fork = NULL;
	else
		ph->right_fork = &forks[(i + 1) % id_c];
	return (ph);
}

pthread_mutex_t	*forks_initilizer(t_context *ctx)
{
	pthread_mutex_t	*forks;
	int				j;

	forks = malloc(sizeof(pthread_mutex_t) * (ctx->id_c));
	if (!forks)
	{
		printf("Failed to allocate memmory for stop flag");
		free(ctx->threads);
		free(ctx->sims);
		free(ctx->die_f);
		free(ctx->stop);
		return (NULL);
	}
	j = 0;
	while (j < ctx->id_c)
	{
		pthread_mutex_init(&forks[j], NULL);
		j++;
	}
	return (forks);
}

t_simulation	*simulation_initializer(t_context *ctx)
{
	ctx->sim = malloc(sizeof(t_simulation));
	if (!ctx->sim)
	{
		printf("Failed to allocate memmory for sim");
		free(ctx->ph);
		return (NULL);
	}
	ctx->sim->stop = ctx->stop;
	ctx->sim->die_f = ctx->die_f;
	ctx->sim->start = ctx->start;
	ctx->sim->ph = ctx->ph;
	ctx->sim->total_ph = ctx->id_c;
	ctx->sim->last_meal_mutex = &ctx->last_meal_mutex;
	ctx->sim->last_meal_time = last_meal_initializer(ctx->sim, ctx->start);
	if (!ctx->sim->last_meal_time)
		return (NULL);
	ctx->sim->times = times_initializer(ctx->sim);
	return (ctx->sim);
}

struct timeval	*last_meal_initializer(t_simulation *sim, struct timeval start)
{
	sim->last_meal_time = malloc(sizeof(struct timeval));
	if (!sim->last_meal_time)
	{
		printf("Failed to allocate memmory for sim->last_meal_time");
		free(sim->ph);
		free(sim);
		return (NULL);
	}
	pthread_mutex_lock(sim->last_meal_mutex);
	*(sim->last_meal_time) = start;
	pthread_mutex_unlock(sim->last_meal_mutex);
	return (sim->last_meal_time);
}

int	*times_initializer(t_simulation *sim)
{
	int	*times;

	times = malloc(sizeof(int));
	if (!times)
	{
		printf("Failed to allocate memmory for sim->times");
		free(sim->ph);
		free(sim->times);
		free(sim);
		return (NULL);
	}
	*times = 1;
	return (times);
}
