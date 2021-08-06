#ifndef _CLIST_H_
#define _CLIST_H_

//#include <type_traits>

#ifndef LIST_IT_PTR
#define LIST_IT_PTR(TT) TT*
#endif

//#ifdef _AMD64_
//	#pragma pack(push, 8)
//#else
//	#pragma pack(push, 4)
//#endif

template <class TT>
class cListItem
{
public:

	TT	Data;

	LIST_IT_PTR( cListItem<TT> )	Next;
	LIST_IT_PTR( cListItem<TT> )	Prev;

	inline static void Free(cListItem *pItem)
	{
		delete pItem;
	}
	inline static cListItem *Alloc()
	{
		return new cListItem;
	}
};

//#pragma pack(pop)


template < class T, class ALC = cListItem<T> >
class cList
{
public:
	typedef ALC TItem;
	typedef ALC *PItem;
	typedef T t_it;
	typedef T *p_it;

private:
	SIZE_T	m_uItems;
	TItem *	m_pRoot;
	TItem *	m_pTail;

public:
	__forceinline void Init()
	{
		m_uItems = 0;
		m_pRoot = m_pTail = 0;
	}
	__forceinline cList()
	{
		Init();
	}
	~cList()
	{
		Clear();
	}

public:
	inline static T* Alloc()
	{
		PItem pItem = ALC::Alloc();
		return &pItem->Data;
	}

	static PItem CutOrAlloc(cList<T,ALC> & trash)
	{
		PItem pItem = trash.m_pTail;
		if(!pItem)
			return ALC::Alloc();
		trash.Cut(&pItem->Data);
		return pItem;
	}

public:
	T * New()
	{
		PItem pItem = ALC::Alloc();
		return pItem ? AddItemImpl(pItem) : NULL;
	}

	T * New(cList<T,ALC> & trash)
	{
		PItem pItem = CutOrAlloc(trash);
		return pItem ? AddItemImpl(pItem) : NULL;
	}


	T * Add(T const & pNew)
	{
		PItem pItem = ALC::Alloc();
		if(pItem)
		{
			pItem->Data = pNew;
			return AddItemImpl(pItem);
		}
		return NULL;
	}

	T * Add(T const & pNew, cList<T,ALC> & trash)
	{
		PItem pItem = CutOrAlloc(trash);
		if(pItem)
		{
			pItem->Data = pNew;
			return AddItemImpl(pItem);
		}
		return NULL;
	}


	inline T * AddPtr(T *Ptr)
	{
		return Ptr ? AddItemImpl( (PItem)Ptr ) : Ptr;
	}

	inline T * AddRootPtr(T *Ptr)
	{
		return m_uItems ? MergeBefore(&m_pRoot->Data, Ptr) : AddItemImpl( (PItem)Ptr );
	}

//private:
	T * AddItemImpl(PItem pItem)
	{
		pItem->Next = NULL;
		pItem->Prev = m_pTail;

		if(m_pRoot)
			m_pTail->Next = pItem;
		else
			m_pRoot = pItem;

		m_pTail = pItem;
		m_uItems++;

		return &pItem->Data;
	}

public:
	inline T * Add_Root(T const & New)
	{
		return m_uItems ? InsertBefore(&m_pRoot->Data, New) : Add(New);
	}

	inline T * New_Root()
	{
		return m_uItems ? InsertBefore(&m_pRoot->Data) : New();
	}


	bool Delete(const T & Data)
	{
		PItem pItem = FindItem(Data);
		if(!pItem)
			return false;
		Delete(&pItem->Data);
		return true;
	}

	void Delete(T * pData)
	{
		if(Cut(pData))
			ALC::Free( (PItem)pData );
	}

	void Delete(T * pData, cList<T,ALC> & trash)
	{
		if(Cut(pData))
			trash.AddItemImpl( (PItem)pData );
	}


	bool Delete_Check(T * pData)
	{
		if(!m_uItems)
			return false;
		if(Index(pData) == ~0)
			return false;
		if(!Cut(pData))
			return false;
		ALC::Free((PItem)pData);
		return true;
	}


