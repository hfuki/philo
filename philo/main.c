/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfuki <hfuki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:55:25 by hfuki             #+#    #+#             */
/*   Updated: 2025/10/14 12:59:30 by hfuki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	argc6(int argc, char *argv5, t_args *a)
{
	long	num;
	int		ok;

	if (argc == 6)
	{
		num = ft_atoi(argv5, &ok);
		if (num > 2147483647 || num < 0 || !ok)
			return (1);
		a->must_eat = num;
	}
	else
		a->must_eat = -1;
	return (0);
}

static int	parse_args(int argc, char **argv, t_args *a)
{
	int		ok;
	long	num;

	if (!(argc == 5 || argc == 6))
		return (1);
	num = ft_atoi(argv[1], &ok);
	if (num > 2147483647 || num < 1 || !ok)
		return (1);
	a->n_philo = num;
	num = ft_atoi(argv[2], &ok);
	if (num < 0 || !ok)
		return (1);
	a->t_die = num;
	num = ft_atoi(argv[3], &ok);
	if (num < 0 || !ok)
		return (1);
	a->t_eat = num;
	num = ft_atoi(argv[4], &ok);
	if (num < 0 || !ok)
		return (1);
	a->t_sleep = num;
	if (argc6(argc, argv[5], a))
		return (1);
	return (0);
}

static void	create_philo(t_args *a, t_shared *sh, t_philo *ph)
{
	int	i;

	i = 0;
	while (i < a->n_philo)
	{
		if (pthread_create(&ph[i].th, NULL, philo_thread, &ph[i]))
		{
			set_stop(sh);
			while (--i >= 0)
				pthread_join(ph[i].th, NULL);
			free(ph);
			destroy_shared(sh, a);
			return (ft_putstr_fd("Error: pthread_create\n", 2));
		}
		i++;
	}
}

static void	monitor_process(t_args *a, t_shared *sh, t_philo *ph)
{
	t_monitor_arg	ma;
	int				i;

	ma.sh = sh;
	ma.ph = ph;
	ma.a = a;
	pthread_create(&ma.mon, NULL, monitor_thread, &ma);
	pthread_join(ma.mon, NULL);
	i = 0;
	while (i < a->n_philo)
		pthread_join(ph[i++].th, NULL);
	free(ph);
	destroy_shared(sh, a);
}

int	main(int argc, char **argv)
{
	t_args			a;
	t_shared		sh;
	t_philo			*ph;

	if (parse_args(argc, argv, &a))
		return (ft_putstr_fd(
				"Usage: ./philo n_philo t_die t_eat t_sleep [must_eat]\n",
				2), 1);
	if (init_shared(&sh, &a))
		return (ft_putstr_fd("Error: init_shared\n", 2), 1);
	if (init_philos(&ph, &sh, &a))
		return (ft_putstr_fd("Error: init_philos\n", 2), 1);
	sync_start_time(&sh, ph, a.n_philo);
	create_philo(&a, &sh, ph);
	monitor_process(&a, &sh, ph);
	return (0);
}
