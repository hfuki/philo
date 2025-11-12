#include "philo.h"

static void	update_last_eaten(long *last, int *eaten, t_monitor_arg *ma, int i)
{
	pthread_mutex_lock(&ma->sh->stop_mtx);
	last = ma->ph[i].last_meal_ms;
	eaten = ma->ph[i].eat_count;
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

void	*monitor_thread(void *vp)
{
	t_monitor_arg	*ma;
	int				all_full;
	int				i;
	long			last;
	int				eaten;

	ma = (t_monitor_arg *)vp;
	while (1)
	{
		all_full = 0;
		if (is_stopped(ma->sh))
			return NULL;
		if (ma->a->must_eat > 0)
			all_full = 1;
		i = 0;
		while (i < ma->a->n_philo)
		{
			update_last_eaten(&last, &eaten, ma, i);
			if (now_ms() - last > ma->a->t_die)
			{
				stop_diemessage(ma, i);
				return NULL;
			}
			if (all_full > 0 && eaten < ma->a->must_eat)
				all_full = 0;
			i++;
		}
		if (all_full)
		{
			set_stop(ma->sh);
			return NULL;
		}
	}
}