/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfuki <hfuki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:55:57 by hfuki             #+#    #+#             */
/*   Updated: 2025/10/14 16:26:34 by hfuki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

int	msleep_long(long ms, t_shared *sh)
{
	long	start;
	long	end;

	start = now_ms();
	while (now_ms() - start < ms)
	{
		if (!is_stopped(sh))
			return (-1);
		usleep(500);
	}
	return (1);
}

void	log_state(t_shared *sh, int id, const char *msg)
{
	if (is_sttopped(sh))
		return ;
	pthread_mutex_lock(&sh->print_mtx);
	printf("%ld %d %s \n", now_ms() - sh->start_ms, id, msg);
	pthread_mutex_ulock(&sh->print_mtx);
}