	T * GetData(SIZE_T Index) const
	{
		if(Index >= m_uItems)
			return NULL;

		SIZE_T Count = 0;

		for(PItem pItem = m_pRoot; pItem; pItem = pItem->Next)
		{
			if(Count == Index)
				return &pItem->Data;
			Count++;
		}
		return NULL;
	}

	inline T * operator [] (SIZE_T Index) const
    {
		return GetData(Index);
	}


	T * Cut(T * pData)
	{
		if(!m_uItems)
			return NULL;
		m_uItems--;

		PItem pItem = (PItem)pData;

		PItem pNext = pItem->Next;
		PItem pPrev = pItem->Prev;

		if(pItem == m_pRoot)
			m_pRoot = pNext;
		else
			pPrev->Next = pNext;

		if(pItem == m_pTail)
			m_pTail = pPrev;
		else
			pNext->Prev = pPrev;

		pItem->Next = NULL;
		pItem->Prev = NULL;

		return &pItem->Data;
	}


	T * SplitList(T * pStart, T * pEnd)
	{
		if(pStart == pEnd)
			return Cut(pStart);

		PItem start = (PItem)pStart;
		PItem end = (PItem)pEnd;

		SIZE_T iItems = 0;

		PItem tmp = start;
		while(tmp)
		{
			if(iItems++ > m_uItems)
				return NULL; // end not found
			if(tmp == end)
				break;
			tmp = tmp->Next;
		}
		if(!tmp)
			return NULL;

		if(m_uItems >= iItems)
			m_uItems -= iItems;

		if(start == m_pRoot)
			m_pRoot = end->Next;
		else
			start->Prev->Next = end->Next;

		if(end == m_pTail)
			m_pTail = start->Prev;
		else
			end->Next->Prev = start->Prev;

		start->Prev = NULL;
		end->Next = NULL;

		return &start->Data;
	}

	T * MergeListAfter(T * pData1, T * pData2)
	{
		PItem start = (PItem)pData2;
		PItem end = start;

		SIZE_T iItems = 0;

		while(end)
		{
			iItems++;
			if(!end->Next)
				break;
			end = end->Next;
		}

		if(start == end)
			return MergeAfter(pData1, pData2);

		PItem pItem1 = (PItem)pData1;

		end->Next = pItem1->Next;
		start->Prev = pItem1;

		if(pItem1->Next)
			pItem1->Next->Prev = start;

		pItem1->Next = start;

		if(!m_pTail || m_pTail == pItem1)
			m_pTail = end;

		m_uItems += iItems;

		return &pItem1->Data;
	}


	T * MergeList(T * pData)
	{
		PItem start = (PItem)pData;

		start->Prev = m_pTail;

		if(!m_pRoot)
			m_pRoot = start;
		else
			m_pTail->Next = start;

		for(PItem end = start, next; ; end = next)
		{
			m_uItems++;

			if(!(next = end->Next))
			{
				m_pTail = end;
				break;
			}
		}

		return &start->Data;
	}


	T * MergeListBefore(T * pData1, T * pData2)
	{
		PItem start = (PItem)pData2;
		PItem end = start;

		LONG_PTR iItems = 0;

		while(end)
		{
			iItems++;

			if(!end->Next)
				break;

			end = end->Next;
		}

		if(start == end)
			return MergeBefore(pData1, pData2);

		PItem pItem1 = (PItem)pData1;

		end->Next = pItem1;
		start->Prev = pItem1->Prev;

		if(pItem1->Prev)
			pItem1->Prev->Next = start;

		pItem1->Prev = end;

		if(!m_pRoot || m_pRoot == pItem1)
			m_pRoot = start;

		m_uItems += iItems;

		return &start->Data;
	}

protected:

//	PItem FindItem(const std::conditional_t<std::is_pointer<T>::value, std::remove_pointer_t<T> const *, T> Data) const
	PItem FindItem(const T & Data) const
	{
		for(PItem pItem = m_pRoot; pItem; pItem = pItem->Next)
		{
			if(pItem->Data == Data)
				return pItem;
		}
		return NULL;
	}

public:

//	inline T * Find(const std::conditional_t<std::is_pointer<T>::value, std::remove_pointer_t<T> const *, T> Data) const
	T * Find(const T & Data) const
	{
		PItem pItem = FindItem(Data);
		return pItem ? &pItem->Data : NULL;
	}

//	T * Find(const T & Data) const
//	{
//		for(PItem pItem = m_pRoot; pItem; pItem = pItem->Next)
//		{
//			if(pItem->Data == Data)
//				return &pItem->Data;
//		}
//		return NULL;
//	}

