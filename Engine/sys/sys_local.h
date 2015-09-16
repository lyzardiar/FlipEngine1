#ifndef __SYS_LOCAL__
#define __SYS_LOCAL__

typedef struct {
	int			width;
	int			height;
	bool			fullScreen;
	bool			stereo;
	int			displayHz;
	int			multiSamples;
} glimpParms_t;

void	Sys_CreateGLWindow(glimpParms_t);

#endif /* !__SYS_LOCAL__ */
