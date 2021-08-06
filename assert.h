#pragma once



#if defined(DBG) || defined(LOAD_TEST) || defined(GUITEST)


//#define assert(exp) ((!(exp)) ? (RtlAssert(#exp, __FILE__, __LINE__, NULL),FALSE) : TRUE)

#define assert(exp) if(!(exp)) __debugbreak();


#else

#define assert(...)

#endif


#define ASSERT  assert