	template <class TV, ULONG OFS>
		T * Find_ByVar(TV Var) const
		{
			for(PItem pItem = m_pRoot; pItem; pItem = pItem->Next)
			{
				if(Var == *(TV*)(((PUCHAR)&pItem->Data)+OFS))
					return &pItem->Data;
			}
			return NULL;
		}


	SIZE_T Index(const T * Data) const
	{
		if(Data)
		{
			SIZE_T i = 0;

			for(PItem pItem = m_pRoot; pItem; pItem = pItem->Next)
			{
				if( (&pItem->Data) == Data )
					return i;
				i++;
			}
		}
		return ~0;
	}

	SIZE_T Index(const T & Data) const
	{
		SIZE_T i = 0;

		for(PItem pItem = m_pRoot; pItem; pItem = pItem->Next)
		{
			if(pItem->Data == Data)
				return i;
			i++;
		}
		return ~0;
	}

	__forceinline SIZE_T Size() const
	{
		return m_uItems;
	}

	__forceinline BOOL Empty() const
	{
		return (m_uItems == 0);
	}

	inline SIZE_T CountLinks() const
	{
		SIZE_T i = 0;
		for(PItem pItem = m_pRoot; pItem; pItem = pItem->Next)
			i++;
		return i;
	}

	bool Move(T * pDst, T * pItem)
	{
		if(pDst == pItem)
			return true;
		LONG_PTR iDst = Index(pDst);
		if(iDst == ~0)
			return false;
		LONG_PTR iItem = Index(pItem);
		if(iItem == ~0)
			return false;
		if(!Cut(pItem))
			return false;
		if(iDst < iItem)
			MergeBefore(pDst, pItem);
		else
			MergeAfter(pDst, pItem);
		return true;
	}

	bool Swap_Check(T * pData1, T * pData2)
	{
		if(Index(pData1) == ~0)	return false;
		if(Index(pData2) == ~0)	return false;
		return Swap(pData1, pData2);
	}

	bool Swap(T * pData1, T * pData2)
	{
		PItem a1 = (PItem)pData1;
		PItem a2 = (PItem)pData2;

		PItem a1p = a1->Prev;
		PItem a2p = a2->Prev;
		PItem a1p_o = a1p;//a1->Prev;
		PItem a2p_o = a2p;//a2->Prev;

		PItem a1n = a1->Next;
		PItem a2n = a2->Next;
		PItem a1n_o = a1n;//a1->Next;
		PItem a2n_o = a2n;//a2->Next;

		if(a1->Next == a2) // ...->[ a1 ]->[ a2 ]->...
		{
			//if(a1p)
			//	a1p->Next = NULL;

			//if(a2n)
			//	a2n->Prev = NULL;

			a2->Next = a1;
			a1->Next = a2n_o;

			a1->Prev = a2;
			a2->Prev = a1p_o;

			if(a1p)
				a1p->Next = a2;

			if(a2n)
				a2n->Prev = a1;
		}
		else if(a2->Next == a1) // ...->[ a2 ]->[ a1 ]->...
		{
			//if(a2p)
			//	a2p->Next = NULL;

			//if(a1n)
			//	a1n->Prev = NULL;

			a1->Next = a2;
			a2->Next = a1n_o;

			a2->Prev = a1;
			a1->Prev = a2p_o;

			if(a2p)
				a2p->Next = a1;

			if(a1n)
				a1n->Prev = a2;
		}
		else // ...->[ a1 ]->...->[ a2 ]->...
		{
			//if(a2p)
			//	a2p->Next = NULL;

			//if(a1n)
			//	a1n->Prev = NULL;

			if(a1p)
				a1p->Next = a2;

			if(a2n)
				a2n->Prev = a1;

			if(a2p)
				a2p->Next = a1;

			if(a1n)
				a1n->Prev = a2;

			//a1->Next = NULL;
			a2->Next = a1n_o;
			a1->Next = a2n_o;

			//a2->Prev = NULL;
			a1->Prev = a2p_o;
			a2->Prev = a1p_o;
		}

		if(m_pTail == a2) m_pTail = a1; else
		if(m_pTail == a1) m_pTail = a2;

		if(m_pRoot == a2) m_pRoot = a1; else
		if(m_pRoot == a1) m_pRoot = a2;

		return true;
	}


