/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:09:15 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/15 17:09:19 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	mallocator1(t_context *ctx)
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
	return (1);
}

int	mallocator2(t_context *ctx)
{
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
