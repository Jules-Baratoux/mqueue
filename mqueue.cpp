#include "exception.hh"
#include "mqueue.hh"

#include <ostream>
#include <cerrno>
#include <string>

using std::ostream;
using std::string;

mq::system_error::system_error(const error_code& code) throw()
        : mq::exception(), runtime_error(code), internal_code(code)
{ }

mq::system_error::system_error(const string& funcname, const error_code& code) throw()
        : mq::exception(), runtime_error(funcname + ": " + code.macro()), internal_code(code)
{ }

const error_code& mq::system_error::code(void) const throw()
{
        return internal_code;
}

const char* mq::system_error::what(void) const throw()
{
        return runtime_error::what();
}

mq::attr::attr (long flags, long maxmsg, long msgsize, long curmsgs)
        : flags(flags), maxmsg(maxmsg), msgsize(msgsize), curmsgs(curmsgs)
{ }

mq::attr::operator struct mq_attr (void)
{
        return (struct mq_attr) {flags, maxmsg, msgsize, curmsgs};
}

void            mq::open(const string& name, int oflag)
{
        qname = name;
        mqdes = safe(-1, mq_open, qname.c_str(), oflag) ;
}

void            mq::open(const string& name, int oflag, mode_t mode, const struct mq::attr& attr)
{
        qname = name;
        mqdes = safe(-1, mq_open, qname.c_str(), oflag, mode, &attr) ;
}

void            mq::open(const string& name, int oflag, mode_t mode, const struct mq_attr *attr)
{
        qname = name;
        mqdes = safe(-1, mq_open, qname.c_str(), oflag, mode, attr) ;
}

void            mq::close(void)
{
        safe(-1, mq_close, mqdes) ;
        mqdes = -1;
}

void      mq::unlink(const string& name)
{
        safe(-1, mq_unlink, name.c_str()) ;
}

void     mq::unlink(void) const
{
	mq::unlink(qname);
}

ssize_t  mq::receive(char *msg_ptr, size_t msg_len, unsigned *msg_prio) const
{
        return safe(-1, mq_receive, mqdes, msg_ptr, msg_len, msg_prio) ;
}

void     mq::send(const char *msg_ptr, size_t msg_len, unsigned msg_prio) const
{
        safe(-1, mq_send, mqdes, msg_ptr, msg_len, msg_prio) ;
}

void            mq::send(const string& msg, unsigned msg_prio) const
{
        safe(-1, mq_send, mqdes, msg.c_str(), msg.length(), msg_prio) ;
}

mq::mq (void) : mqdes(-1) {}

mq::mq(const string& name, int oflag)
{
	mq::open(name, oflag);
}

mq::mq(const string& name, int oflag, mode_t mode)
{
	mq::open(name, oflag, mode);
}

mq::mq(const string& name, int oflag, mode_t mode, const struct mq::attr& attr)
{
	mq::open(name, oflag, mode, attr);
}

mq::~mq (void)
{
        if (mqdes != -1)
		mq::close();
}

mq::operator mqd_t (void) const
{
        return mqdes;
}

mq::operator const char* (void) const
{
        return qname.c_str();
}
