#ifndef __I_MACROS_H__
#define __I_MACROS_H__


#define SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)
#define SAFE_FREE(p)                do { if(p) { free(p); (p) = 0; } } while(0)
#define SAFE_RELEASE(p)            do { if(p) { (p)->release(); } } while(0)
#define SAFE_RELEASE_NULL(p)        do { if(p) { (p)->release(); (p) = 0; } } while(0)
#define SAFE_RETAIN(p)            do { if(p) { (p)->retain(); } } while(0)
#define BREAK_IF(cond)            if(cond) break


#endif