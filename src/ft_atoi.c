/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apavlopo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:20:49 by apavlopo          #+#    #+#             */
/*   Updated: 2024/11/09 17:21:33 by apavlopo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

static int	ft_isspace(char c)
{
	size_t	i;
	char	*space_char;

	i = 0;
	space_char = " \t\n\v\f\r";
	while (i < 6)
	{
		if (c == space_char[i])
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		signd;
	int		sign;
	int		num;

	i = 0;
	signd = 0;
	sign = 1;
	num = 0;
	while (ft_isspace(nptr[i]) == 1)
		i++;
	if (signd == 0 && (nptr[i] == '+' || nptr[i] == '-'))
	{
		if (nptr[i] == '-')
			sign = -1;
		signd = 1;
		i++;
	}
	if ((signd == 1) && (nptr[i] == '+' || nptr[i] == '-'))
		return (num);
	while ((nptr[i] != '\0') && (nptr[i] >= 48) && (nptr[i] <= 57))
		num = (num * 10) + (nptr[i++] - '0');
	num = num * sign;
	return (num);
}
