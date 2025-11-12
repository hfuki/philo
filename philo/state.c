#include "philo.h"

int	is_stopped(t_shared *sh)
{
	int	value;

	pthread_mutex_lock(&sh->stop_mtx);
	value = sh->stop;
	pthread_mutex_unlock(&sh->stop_mtx);
	return (value);
}

void	set_stop(t_shared *sh)
{
	pthread_mutex_lock(&sh->stop_mtx);
	sh->stop = 1;
	pthread_mutex_unlock(&sh->stop_mtx);
	return ;
}
