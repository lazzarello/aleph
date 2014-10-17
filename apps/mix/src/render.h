#ifndef _ALEPH_APP_MIX_RENDER_H_
#define _ALEPH_APP_MIX_RENDER_H_


// init
extern void render_init(void);

// update
extern void render_update(void);

// render amplitude
extern void render_amp(u8 ch);

// render mute
extern void render_mute(u8 ch);

// startup state
extern void render_startup(void);

#endif // h guard