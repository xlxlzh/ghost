#ifndef _GHOST_H_

#define _GHOST_H_

#include <memory>

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

#define GHOST_SMARTPOINTER_CAST(castType, p) std::dynamic_pointer_cast<castType>(p);

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


#endif // !_GHOST_H_
