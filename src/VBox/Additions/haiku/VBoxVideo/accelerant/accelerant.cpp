#include <Accelerant.h>
#include "accelerant.h"
#include <stdio.h>
#include <string.h>

#define TRACE(x...) do { FILE* logfile = fopen("/var/log/vboxvideo.accelerant.log", "a"); fprintf(logfile, x); fflush(logfile); fsync(fileno(logfile)); fclose(logfile); sync(); } while(0)

class AreaCloner {
	public:
		AreaCloner() : fArea(-1) {}
		~AreaCloner() {
			if (fArea >= B_OK)
				delete_area(fArea);
		}

		area_id Clone(const char *name, void **_address, uint32 spec, uint32 protection, area_id sourceArea) {
			fArea = clone_area(name, _address, spec, protection, sourceArea);
			return fArea;
		}
		
		status_t InitCheck() { return fArea < B_OK ? (status_t)fArea : B_OK; }
		void Keep() { fArea = -1; }

	private:
		area_id	fArea;
};

AccelerantInfo gInfo;

static engine_token sEngineToken = {1, 0 /*B_2D_ACCELERATION*/, NULL};

extern "C" void* get_accelerant_hook(uint32 feature, void* data) {
	TRACE("%s\n", __FUNCTION__);
	switch (feature) {
		/* general */
		case B_INIT_ACCELERANT:
			return (void*)vboxvideo_init_accelerant;
		case B_UNINIT_ACCELERANT:
			return (void*)vboxvideo_uninit_accelerant;
		case B_CLONE_ACCELERANT:
			return (void*)vboxvideo_clone_accelerant;
		case B_ACCELERANT_CLONE_INFO_SIZE:
			return (void*)vboxvideo_accelerant_clone_info_size;
		case B_GET_ACCELERANT_CLONE_INFO:
			return (void*)vboxvideo_get_accelerant_clone_info;
		case B_GET_ACCELERANT_DEVICE_INFO:
			return (void*)vboxvideo_get_accelerant_device_info;
		case B_ACCELERANT_RETRACE_SEMAPHORE:
			return (void*)vboxvideo_accelerant_retrace_semaphore;

		/* mode configuration */
		case B_ACCELERANT_MODE_COUNT:
			return (void*)vboxvideo_accelerant_mode_count;
		case B_GET_MODE_LIST:
			return (void*)vboxvideo_get_mode_list;
		case B_PROPOSE_DISPLAY_MODE:
			return (void*)vboxvideo_propose_display_mode;
		case B_SET_DISPLAY_MODE:
			return (void*)vboxvideo_set_display_mode;
		case B_GET_DISPLAY_MODE:
			return (void*)vboxvideo_get_display_mode;
		case B_GET_EDID_INFO:
			return (void*)vboxvideo_get_edid_info;
		case B_GET_FRAME_BUFFER_CONFIG:
			return (void*)vboxvideo_get_frame_buffer_config;
		case B_GET_PIXEL_CLOCK_LIMITS:
			return (void*)vboxvideo_get_pixel_clock_limits;
		case B_MOVE_DISPLAY:
			return (void*)vboxvideo_move_display;
		case B_SET_INDEXED_COLORS:
			return (void*)vboxvideo_set_indexed_colors;
		case B_GET_TIMING_CONSTRAINTS:
			return (void*)vboxvideo_get_timing_constraints;

		/* DPMS */
		case B_DPMS_CAPABILITIES:
			return (void*)vboxvideo_dpms_capabilities;
		case B_DPMS_MODE:
			return (void*)vboxvideo_dpms_mode;
		case B_SET_DPMS_MODE:
			return (void*)vboxvideo_set_dpms_mode;

		/* cursor managment */
		case B_SET_CURSOR_SHAPE:
			return (void*)vboxvideo_set_cursor_shape;
		case B_MOVE_CURSOR:
			return (void*)vboxvideo_move_cursor;
		case B_SHOW_CURSOR:
			return (void*)vboxvideo_show_cursor;

		/* engine/synchronization */
		case B_ACCELERANT_ENGINE_COUNT:
			return (void*)vboxvideo_accelerant_engine_count;
		case B_ACQUIRE_ENGINE:
			return (void*)vboxvideo_acquire_engine;
		case B_RELEASE_ENGINE:
			return (void*)vboxvideo_release_engine;
		case B_WAIT_ENGINE_IDLE:
			return (void*)vboxvideo_wait_engine_idle;
		case B_GET_SYNC_TOKEN:
			return (void*)vboxvideo_get_sync_token;
		case B_SYNC_TO_TOKEN:
			return (void*)vboxvideo_sync_to_token;
	}

	return NULL;
}

