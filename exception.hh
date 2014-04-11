#pragma once
#include <stdexcept>

using std::ostream;
using std::string;

class error_code
{
	// error number
	int	errnum;

	public:
	error_code(int errnum);
	operator int (void) const;
	operator const string& (void) const;
	friend ostream&	operator << (ostream& stream, const error_code& error);

	// return errnum MACRO's
	const string& macro(void) const;
	static inline const string& macro(int errnum);
};
