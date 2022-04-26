#ifndef _GHOST_H_

#define _GHOST_H_

#include <memory>
#include <assert.h>

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

#define GHOST_MAKE_SMART_POINTER(type, ...) std::make_shared<type>(__VA_ARGS__);

#define GHOST_SMARTPOINTER_CAST(castType, p) std::dynamic_pointer_cast<castType>(p);

#define GHOST_MOVE(arg) std::move(arg)

#define GHOST_FORWARD(func, ...) std::forward<func>(__VA_ARGS__)

#define SAFE_RELEASE(p) \
{\
    if (p) \
    {\
        p->Release();\
        p = nullptr;\
    }\
}

#define SAFE_DELETE(p) \
{\
    if (p) \
    {\
        delete p;\
        p = nullptr;\
    }\
}

#define SAFE_DELETE_ARRAY(p) \
{\
    if (p) \
    {\
        delete[] p;\
        p = nullptr;\
    }\
}

#define GHOST_INVALID_INDEX -1


#endif // !_GHOST_H_
