#pragma once
#include <cerrno>
#include <string>
#include <ostream>

#include <mqueue.h>
#include "exception.hh"
#include <cstring>

using std::string;
using std::ostream;

// -----------------------------------------------------------------------------
// A wrapper around POSIX message queues
// POSIX message queues allow processes to exchange data in the form of messages
// see also: mq_overview(7)
struct mq
{
	struct attr;

	// ---------------------------------------------------------------------
	// default constructor
	mq (void);

	// constructs and open a message queue
	explicit mq(const string& name, int oflag);
	explicit mq(const string& name, int oflag, mode_t mode);
	explicit mq(const string& name, int oflag, mode_t mode, const struct mq::attr& attr);


	// ---------------------------------------------------------------------
	// open a message queue
	// see also: mq_open(3)
	void		open(const string& name, int oflag);
	void		open(const string& name, int oflag, mode_t mode, const struct mq::attr& attr);
	void		open(const string& name, int oflag, mode_t mode, const struct mq_attr *attr = NULL);


	// ---------------------------------------------------------------------
	// close the message queue
	// see also: mq_close(3)
	void		close(void);


	// ---------------------------------------------------------------------
	// remove a message queue
	// mq::unlink()  removes  the  message  queue  name. The queue itself is
	// destroyed  once  any  other  processes that have the queue open close
	// their descriptors referring to the queue.
	// see also: mq_unlink(3)
	static void	unlink(const string& name);
	void		unlink(void) const;


	// ---------------------------------------------------------------------
	// receive a message from a message queue
	// see also: mq_receive(3)
	ssize_t		receive(char *msg_ptr, size_t msg_len, unsigned *msg_prio = NULL) const;

	template <typename data_type>
	ssize_t		receive(data_type& data, unsigned *msg_prio = NULL) const;


	// ---------------------------------------------------------------------
	// send a message to a message queue
	// see also: mq_send(3)
	void		send(const string& msg, unsigned msg_prio) const;
	void		send(const char *msg_ptr, size_t msg_len, unsigned msg_prio) const;

	template <typename data_type>
	ssize_t		send(const data_type& data, unsigned msg_prio) const;


	// ---------------------------------------------------------------------
	// conversion operator for retro-compatibility
	// see also: mq_overview(7), mq_unlink(3)
	operator mqd_t (void) const;
	operator const char* (void) const;


	// ---------------------------------------------------------------------
	// conveniently replace struct  mq_attr  with a  conversion operator for
	// retrocompatibility purpose
	// see also: mq_getattr(3)
	struct attr
	{
		long flags;       // Flags: 0 or O_NONBLOCK
		long maxmsg;      // Max. # of messages on queue
		long msgsize;     // Max. message size (bytes)
		long curmsgs;     // # of messages currently in queue

		attr (long flags   = 0,
		      long maxmsg  = 10,
		      long msgsize = 8192,
		      long curmsgs = 0);

		operator struct mq_attr (void);
	};


	// ---------------------------------------------------------------------
	// serves as the base class  for  the exceptions  thrown  by  the wraper
	struct exception
	{
		virtual ~exception(void) throw() {}
	};

	// defines an exceptions to report operating system runtime error
	struct system_error : public mq::exception, public std::runtime_error
	{
		explicit system_error(const error_code& code) throw();
		explicit system_error(const string& call_name, const error_code& code = errno) throw();

		// returns the error_code object associated with the exception.
		const error_code&	code(void) const throw();
		// returns a symbolic error name eg. EACCES
		const char*		what(void) const throw();


		private:
		const error_code internal_code;
	};


	~mq (void);

	private:
	mqd_t		mqdes;
	string		qname;
};

#include "mqueue.tcc"