	void Clear()
	{
		PItem pos = m_pRoot;

		m_uItems = 0;
		m_pRoot = m_pTail = NULL;

		while(pos)
		{
			PItem p = pos;
			pos = pos->Next;

			ALC::Free(p);
		}
	}

//------------------------------------------------------------

	T * InsertAfter(T * pData, T const & New)
	{
		PItem pNew = ALC::Alloc();
		pNew->Data = New;
		return MergeAfter(pData, &pNew->Data);
	}

	T * InsertBefore(T * pData, T const & New)
	{
		PItem pNew = ALC::Alloc();
		pNew->Data = New;
		return MergeBefore(pData, &pNew->Data);
	}

//------------------------------------------------------------

	T * InsertAfter(T * pData)
	{
		PItem pNew = ALC::Alloc();
		return MergeAfter(pData, &pNew->Data);
	}

	T * InsertBefore(T * pData)
	{
		PItem pNew = ALC::Alloc();
		return MergeBefore(pData, &pNew->Data);
	}

//------------------------------------------------------------

	T * MergeAfter(const T * pData1, const T * pData2)
	{
		PItem pItem1 = (PItem)pData1;
		PItem pItem2 = (PItem)pData2;

		pItem2->Next = pItem1->Next;
		pItem2->Prev = pItem1;

		if(pItem1->Next)
			pItem1->Next->Prev = pItem2;

		pItem1->Next = pItem2;

		if(!m_pTail || m_pTail == pItem1)
			m_pTail = pItem2;

		m_uItems++;

		return &pItem2->Data;
	}

	T * MergeBefore(T * pData1, T * pData2)
	{
		PItem pItem1 = (PItem)pData1;
		PItem pItem2 = (PItem)pData2;

		pItem2->Next = pItem1;
		pItem2->Prev = pItem1->Prev;

		if(pItem1->Prev)
			pItem1->Prev->Next = pItem2;

		pItem1->Prev = pItem2;

		if(!m_pRoot || m_pRoot == pItem1)
			m_pRoot = pItem2;

		m_uItems++;

		return &pItem2->Data;
	}

	inline T *GetNextAndDeleteThis(T *pData)
	{
		T *Ret = GetNext(pData);
		Delete(pData);
		return Ret;
	}
//------------------------------------------------------------

public:
	__forceinline T *GetFirst() const
	{
		return m_pRoot ? &m_pRoot->Data : NULL;
	}
	__forceinline T *GetLast() const
	{
		return m_pTail ? &m_pTail->Data : NULL;
	}
	static inline T *GetNext(const T *pData)
	{
		PItem p = (PItem)pData;
		p = p->Next;
		return p ? &p->Data : NULL;
	}
	static inline T *GetPrev(const T *pData)
	{
		PItem p = (PItem)pData;
		p = p->Prev;
		return p ? &p->Data : NULL;
	}
};



//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------

template <class T, ULONG L>
struct cArray
{
	ULONG	_Count;
	T		_Array[L];

public:
	cArray() : _Count(0)
	{}
	inline void Clear()
	{
		_Count = 0;
	}
	inline void Clear_MEMSET()
	{
		memset0(this,sizeof(*this));
	}
	inline bool Add(const T& Data)
	{
		if(_Count >= L)
			return 0;
		_Array[ _Count++ ] = Data;
		return 1;
	}
	inline T* New()
	{
		if(_Count >= L)
			return NULL;
		return &_Array[ _Count++ ];
	}

