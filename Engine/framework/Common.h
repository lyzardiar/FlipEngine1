#ifndef __COMMON_H__
#define __COMMON_H__

extern const char* GAME_NAME;

void Common_Init();

void Common_Frame();

void Common_Printf(const char* fmt, ...);

void Common_Error(const char* fmt, ...);

void Common_Quit();

#endif /* !__COMMON_H__ */
