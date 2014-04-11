#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <cerrno>
#include <cstring>

using std::runtime_error;
using std::ostream;

class system_error : public runtime_error
{
	class error_code
	{
		int	errnum;

		public:
		explicit error_code(int errnum) : errnum(errnum) {}

		operator int (void) const
		{
			return errnum;
		}

		friend ostream&	operator << (ostream& stream, const error_code& error)
		{
			static char const * const macro[] = {
			"SUCCESS",
			"EPERM",           "ENOENT",          "ESRCH",           "EINTR",
			"EIO",             "ENXIO",           "E2BIG",           "ENOEXEC",
			"EBADF",           "ECHILD",          "EAGAIN",          "ENOMEM",
			"EACCES",          "EFAULT",          "ENOTBLK",         "EBUSY",
			"EEXIST",          "EXDEV",           "ENODEV",          "ENOTDIR",
			"EISDIR",          "EINVAL",          "ENFILE",          "EMFILE",
			"ENOTTY",          "ETXTBSY",         "EFBIG",           "ENOSPC",
			"ESPIPE",          "EROFS",           "EMLINK",          "EPIPE",
			"EDOM",            "ERANGE",          "EDEADLK",         "ENAMETOOLONG",
			"ENOLCK",          "ENOSYS",          "ENOTEMPTY",       "ELOOP",
			"?",               "ENOMSG",          "EIDRM",           "ECHRNG",
			"EL2NSYNC",        "EL3HLT",          "EL3RST",          "ELNRNG",
			"EUNATCH",         "ENOCSI",          "EL2HLT",          "EBADE",
			"EBADR",           "EXFULL",          "ENOANO",          "EBADRQC",
			"EBADSLT",         "?",               "EBFONT",          "ENOSTR",
			"ENODATA",         "ETIME",           "ENOSR",           "ENONET",
			"ENOPKG",          "EREMOTE",         "ENOLINK",         "EADV",
			"ESRMNT",          "ECOMM",           "EPROTO",          "EMULTIHOP",
			"EDOTDOT",         "EBADMSG",         "EOVERFLOW",       "ENOTUNIQ",
			"EBADFD",          "EREMCHG",         "ELIBACC",         "ELIBBAD",
			"ELIBSCN",         "ELIBMAX",         "ELIBEXEC",        "EILSEQ",
			"ERESTART",        "ESTRPIPE",        "EUSERS",          "ENOTSOCK",
			"EDESTADDRREQ",    "EMSGSIZE",        "EPROTOTYPE",      "ENOPROTOOPT",
			"EPROTONOSUPPORT", "ESOCKTNOSUPPORT", "EOPNOTSUPP",      "EPFNOSUPPORT",
			"EAFNOSUPPORT",    "EADDRINUSE",      "EADDRNOTAVAIL",   "ENETDOWN",
			"ENETUNREACH",     "ENETRESET",       "ECONNABORTED",    "ECONNRESET",
			"ENOBUFS",         "EISCONN",         "ENOTCONN",        "ESHUTDOWN",
			"ETOOMANYREFS",    "ETIMEDOUT",       "ECONNREFUSED",    "EHOSTDOWN",
			"EHOSTUNREACH",    "EALREADY",        "EINPROGRESS",     "ESTALE",
			"EUCLEAN",         "ENOTNAM",         "ENAVAIL",         "EISNAM",
			"EREMOTEIO",       "EDQUOT",          "ENOMEDIUM",       "EMEDIUMTYPE",
			"ECANCELED",       "ENOKEY",          "EKEYEXPIRED",     "EKEYREVOKED",
			"EKEYREJECTED",    "EOWNERDEAD",      "ENOTRECOVERABLE" };
			return stream << macro[error.errnum];
		}

	}      error;

	public:

	explicit system_error(int errnum) throw()
	: runtime_error (::strerror(errnum)),
	  error (errnum)
	{}

	const error_code code() const
	{
		return error;
	}
};

int		main(int argc, const char *argv[])
{
	class system_error e(5);

	std::cout << e.code() << std::endl;
	return EXIT_SUCCESS;
}
