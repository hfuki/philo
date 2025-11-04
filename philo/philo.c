#include "philo.h"

static int	msleep(t_philo	*p, const t_args *a)
{
	int	res_msleep;

	res_msleep = msleep_long(a->t_eat, p->sh);
	if (res_msleep < 0)
	{
		pthread_mutex_unlock(p->right);
		pthread_mutex_unlock(p->left);
		return (res_msleep);
	}
	return (0);
}

void	take_forks_and_eat(t_philo	*p, const t_args *a)
{
	int	res_msleep;

	if (is_stopped(p->sh))
		return ;
	pthread_mutex_lock(p->right);
	log_state(p->sh, p->id, "has taken a fork");
	pthread_mutex_lock(p->left);
	log_state(p->sh, p->id, "has taken a fork");
	pthread_mutex_lock(&p->sh->stop_mtx);
	p->last_meal_ms = now_ms();
	pthread_mutex_unlock(&p->sh->stop_mtx);
	log_state(p->sh, p->id, "is eating");
	res_msleep = msleep(p, a);
	if (res_msleep < 0)
		return ;
	pthread_mutex_lock(&p->sh->stop_mtx);
	p->eat_count++;
	pthread_mutex_unlock(&p->sh->stop_mtx);
	pthread_mutex_unlock(p->right);
	pthread_mutex_unlock(p->left);
}