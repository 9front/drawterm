typedef struct Wlwin Wlwin;
typedef struct Clipboard Clipboard;

/* The contents of the clipboard
 * are not stored in the compositor.
 * Instead we signal that we have content
 * and the compositor gives us a pipe
 * to the program that wants it when
 * the content is pasted. */
struct Clipboard {
	QLock lk;
	char *content;

	/* Wayland requires that in order
	 * to put data in to the clipboard
	 * you must be the focused application.
	 * So we must provide the serial we get
	 * on keyboard.enter. */
	u32int serial;
};

struct Mouse {
	Point xy;
	int buttons;
	ulong msec;
};

enum{
	Aunpress,
	Apress,
	Aenter1,
	Aenter2,
};

struct Wlwin {
	int dx;
	int dy;
	int monx;
	int mony;
	Mouse mouse;
	Clipboard clip;
	Rectangle r;
	int dirty;
	int alt; /* Kalt state */

	/* Wayland State */
	int runing;
	int poolsize;
	int pointerserial;
	void *shm_data;
	struct wl_compositor *compositor;
	struct wl_display *display;
	struct wl_surface *surface;
	struct wl_surface *surfaceover;
	struct wl_surface *cursorsurface;
	struct xdg_wm_base *xdg_wm_base;
	struct xdg_toplevel *xdg_toplevel;
	struct wl_shm_pool *pool;
	struct wl_buffer *screenbuffer;
	struct wl_buffer *cursorbuffer;
	struct wl_shm *shm;
	struct wl_seat *seat;
	struct wl_data_device_manager *data_device_manager;
	struct wl_data_device *data_device;

	/* these resources can be withdrawn by a wl_seat
	   event, so must be protected by a lock */
	RWLock caplk;
	struct wl_pointer *pointer;
	struct wl_keyboard *keyboard;

	/* Keyboard state */
	struct xkb_state *xkb_state;
	struct xkb_context *xkb_context;

	/* Decoration state */
	struct libdecor *decor;
	struct libdecor_frame *decor_frame;

	struct zwp_primary_selection_device_manager_v1 *primsel;
	struct zwp_primary_selection_device_v1 *primsel_device;

	struct zwp_pointer_constraints_v1 *constraints;
};

void wlallocbuffer(Wlwin*);
void wlsetcb(Wlwin*);
void wlsettitle(Wlwin*, char*);
char* wlgetsnarf(Wlwin*);
void wlsetsnarf(Wlwin*, char*);
void wlsetmouse(Wlwin*, Point);
void wldrawcursor(Wlwin*, Cursorinfo*);
void wlresize(Wlwin*, int, int);
void wlflush(Wlwin*);
void wlclose(Wlwin*);

