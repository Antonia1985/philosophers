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

int	initializer(t_context *ctx)
{
	ctx->threads = malloc(sizeof(pthread_t) * (ctx->id_c + 1));
	if (!ctx->threads)
		return (0);		
	ctx->sims = malloc(sizeof(t_simulation *) * (ctx->id_c));
	if (!ctx->sims)
	{
		free(ctx->threads);
		return (0);
	}
	ctx->die_f = die_flag_initialize();
	if (!ctx->die_f)
	{
		free(ctx->sims);
		free(ctx->threads);
		return (0);
	}		
	ctx->stop = stop_flag_initialize();
	if (!ctx->stop)
	{
		free(ctx->sims);
		free(ctx->threads);
		free(ctx->die_f);
		return (0);
	}
	ctx->forks = forks_initilizer(ctx);
	if (!ctx->forks)
	{
		free(ctx->sims);
		free(ctx->threads);
		free(ctx->die_f);
		free(ctx->stop);
		return (0);
	}				
	initialize_mutexes(&ctx->log_mutex, &ctx->die_mutex, &ctx->last_meal_mutex,
		&ctx->stop_mutex);
	return (1);
}

int	validate_args_and_alloc(t_context *ctx, int ac, char **av)
{
	ctx->ac = ac;
	ctx->av = av;
	if (ac > 6 || ac < 5)
	{
		printf("Usage ./philo <number_of_philosophers> <time_to_die>"
			" <time_to_eat> <time_to_sleep>"
			" [number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	ctx->id_c = ft_atoi(av[1]);
	if (ctx->id_c <= 0 ||  ft_atoi(av[2]) <= 0 ||  ft_atoi(av[3]) <= 0 ||  ft_atoi(av[4]) <= 0)
    {
		printf("Arguments must be > 0");
		return (1);
	}
	gettimeofday(&ctx->start, NULL);
	if (!initializer(ctx))
	{
		if(ctx->forks)
			free(ctx->forks);
		return (0);	
	}
			
	return (1);
}

int	create_threads(t_context *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->id_c)
	{
		ctx->ph = philosopher_initializer(ctx->ac, ctx->av, i, ctx->forks);
		if (!ctx->ph)
			return (1);
		ctx->sim = simulation_initializer(ctx);
		if (!ctx->sim)
			return (1);
		ctx->sim->log_mutex = &ctx->log_mutex;
		ctx->sim->die_mutex = &ctx->die_mutex;
		ctx->sim->stop_mutex = &ctx->stop_mutex;
		ctx->sims[i] = ctx->sim;
		pthread_create(&ctx->threads[i], NULL, task, ctx->sim);
		usleep(100);
		i++;
	}
	pthread_create(&ctx->threads[ctx->id_c], NULL, monitor_threads,
		(void *)ctx->sims);
	usleep(100);
	join_threads(ctx->threads, ctx->id_c);
	return (0);
}

void	cleanup_and_exit_main(t_context *ctx)
{
	int	i;

	destroy_fork_mutexs(ctx->forks, ctx->id_c);
	destroy_mutexs_before_exit(&ctx->log_mutex, &ctx->die_mutex,
		&ctx->last_meal_mutex, &ctx->stop_mutex);
	clean_flags_before_exit(ctx->die_f, ctx->stop);
	free(ctx->forks);
	free(ctx->threads);
	i = 0;
	while (i < ctx->id_c)
	{
		if (ctx->sims[i])
		{
			pthread_mutex_destroy(&ctx->sims[i]->times_mutex);
			free(ctx->sims[i]->times);
			free(ctx->sims[i]->last_meal_time);
			free(ctx->sims[i]->ph);
			free(ctx->sims[i]);
		}
		i++;
	}
	free(ctx->sims);
	free(ctx);
}

int	main(int ac, char **av)
{
	t_context	*ctx;

	ctx = malloc(sizeof(t_context));
	if (!ctx)
	{
		printf("Failed to allocate memory for context\n");
		return (1);
	}
	if (!validate_args_and_alloc(ctx, ac, av))
	{
		//free(ctx);
		return (1);
	}
	if (!create_threads(ctx))
	{
		cleanup_and_exit_main(ctx);
		return (1);
	}		
	cleanup_and_exit_main(ctx);
	
	return (0);
}