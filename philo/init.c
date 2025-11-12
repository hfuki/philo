#include "philo.h"

static int	init_forks(t_shared *sh, const t_args *a)
{
	int	i;

	i = 0;
	while (i < a->n_philo)
	{
		if (pthread_mutex_init(&sh->forks[i], NULL) != 0)
		{
			while (i >= 0)
			{
				pthread_mutex_destroy(&sh->forks[i]);
				i--;
			}
			pthread_mutex_destroy(&sh->print_mtx);
			pthread_mutex_destroy(&sh->stop_mtx);
			free(sh->forks);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	init_shared(t_shared *sh, const t_args *a)
{
	int	re_forks;

	if (!sh || !a || a->n_philo <= 0)
		return (-1);
	memset(sh, 0, sizeof(*sh));
	sh->stop = 0;
	sh->forks = calloc(a->n_philo, sizeof(pthread_mutex_t));
	if (!sh->forks)
		return (-1);
	if (pthread_mutex_init(&sh->print_mtx, NULL) != 0)
		return (free(sh->forks), -1);
	if (pthread_mutex_init(&sh->stop_mtx, NULL) != 0)
		return (pthread_mutex_destroy(&sh->stop_mtx), free(sh->forks), -1);
	re_forks = init_forks(sh, a);
	if (re_forks < 0)
		return (-1);
	return (0);
}

int	init_philos(t_philo **out, t_shared *sh, const t_args *a)
{
	t_philo	*ps;
	int		i;

	if (!out || !sh || !a || a->n_philo <= 0 || !sh->forks)
		return (-1);
	ps = calloc(a->n_philo, sizeof(t_philo));
	if (!ps)
		return (-1);
	i = 0;
	while (i < a->n_philo)
	{
		ps[i].id = i + 1;
		ps[i].eat_count = 0;
		ps[i].last_meal_ms = 0;
		ps[i].right = &sh->forks[i];
		ps[i].left = &sh->forks[(i + 1) % a->n_philo];
		ps[i].th = 0;
		ps[i].sh = sh;
		ps[i].a = a;
		i++;
	}
	*out = ps;
	return (0);
}

void	sync_start_time(t_shared *sh, t_philo *ph, int n)
{
	int	i;

	sh->start_ms = now_ms();
	i = 0;
	while (i < n)
	{
		ph[i].last_meal_ms = sh->start_ms;
		i++;
	}
}
