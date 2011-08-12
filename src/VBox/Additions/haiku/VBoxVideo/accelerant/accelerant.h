
#ifndef _ACCELERANT_H
#define _ACCELERANT_H

#include <Accelerant.h>
#include "../common/VBoxVideo_common.h"

struct AccelerantInfo {
	int deviceFD;
	bool isClone;
	
	SharedInfo* sharedInfo;
	area_id sharedInfoArea;
};
extern AccelerantInfo gInfo;

// general
status_t vboxvideo_init_accelerant(int fd);
ssize_t vboxvideo_accelerant_clone_info_size(void);
void vboxvideo_get_accelerant_clone_info(void *data);
status_t vboxvideo_clone_accelerant(void *data);
void vboxvideo_uninit_accelerant(void);
status_t vboxvideo_get_accelerant_device_info(accelerant_device_info *adi);
sem_id vboxvideo_accelerant_retrace_semaphore(void);

// modes & constraints
uint32 vboxvideo_accelerant_mode_count(void);
status_t vboxvideo_get_mode_list(display_mode *dm);
status_t vboxvideo_propose_display_mode(display_mode *target, const display_mode *low, const display_mode *high);
status_t vboxvideo_set_display_mode(display_mode *modeToSet);
status_t vboxvideo_get_display_mode(display_mode *currentMode);
status_t vboxvideo_get_edid_info(void *info, size_t size, uint32 *_version);
status_t vboxvideo_get_frame_buffer_config(frame_buffer_config *config);
status_t vboxvideo_get_pixel_clock_limits(display_mode *dm, uint32 *low, uint32 *high);
status_t vboxvideo_move_display(uint16 hDisplayStart, uint16 vDisplayStart);
status_t vboxvideo_get_timing_constraints(display_timing_constraints *dtc);
void vboxvideo_set_indexed_colors(uint count, uint8 first, uint8 *colorData, uint32 flags);

// DPMS
uint32 vboxvideo_dpms_capabilities(void);
uint32 vboxvideo_dpms_mode(void);
status_t vboxvideo_set_dpms_mode(uint32 dpmsFlags);

// cursor
status_t vboxvideo_set_cursor_shape(uint16 width, uint16 height, uint16 hotX, uint16 hotY, uint8 *andMask, uint8 *xorMask);
void vboxvideo_move_cursor(uint16 x, uint16 y);
void vboxvideo_show_cursor(bool is_visible);

// accelerant engine
uint32 vboxvideo_accelerant_engine_count(void);
status_t vboxvideo_acquire_engine(uint32 capabilities, uint32 maxWait, sync_token *st, engine_token **et);
status_t vboxvideo_release_engine(engine_token *et, sync_token *st);
void vboxvideo_wait_engine_idle(void);
status_t vboxvideo_get_sync_token(engine_token *et, sync_token *st);
status_t vboxvideo_sync_to_token(sync_token *st);

// 2D acceleration
void vboxvideo_screen_to_screen_blit(engine_token *et, blit_params *list, uint32 count);
void vboxvideo_fill_rectangle(engine_token *et, uint32 color, fill_rect_params *list, uint32 count);
void vboxvideo_invert_rectangle(engine_token *et, fill_rect_params *list, uint32 count);
void vboxvideo_fill_span(engine_token *et, uint32 color, uint16 *list, uint32 count);

#endif	/* _ACCELERANT_PROTOS_H */
