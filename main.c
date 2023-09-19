#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <errno.h>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <X11/XKBlib.h>

// #include <xkbcommon/xkbcommon-keysyms.h>
#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-x11.h>

// #include <xcb/xcb.h>

#include <unicode/ustring.h>
#include <unicode/ucnv.h>
#include <unicode/ustdio.h>
#include <unicode/utf8.h>

#include <libinput.h>

#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <linux/keyboard.h>


uint8_t *get_input_text();
KeySym *transform_stdin_to_KeySyms(uint8_t *text);
int send_KeySym(KeySym KeySym);
int send_key(int fd, struct input_event *ev, int value, KeyCode keycode);
int create_event(struct input_event *ev, int type, int code, int value);
int write_event(int fd, const struct input_event *ev);




int main() {
    
    // uint8_t *text = get_input_text();
    uint8_t *text = "Текст";
    KeySym *arr = transform_stdin_to_KeySyms(text);
    
    int32_t length = strlen((const char *)text);

    if (length <= 0)
    {
        return -1;
    }
    

    printf("length: %d\n", 5);

    // send_KeySym(XK_A);

    

    for (int i = 0; i < length; i++)
    {
        send_KeySym(arr[i]);
    }
    
    return 0;
}

uint8_t *get_input_text()
{
    if (stdin == NULL) {
        perror("Ошибка при открытии stdin");
    }

    // Получение размера входного потока
    fseek(stdin, 0, SEEK_END);
    size_t size = ftell(stdin);
    rewind(stdin);

    uint8_t *text = malloc(sizeof(uint8_t)*size + 1);
    if (text == NULL) {
        perror("Ошибка при выделении памяти");
    }

    // Чтение всего текста
    size_t bytesRead = fread(text, sizeof(uint8_t), size, stdin);
    if (bytesRead != size) {
        perror("Произошла ошибка при чтении stdin");
    }

    text[size+1] = '\0';

    printf("Весь текст: %s\n", text);

    return text;
}

KeySym *transform_stdin_to_KeySyms(uint8_t *text)
{
    int32_t length = strlen((const char *)text);
    int32_t index = 0;

    KeySym *arr = (KeySym *)malloc(sizeof(KeySym)*length);

    UChar32 codepoint;
    U8_NEXT(text, index, length, codepoint);

    while (codepoint > 0) {
        // printf("index: %d Символ: %c U+%04X\n", index-1, text[index-1], codepoint);

        xkb_keysym_t keysym = xkb_utf32_to_keysym((uint64_t)codepoint);


        char buffer[100];
        xkb_keysym_get_name(keysym, buffer, 100);

        char* codestr = (char*)malloc(sizeof(char)*8);
        snprintf(codestr, 15, "U+%04X", codepoint);
        // printf("codepoint: %s ", codestr);

        printf("index: %d codepoint: %s name: %s \n", index-1, codestr, codestr);

        // This is expected to be KeySym = xkb_keysym_t
        arr[index-1] = (KeySym)keysym;
        U8_NEXT(text, index, length, codepoint);
    }

    return arr;
}

int send_KeySym(KeySym keysym)
{
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Failed to open display\n");
        return -1;
    }

    // printf("KeyCode: %d \n", XKeysymToKeycode(display, XK_A));

    Window focus;
    int revert;
    XGetInputFocus(display, &focus, &revert);
    if (focus == None) {
        fprintf(stderr, "No owner for PRIMARY selection\n");
        XCloseDisplay(display);
        return -1;
    }

    KeyCode keycode = XKeysymToKeycode(display, keysym);

    // eventX должен быть равен клавиатуре, чтобы найти клавиатуру воспользуйтесь sudo evtest
    const char* inputDevice = "/dev/input/event0";
    int fd = open(inputDevice, O_WRONLY);
    if (fd == -1) {
        printf("Ошибка при открытии устройства ввода\n");
        return -1;
    }

    struct input_event ev;


    XkbStateRec state;
    XkbGetState(display, XkbUseCoreKbd, &state);
    
    // если есть CapsLacks
    if (state.locked_mods & LockMask)
    {
        if (xkb_keysym_to_upper(keysym) == keysym)
        {
            // заглавная буква
            send_key(fd, &ev, 1, keycode);
            send_key(fd, &ev, 0, keycode);
        } else
        {
            // строчная буква
            send_key(fd, &ev, 1, XKeysymToKeycode(display, XK_Shift_L));
            send_key(fd, &ev, 1, keycode);
            send_key(fd, &ev, 0, XKeysymToKeycode(display, XK_Shift_L));
            send_key(fd, &ev, 0, keycode);
        }
    } else
    {
        if (xkb_keysym_to_upper(keysym) == keysym)
        {
            // заглавная буква
            send_key(fd, &ev, 1, XKeysymToKeycode(display, XK_Shift_L));
            send_key(fd, &ev, 1, keycode);
            send_key(fd, &ev, 0, XKeysymToKeycode(display, XK_Shift_L));
            send_key(fd, &ev, 0, keycode);

        } else
        {
            // строчная буква
            send_key(fd, &ev, 1, keycode);
            send_key(fd, &ev, 0, keycode);
        }
    }


    
    
     
    close(fd);

    return 0;
}

int send_key(int fd, struct input_event *ev, int value, KeyCode keycode)
{
    // keycode от scancode отличается на 8
    // нажатие клавиши
    if (create_event(ev, EV_KEY, keycode-8, value))
    {
        perror("Ошибка при создании эвента");
        return -1;
    }
    
    if (write_event(fd, ev))
    {
        perror("Ошибка при записи эвента");
        return -1;
    }

    if (create_event(ev, EV_SYN, SYN_REPORT, 0))
    {
        perror("Ошибка при создании эвента");
        return -1;
    }
    
    if (write_event(fd, ev))
    {
        perror("Ошибка при записи эвента");
        return -1;
    }


    return -1;
}

int write_event(int fd, const struct input_event *ev)
{
	int ret;
	ret = write(fd, ev, sizeof(*ev));
	return (ret == -1 || (size_t)ret < sizeof(*ev)) ? -1 : 0;
}

int create_event(struct input_event *ev, int type, int code, int value)
{
	ev->time.tv_sec = 0;
	ev->time.tv_usec = 0;
	ev->type = type;
	ev->code = code;
	ev->value = value;
	return 0;
}