	inline T* GetLast()
	{
		return _Count ? &_Array[ _Count - 1 ] : NULL;
	}

	inline bool Del(T *Ptr)
	{
		if(Ptr < _Array)
			return 0;
		T *pEnd = &_Array[_Count];
		if(Ptr >= pEnd)
			return 0;
		_Count--;
		T *pNext = Ptr + 1;
		if(pNext < pEnd)
			memcpy( (PUCHAR)Ptr, (PUCHAR)pNext, (SIZE_T)pEnd - (SIZE_T)pNext ); // __movsb
		return 1;
	}
	inline bool Del_At(SIZE_T i)
	{
		return Del( &_Array[i] );
	}
	inline bool Del_Find(const T& Data)
	{
		for(ULONG i = 0; i < _Count; i++)
			if(_Array[i] == Data)
				return Del_At(i);
		return 0;
	}
	inline T &operator [] (SIZE_T i)
	{
		return _Array[i];
	}
	inline ULONG Size() const
	{
		return _Count;
	}
	inline ULONG MaxSize() const
	{
		return L;
	}
	inline T *Data()
	{
		return _Array;
	}

	template <class TV, const ULONG_PTR OFS>
		T *Find_ByVar(const TV Var)
		{
			for(ULONG i = 0; i < _Count; i++)
				if(*(TV*)(((ULONG_PTR)(&_Array[i])) + OFS) == Var)
					return &_Array[i];
			return 0;
		}
	__forceinline
	bool Find(const T& Var) const
	{
		for(ULONG i = 0; i < _Count; i++)
			if(_Array[i] == Var)
				return 1;
		return 0;
	}
};

//------------------------------------------------------------

template <class T>
struct cArrayDynamic
{
private:
	ULONG	_Count;
	ULONG	_Max;
	T *		_Array;

public:
	inline cArrayDynamic()
	{
		*(PULONG64)&_Count = 0;
		_Array = NULL;
	}
	inline ~cArrayDynamic()
	{
		PUCHAR pp = (PUCHAR)_Array;
		if(pp)
			delete pp;
		//if(_Array)
		//	delete [] _Array;
	}
	inline VOID Destroy()
	{
		PUCHAR pp = (PUCHAR)_Array;
		memset0(this,sizeof(*this));
		if(pp)
			delete pp;
	}
	void Init(ULONG i)
	{
		if(i <= _Max)
			return;
		T *New = (T *) new UCHAR [ i * sizeof(T) ];
		if(_Array)
		{
			memcpy((PUCHAR)New, (PUCHAR)_Array, _Count * sizeof(T)); // __movsb
			PUCHAR mem = (PUCHAR)_Array;
			delete mem;
		}
		_Max = i;
		_Array = New;
	}
	inline void Clear()
	{
		_Count = 0;
	}
	void Clear_MEMSET()
	{
		ULONG sz = _Count * sizeof(T);
		_Count = 0;
		if(sz && _Array)
			memset(_Array, 0, sz);
	}
	void Add(const T& Data)
	{
		if(_Count >= _Max)
			Init( _Max ? (_Max << 1) : 1 );
		_Array[_Count++] = Data;
	}
	T* New()
	{
		if(_Count >= _Max)
			Init( _Max ? (_Max << 1) : 1 );
		return &_Array[_Count++];
	}

	__forceinline bool IsInside(T *Ptr)
	{
		if(Ptr < _Array || Ptr >= &_Array[_Count])
			return 0;
		return 1;
	}

	inline bool Del(T *Ptr)
	{
		if(Ptr < _Array)
			return 0;
		T *pEnd = &_Array[_Count];
		if(Ptr >= pEnd)
			return 0;
		_Count--;
		T *pNext = Ptr + 1;
		if(pNext < pEnd)
			memcpy((PUCHAR)Ptr, (PUCHAR)pNext, (SIZE_T)pEnd - (SIZE_T)pNext ); // __movsb
		return 1;
	}
	inline bool Del_At(ULONG i)
	{
		return Del( &_Array[i] );
	}

