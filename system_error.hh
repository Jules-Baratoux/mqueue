using std::runtime_error;
using std::ostream;

// -----------------------------------------------------------------------------
// This  class  defines  the type of  objects  thrown  as  exceptions  to report
// conditions  originating during  runtime  from the operating system  or  other
// low-level application program interfaces which have an associated  error_code
// see also: C++ 11 std::system_error
class system_error : public runtime_error
{
	public:

	// constructs a system_error using the error code passed in argument
	// sets what arg to an appropriate description
	// see also: strerror(3)
	explicit system_error(int errnum) throw();

	// return an errno based error_code
	const error_code code() const;

	private:
	class error_code
	{
		public:
		explicit error_code(int errnum);
		operator int (void) const;
		friend ostream&	operator << (ostream& stream,
					     const error_code& error);

		private:
		int	errnum;
	}      error;
};
