#ifndef _GHOST_H_

#define _GHOST_H_

#ifdef GHOST_LIB_STATIC
#define GHOST_API
#else
#ifdef GHOST_EXPORT
#define GHOST_API __declspec(dllexport)
#else
#define GHOST_API __declspec(dllimport)
#endif
#endif

#define DECLAR_SMART_POINTER(obj)  \
using obj##Ptr = std::shared_ptr<obj>;  \
using obj##WeakPtr = std::weak_ptr<obj>; \
using obj##UniquePtr = std::unique_ptr<obj>;


#endif // !_GHOST_H_
