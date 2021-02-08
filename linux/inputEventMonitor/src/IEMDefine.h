#ifndef __IEMDEFINE_H_
#define __IEMDEFINE_H_

#define INPUT_DEV_DIR   "//dev//input//by-id//"

#ifdef DEBUG
#define IME_LOG(var)
#else
#define IEM_LOG(...) printf(__VA_ARGS__)
#endif


#define IEM_TRUE 1
#define IEM_FALSE 0
#define uint8_t unsigned char
#endif //__IEMDEFINE_H