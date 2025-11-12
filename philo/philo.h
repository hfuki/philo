/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfuki <hfuki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:55:54 by hfuki             #+#    #+#             */
/*   Updated: 2025/10/14 15:28:56 by hfuki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define sleep_time 500

typedef struct s_shared {
	long			start_ms;
	int				stop;
	pthread_mutex_t	print_mtx;
	pthread_mutex_t	stop_mtx;
	pthread_mutex_t	*forks;
}	t_shared;

typedef	struct s_args{
	int				n_philo;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				must_eat;	
}	t_args;


typedef struct s_philo{
	int				id;
	long			last_meal_ms;
	int				eat_count;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	pthread_t		th;
	t_shared		*sh;
	const t_args	*a;
}	t_philo;

typedef struct s_monitor_arg{
	t_shared		*sh;
	t_philo			*ph;
	const t_args	*a;
	pthread_t		mon;
}	t_monitor_arg;

int		is_stopped(t_shared *sh);
void	log_state(t_shared *sh, int id, const char *msg);
long	now_ms(void);
int		msleep_long(long ms, t_shared *sh);
void	set_stop(t_shared *sh);
void	ft_putstr_fd(char *msg, int fd);
int		init_shared(t_shared *sh, const t_args *a);
int		init_philos(t_philo **out, t_shared *sh, const t_args *a);
void	sync_start_time(t_shared *sh, t_philo *ph, int n);
long	ft_atoi(const char *str, int *ok);
void	*philo_thread(void *arg);
void	*monitor_thread(void *vp);

#endif
