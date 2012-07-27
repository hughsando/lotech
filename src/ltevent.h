/* Copyright (C) 2011 Ian MacLarty */
LT_INIT_DECL(ltevent)
struct LTSceneNode;

extern LTSceneNode *lt_exclusive_receiver;

#define LT_EVENT_TOUCH_DOWN     0x0301
#define LT_EVENT_TOUCH_UP       0x0302
#define LT_EVENT_TOUCH_MOVE     0x0304
#define LT_EVENT_MOUSE_DOWN     0x0501
#define LT_EVENT_MOUSE_UP       0x0502
#define LT_EVENT_MOUSE_MOVE     0x0504
#define LT_EVENT_KEY_DOWN       0x0801
#define LT_EVENT_KEY_UP         0x0802

#define LT_EVENT_POINTER        0x0100
#define LT_EVENT_POINTER_DOWN   0x0101
#define LT_EVENT_POINTER_UP     0x0102
#define LT_EVENT_POINTER_MOVE   0x0104
#define LT_EVENT_TOUCH          0x0300
#define LT_EVENT_MOUSE          0x0500
#define LT_EVENT_KEY            0x0800

struct LTEvent : LTObject {
    int event;
    LTfloat orig_x;
    LTfloat orig_y;
    LTfloat x;
    LTfloat y;
    int touch_id;
    int button;
    LTKey key;

    LTEvent() {
        event = 0;
        orig_x = 0;
        orig_y = 0;
        x = 0;
        y = 0;
        touch_id = 0;
        button = 0;
        key = LT_KEY_UNKNOWN;
    }

    LTEvent(LTEvent *event) {
        LTEvent::event = event->event;
        LTEvent::orig_x = event->orig_x;
        LTEvent::orig_y = event->orig_y;
        LTEvent::x = event->x;
        LTEvent::y = event->y;
        LTEvent::touch_id = event->touch_id;
        LTEvent::button = event->button;
        LTEvent::key = event->key;
    }
};

struct LTEventHandlerBB {
    LTfloat left;
    LTfloat bottom;
    LTfloat right;
    LTfloat top;
};

struct LTEventHandler {
    LTEventHandlerBB *bb;
    int filter;

    LTEventHandler(int filter, LTfloat left, LTfloat bottom, LTfloat right, LTfloat top);
    LTEventHandler(int filter);
    virtual ~LTEventHandler();

    bool hit(LTEvent *e);

    // Returning true stops the event propogating further.
    virtual bool consume(LTSceneNode *node, LTEvent *event) = 0;
};

void ltPropagateEvent(LTSceneNode *node, LTEvent *event);

void *lt_alloc_LTEvent(lua_State *L);
LTEvent *lt_expect_LTEvent(lua_State *L, int arg);
