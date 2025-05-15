/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers_flags.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 19:29:25 by apavlopo          #+#    #+#             */
/*   Updated: 2025/05/10 19:29:28 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	*die_flag_initialize()
{
	int	*die_f;

	die_f = malloc(sizeof(int));
	if (!die_f)
	{
		printf("Failed to allocate memmory for die_f flag");
		return (NULL);
	}
	*die_f = 0;
	return (die_f);
}

int	*stop_flag_initialize()
{
	int	*stop;

	stop = malloc(sizeof(int));
	if (!stop)
	{
		printf("Failed to allocate memmory for stop flag");		
		return (NULL);
	}
	*stop = 0;
	return (stop);
}