#include "philo.h"

static void	update_last_eaten(t_monitor_arg *ma, int i)
{
	pthread_mutex_lock(&ma->sh->stop_mtx);
	ma->last = ma->ph[i].last_meal_ms;
	ma->eaten = ma->ph[i].eat_count;
	pthread_mutex_unlock(&ma->sh->stop_mtx);
}

static void	stop_diemessage(t_monitor_arg *ma, int i)
{
	set_stop(ma->sh);
	pthread_mutex_lock(&ma->sh->print_mtx);
	printf("%ld %d died\n", now_ms(), ma->ph[i].id);
	pthread_mutex_unlock(&ma->sh->print_mtx);
	return ;
}

static int	repeat_funcs_allfull(t_monitor_arg *ma, int i)
{
	update_last_eaten(ma, i);
	if (now_ms() - ma->last > ma->a->t_die)
	{
		stop_diemessage(ma, i);
		return (1);
	}
	if (ma->all_full > 0 && ma->eaten < ma->a->must_eat)
		ma->all_full = 0;
	return (0);
}

void	*monitor_thread(void *vp)
{
	t_monitor_arg	*ma;
	int				i;

	ma = (t_monitor_arg *)vp;
	while (1)
	{
		ma->all_full = 0;
		if (is_stopped(ma->sh))
			return NULL;
		if (ma->a->must_eat > 0)
			ma->all_full = 1;
		i = 0;
		while (i < ma->a->n_philo)
		{
			if (repeat_funcs_allfull(ma, i))
				return NULL;
			i++;
		}
		if (ma->all_full)
		{
			set_stop(ma->sh);
			return NULL;
		}
	}
}