status_t vboxvideo_init_accelerant(int fd) {
	TRACE("%s\n", __FUNCTION__);
	
	gInfo.deviceFD = fd;
	gInfo.isClone = false;
	gInfo.sharedInfo = NULL;
	gInfo.sharedInfoArea = -1;
	
	area_id sharedArea;
	if (ioctl(gInfo.deviceFD, VBOXVIDEO_GET_PRIVATE_DATA, &sharedArea, sizeof(area_id)) != 0) {
		TRACE("ioctl failed\n");
		return B_ERROR;
	}

	AreaCloner sharedCloner;
	gInfo.sharedInfoArea = sharedCloner.Clone("vboxvideo shared info",
		(void **)&gInfo.sharedInfo, B_ANY_ADDRESS,
		B_READ_AREA | B_WRITE_AREA, sharedArea);
	status_t status = sharedCloner.InitCheck();
	if (status < B_OK) {
		TRACE("InitCheck failed (%s)\n", strerror(status));
		return status;
	}
	sharedCloner.Keep();

	return B_OK;
}

ssize_t vboxvideo_accelerant_clone_info_size(void) {
	TRACE("%s\n", __FUNCTION__);
	return B_PATH_NAME_LENGTH;
}

void vboxvideo_get_accelerant_clone_info(void *data) {
	TRACE("%s\n", __FUNCTION__);
	ioctl(gInfo.deviceFD, VBOXVIDEO_GET_DEVICE_NAME, data, B_PATH_NAME_LENGTH);
}

status_t vboxvideo_clone_accelerant(void *data) {
	TRACE("%s\n", __FUNCTION__);
	return B_ERROR;
}

void vboxvideo_uninit_accelerant(void) { TRACE("%s\n", __FUNCTION__); }
status_t vboxvideo_get_accelerant_device_info(accelerant_device_info *adi) {
	TRACE("%s\n", __FUNCTION__);
	adi->version = B_ACCELERANT_VERSION;
	strcpy(adi->name, "VirtualBox Graphics Adapter");
	strcpy(adi->chipset, "VirtualBox Graphics Adapter");
	strcpy(adi->serial_no, "9001");
	return B_OK;
}
sem_id vboxvideo_accelerant_retrace_semaphore(void) { TRACE("%s\n", __FUNCTION__); }

// modes & constraints
uint32 vboxvideo_accelerant_mode_count(void) {
	TRACE("%s\n", __FUNCTION__);
	return 1;
}

status_t vboxvideo_get_mode_list(display_mode *dm) {
	// TODO
	TRACE("%s\n", __FUNCTION__);
	return vboxvideo_get_display_mode(dm);
}

status_t vboxvideo_propose_display_mode(display_mode *target, const display_mode *low, const display_mode *high) {
	TRACE("%s\n", __FUNCTION__);
	return B_UNSUPPORTED;
}

status_t vboxvideo_set_display_mode(display_mode *modeToSet) {
	TRACE("%s\n", __FUNCTION__);
	TRACE("trying to set mode %dx%d\n", modeToSet->timing.h_display, modeToSet->timing.v_display);
	return ioctl(gInfo.deviceFD, VBOXVIDEO_SET_DISPLAY_MODE, modeToSet, sizeof(display_mode));
}
status_t vboxvideo_get_display_mode(display_mode *currentMode) {
	TRACE("%s\n", __FUNCTION__);
	*currentMode = gInfo.sharedInfo->currentMode;
	TRACE("current mode is %dx%d\n", currentMode->timing.h_display, currentMode->timing.v_display);
	return B_OK;
}

