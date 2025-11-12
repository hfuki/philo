#include "philo.h"

static int	msleep_eat(t_philo	*p, const t_args *a)
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

static void	wait_until_die_single(t_philo *p)
{
	pthread_mutex_lock(p->right);
	log_state(p->sh, p->id, "has taken a fork");
	while (!is_stopped(p->sh))
		usleep(1000);
	pthread_mutex_unlock(p->right);
}

static void	take_forks_lmeal(t_philo *p)
{
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(p->right);
		log_state(p->sh, p->id, "has taken a fork");
		pthread_mutex_lock(p->left);
		log_state(p->sh, p->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(p->left);
		log_state(p->sh, p->id, "has taken a fork");
		pthread_mutex_lock(p->right);
		log_state(p->sh, p->id, "has taken a fork");
	}
	pthread_mutex_lock(&p->sh->stop_mtx);
	p->last_meal_ms = now_ms();
	pthread_mutex_unlock(&p->sh->stop_mtx);
	return ;
}

void	take_forks_and_eat(t_philo	*p, const t_args *a)
{
	int	res_msleep;

	if (is_stopped(p->sh))
		return ;
	take_forks_lmeal(p);
	log_state(p->sh, p->id, "is eating");
	pthread_mutex_lock(&p->sh->stop_mtx);
	p->eat_count++;
	pthread_mutex_unlock(&p->sh->stop_mtx);
	if (is_stopped(p->sh))
	{
		pthread_mutex_unlock(p->right);
		pthread_mutex_unlock(p->left);
		return ;
	}
	res_msleep = msleep_eat(p, a);
	if (res_msleep < 0)
	{
		pthread_mutex_unlock(p->right);
		pthread_mutex_unlock(p->left);
		return ;
	}
	pthread_mutex_unlock(p->right);
	pthread_mutex_unlock(p->left);
}

void	*philo_thread(void *arg)
{
	t_philo			*p;

	p = (t_philo *)arg;
	if (p->a->n_philo == 1)
	{
		wait_until_die_single(p);
		return ;
	}
	while (1)
	{
		if (is_stopped(p->sh))
			return ;
		take_forks_and_eat(p, p->a);
		if (is_stopped(p->sh))
			return ;
		log_state(p->sh, p->id, "is sleeping");
		usleep(p->a->t_sleep * 1000);
		log_state(p->sh, p->id, "is thinking");
	}
	return ;
}
