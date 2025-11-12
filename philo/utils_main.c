#include "philo.h"

void	ft_putstr_fd(char *msg, int fd)
{
	int	i;
	
	i = 0;
	while (msg[i])
	{
		write(fd, &msg[i], 1);
		i++;
	}
}

long	ft_atoi(const char *str, int *ok)
{
	int		sign;
	long	result;

	*ok = 1;
	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	if (*str && (*str < '0' || *str > '9'))
		*ok = 0;
	return (result * sign);
}