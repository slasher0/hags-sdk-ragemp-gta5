#pragma once

class IPath;

class cEngFILE
{
public:
	enum e_arg
	{
		e_read	= 0,
		e_write	= 1,
		e_connect_read	= (0 | 2),
		e_connect_write	= (1 | 2),
		e_flush,
		e_size,
		e_seek,
		e_close,
		e_open_read			= (8			), // ret 1=ok
		e_open_read_ipath	= (8|2			),
		e_open_write		= (8   | 1		),
		e_open_write_ipath	= (8|2 | 1		),
		e_create_write		= (8   | 1 | 4	),
		e_create_write_ipath= (8|2 | 1 | 4	),
		e_release = 16,
	};
	struct t_arg
	{
		union
		{
			HANDLE hFile;
			PVOID Buff;
			PCWSTR Path;
			const IPath *PathI;
			LONG SeekPos;
		};
		union
		{
			ULONG	BuffSize;
			bool	ReadAlloc;
			bool	FromEnd;
		};
		ULONG Unused;
	};
	virtual SIZE_T Act(const t_arg *, e_arg) = 0;

	inline SIZE_T ConnectRead(HANDLE hFile, const bool bLoad = 0)
	{
		t_arg arg;
		arg.hFile = hFile;
		arg.ReadAlloc = bLoad;
		return Act(&arg, e_connect_read);
	}

	inline BOOL ConnectWrite(HANDLE hFile, const SIZE_T AllocBuff = 0)
	{
		t_arg arg;
		arg.hFile = hFile;
		arg.BuffSize = (ULONG)AllocBuff;
		return (BOOL)Act(&arg, e_connect_write);
	}

	inline SIZE_T OpenRead(PCWSTR Path, const bool bLoad = 0)
	{
		t_arg arg;
		arg.Path = Path;
		arg.ReadAlloc = bLoad;
		return Act(&arg, e_open_read);
	}

	inline SIZE_T OpenRead(const IPath* Path, const bool bLoad = 0)
	{
		t_arg arg;
		arg.PathI = Path;
		arg.ReadAlloc = bLoad;
		return Act(&arg, e_open_read_ipath);
	}

	inline BOOL OpenWrite(PCWSTR Path, const SIZE_T AllocBuff = 0)
	{
		t_arg arg;
		arg.Path = Path;
		arg.BuffSize = (ULONG)AllocBuff;
		return (BOOL)Act(&arg, e_open_write);
	}

	inline BOOL OpenWrite(const IPath* Path, const SIZE_T AllocBuff = 0)
	{
		t_arg arg;
		arg.PathI = Path;
		arg.BuffSize = (ULONG)AllocBuff;
		return (BOOL)Act(&arg, e_open_write_ipath);
	}

	inline BOOL CreateWrite(PCWSTR Path, const SIZE_T AllocBuff = 0)
	{
		t_arg arg;
		arg.Path = Path;
		arg.BuffSize = (ULONG)AllocBuff;
		return (BOOL)Act(&arg, e_create_write);
	}

	inline BOOL CreateWrite(const IPath* Path, const SIZE_T AllocBuff = 0)
	{
		t_arg arg;
		arg.PathI = Path;
		arg.BuffSize = (ULONG)AllocBuff;
		return (BOOL)Act(&arg, e_create_write_ipath);
	}

	// ret bytes
	inline SIZE_T Write(LPCVOID Buff, SIZE_T Size)
	{
		t_arg arg;
		arg.Buff = (PVOID)Buff;
		arg.BuffSize = (ULONG)Size;
		return Act(&arg, e_write);
	}

	// ret bytes
	inline SIZE_T Read(PVOID Buff, SIZE_T Size)
	{
		t_arg arg;
		arg.Buff = Buff;
		arg.BuffSize = (ULONG)Size;
		return Act(&arg, e_read);
	}

	inline SIZE_T Seek(LONG Offset, bool FromEnd)
	{
		t_arg arg;
		arg.SeekPos = Offset;
		arg.FromEnd = FromEnd;
		return Act(&arg, e_seek);
	}

	inline BOOL Flush()	{	return (BOOL)Act(NULL, e_flush);	}
	inline BOOL Close()	{	return (BOOL)Act(NULL, e_close);	}

	inline SIZE_T Size()	{	return Act(NULL, e_size);	}

	inline PUCHAR Data()	{	return (PUCHAR)Act(NULL, (e_arg)0x70);	}

	inline VOID Release()
	{
		Act(NULL, e_release);
	}
};


