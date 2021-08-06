#pragma once


class PROTOTYPE cConsole
{
public:
	virtual void Printf(PCSTR Fmt, ...) = 0;
	virtual void Write(PCSTR Text, BOOL bMsg = 0) = 0;

	enum e_msg
	{
		e_msg_ok,
		e_msg_fail,
		e_msg_nothing,
	//str:
		e_msg_saved,
		e_err_create,
		e_err_open,
		e_err_encoding,
	};
	virtual void Write(e_msg, PCWSTR str = NULL) = 0;
};