status_t vboxvideo_get_edid_info(void *info, size_t size, uint32 *_version) {
	TRACE("%s\n", __FUNCTION__);
	
	// copied from the X11 implementation:
	static const uint8 edid_data[128] = {
		0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, /* header */
		0x58, 0x58, /* manufacturer (VBX) */
		0x00, 0x00, /* product code */
		0x00, 0x00,0x00, 0x00, /* serial number goes here */
		0x01, /* week of manufacture */
		0x00, /* year of manufacture */
		0x01, 0x03, /* EDID version */
		0x80, /* capabilities - digital */
		0x00, /* horiz. res in cm, zero for projectors */
		0x00, /* vert. res in cm */
		0x78, /* display gamma (120 == 2.2).  Should we ask the host for this? */
		0xEE, /* features (standby, suspend, off, RGB, standard colour space,
				* preferred timing mode) */
		0xEE, 0x91, 0xA3, 0x54, 0x4C, 0x99, 0x26, 0x0F, 0x50, 0x54,
			/* chromaticity for standard colour space - should we ask the host? */
		0x00, 0x00, 0x00, /* no default timings */
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, /* no standard timings */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* descriptor block 1 goes here */
		0x00, 0x00, 0x00, 0xFD, 0x00, /* descriptor block 2, monitor ranges */
		0x00, 0xC8, 0x00, 0xC8, 0x64, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, /* 0-200Hz vertical, 0-200KHz horizontal, 1000MHz pixel clock */
		0x00, 0x00, 0x00, 0xFC, 0x00, /* descriptor block 3, monitor name */
		'V', 'B', 'O', 'X', ' ', 'm', 'o', 'n', 'i', 't', 'o', 'r', '\n',
		0x00, 0x00, 0x00, 0x10, 0x00, /* descriptor block 4: dummy data */
		0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20,
		0x00, /* number of extensions */
		0x00 /* checksum goes here */
	};
	
	if (size < 128)
		return B_BUFFER_OVERFLOW;
	
	*_version = 1; /*EDID_VERSION_1*/
	memcpy(info, edid_data, 128);
	return B_OK;
}

status_t vboxvideo_get_frame_buffer_config(frame_buffer_config *config) { TRACE("%s\n", __FUNCTION__); }
status_t vboxvideo_get_pixel_clock_limits(display_mode *dm, uint32 *low, uint32 *high) { TRACE("%s\n", __FUNCTION__); }
status_t vboxvideo_move_display(uint16 hDisplayStart, uint16 vDisplayStart) { TRACE("%s\n", __FUNCTION__); }
status_t vboxvideo_get_timing_constraints(display_timing_constraints *dtc) { TRACE("%s\n", __FUNCTION__); }
void vboxvideo_set_indexed_colors(uint count, uint8 first, uint8 *colorData, uint32 flags) { TRACE("%s\n", __FUNCTION__); }

// DPMS
uint32 vboxvideo_dpms_capabilities(void) { TRACE("%s\n", __FUNCTION__); }
uint32 vboxvideo_dpms_mode(void) { TRACE("%s\n", __FUNCTION__); }
status_t vboxvideo_set_dpms_mode(uint32 dpmsFlags) { TRACE("%s\n", __FUNCTION__); }

// cursor
status_t vboxvideo_set_cursor_shape(uint16 width, uint16 height, uint16 hotX, uint16 hotY, uint8 *andMask, uint8 *xorMask) {
	// VBoxHGSMIUpdatePointerShape
}
void vboxvideo_move_cursor(uint16 x, uint16 y) { TRACE("%s\n", __FUNCTION__); }
void vboxvideo_show_cursor(bool is_visible) { TRACE("%s\n", __FUNCTION__); }

// accelerant engine
uint32 vboxvideo_accelerant_engine_count(void) {
	TRACE("%s\n", __FUNCTION__);
	return 1;
}

status_t vboxvideo_acquire_engine(uint32 capabilities, uint32 maxWait, sync_token *st, engine_token **et) {
	TRACE("%s\n", __FUNCTION__);
	*et = &sEngineToken;
	return B_OK;
}

status_t vboxvideo_release_engine(engine_token *et, sync_token *st) {
	TRACE("%s\n", __FUNCTION__);
	if (st != NULL)
		st->engine_id = et->engine_id;

	return B_OK;
}

void vboxvideo_wait_engine_idle(void) { TRACE("%s\n", __FUNCTION__); }

status_t vboxvideo_get_sync_token(engine_token *et, sync_token *st) { TRACE("%s\n", __FUNCTION__); return B_OK; }

status_t vboxvideo_sync_to_token(sync_token *st) { TRACE("%s\n", __FUNCTION__); return B_OK; }

// 2D acceleration
void vboxvideo_screen_to_screen_blit(engine_token *et, blit_params *list, uint32 count) {
	TRACE("%s\n", __FUNCTION__);
}
void vboxvideo_fill_rectangle(engine_token *et, uint32 color, fill_rect_params *list, uint32 count) {
	TRACE("%s\n", __FUNCTION__);
}
void vboxvideo_invert_rectangle(engine_token *et, fill_rect_params *list, uint32 count) {
	TRACE("%s\n", __FUNCTION__);
}
void vboxvideo_fill_span(engine_token *et, uint32 color, uint16 *list, uint32 count) {
	TRACE("%s\n", __FUNCTION__);
}