	bool Del_Find(const T& Data)
	{
		for(ULONG i = 0; i < _Count; i++)
			if(_Array[i] == Data)
				return Del_At(i);
		return 0;
	}
	inline bool Delete(const T& Data)
	{
		return Del_Find(Data);
	}
	inline T &operator [] (ULONG i)
	{
		return _Array[i];
	}
	inline const T &operator [] (ULONG i) const
	{
		return _Array[i];
	}
	inline ULONG Size() const
	{
		return _Count;
	}
	inline T *Data() const
	{
		return _Array;
	}
	template <class TV, const ULONG_PTR OFS>
		T *Find_ByVar(const TV Var)
		{
			for(ULONG i = 0; i < _Count; i++)
				if(*(TV*)(((ULONG_PTR)(&_Array[i])) + OFS) == Var)
					return &_Array[i];
			return 0;
		}

	bool Find(const T& Data) const
	{
		for(ULONG i = 0; i < _Count; i++)
			if(_Array[i] == Data)
				return 1;
		return 0;
	}
};

//------------------------------------------------------------

template<class T, ULONG hMAX, class ID_TYPE = ULONG_PTR>
struct qArray
{
	T *_List[ hMAX ];

	__forceinline VOID Erase()
	{
		memset0(_List, sizeof(_List));
	}
	__forceinline qArray()
	{
		Erase();
	}

	__forceinline static ULONG GetIndex(ID_TYPE id)
	{
		ULONG i = (ULONG) (((ULONG_PTR)id) % hMAX);
		return i;
	}

	inline T * Add(T *Ptr, ID_TYPE id)
	{
		Ptr->__Index = id;
		ULONG i = GetIndex(id);
		Ptr->__Next = _List[i];
		_List[i] = Ptr;
		return Ptr;
	}
	inline T *Get(ID_TYPE id) const
	{
		ULONG i = GetIndex(id);
		T *p = _List[i];
		while(p)
		{
			if(p->__Index == id)
				break;
			p = p->__Next;
		}
		return p;
	}

	struct tIter
	{
		inline tIter(const qArray<T, hMAX, ID_TYPE> & aa) : a(aa)
		{}
		inline T *First()
		{
			_p = 0;
			_i = 0;
			return Next();
		}
		inline T *Next()
		{
			T *o = _p;
			if(o)
				o = o->__Next;
			if(!o)
			{
				ULONG k = _i;
				for( ; k < hMAX; )
					if(o = a._List[k++])
						break;
				_i = k;
			}
			return _p = o;
		}
		inline void Repair()
		{
			_p = 0;
			if(_i)
				_i--;
		}
	private:
		T *_p;
		const qArray<T, hMAX, ID_TYPE> & a;
		ULONG _i;
	};

	inline T *Remove(ID_TYPE id)
	{
		ULONG i = GetIndex(id);
		T **pp = &_List[i];
		while(*pp)
		{
			T *p = *pp;
			if(p->__Index == id)
			{
				*pp = p->__Next;
				return p;
			}
			pp = &p->__Next;
		}
		return NULL;
	}

	inline T *RemovePtr(T *pit)
	{
		ULONG i = GetIndex( pit->__Index );
		T **pp = &_List[i];
		while(*pp)
		{
			T *p = *pp;
			if(p == pit)
			{
				*pp = p->__Next;
				return p;
			}
			pp = &p->__Next;
		}
		return NULL;
	}

	inline VOID DeleteAll()
	{
		for(ULONG i = 0; i < hMAX; i++)
		{
			T *p = _List[i];
			_List[i] = 0;

			while(p)
			{
				T *n = p->__Next;
				delete p;
				p = n;
			}
		}
	}

	inline VOID DeleteGroup(ULONG igrp)
	{
		for(ULONG i = 0; i < hMAX; i++)
		{
			T **pp = &_List[i];
			while(*pp)
			{
				T *p = *pp;
				if(p->__Group == igrp)
				{
					T *n = p->__Next;
					delete p;
					*pp = n;
					continue;
					//if(!n)
					//	break;
					//p = n;
				}
				pp = &p->__Next;
			}
		}
	}

};







#endif