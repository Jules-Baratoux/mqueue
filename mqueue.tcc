// -*- mode: c++ -*-

/* -----------------------------------------------------------------------------
SYNOPSIS
	safe(error_case, syscall, args...);

DESCRIPTION
	Invokes a system call syscall, and throw an errno based mq::system_error
	on error.

PARAMETERS
	> error_case	the error case to check
	> syscall	a syscall name
	> args		the syscall arguments

RETURN VALUE
	The syscall return value or throw on error.

EXEMPLE
	ptr = safe(NULL, malloc, size);
                                                                              */
#define safe(ERROR_CASE, SYSCALL, ARGS...)				\
	({								\
		const typeof(SYSCALL(ARGS)) _return = SYSCALL(ARGS);	\
		if (_return == ERROR_CASE)				\
			throw mq::system_error(#SYSCALL, errno);	\
		_return;						\
	})

template <typename data_type>
ssize_t		mq::receive(data_type& data, unsigned *msg_prio) const
{
	return safe(-1, mq_receive, mqdes, reinterpret_cast<char*>(&data), sizeof(data), msg_prio);
}

template <typename data_type>
ssize_t		mq::send(const data_type& data, unsigned msg_prio) const
{
	return safe(-1, mq_send, mqdes, reinterpret_cast<const char*>(&data), sizeof(data), msg_prio);
}
