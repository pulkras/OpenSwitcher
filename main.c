#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

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


uint8_t *readTextFromStdin();
KeySym *transform_stdin_to_KeySyms(uint8_t *text);
int send_KeySym(KeySym KeySym);
int send_key(int fd, struct input_event *ev, int value, KeyCode keycode);
int create_event(struct input_event *ev, int type, int code, int value);
int write_event(int fd, const struct input_event *ev);
uint8_t* appendCharToString(uint8_t* str, uint8_t c);

int main() {
    uint8_t* text = readTextFromStdin();
    if (text != NULL) {
        printf("Считанный текст: %s\n", text);
    }
    // uint8_t *text = "Текст!!!==";
    KeySym *arr = transform_stdin_to_KeySyms(text);
    
    int32_t length = strlen((char *)text);

    if (length <= 0)
    {
        return -1;
    }

    // send_KeySym(XK_equal);

    for (int i = 0; i < length; i++)
    {
        send_KeySym(arr[i]);
    }
    
    return 0;
}

uint8_t *readTextFromStdin()
{
    uint8_t ch;
    uint8_t *text = malloc(1*sizeof(uint8_t));

    if (text == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }

    text[0] = '\0';

    for (size_t i = 1; read(STDIN_FILENO, &ch, 1) > 0; i++)
        text = appendCharToString(text, ch);

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

    // если это буква и если эта буква заглавная
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
     
    close(fd);
    XCloseDisplay(display);

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

uint8_t* appendCharToString(uint8_t* str, uint8_t c)
{
    // Получение текущей длины строки
    int length = strlen((char *)str);
    printf("length gg: %d\n", length);

    // Выделение памяти для новой строки
    uint8_t* newStr = realloc(str, (length + 2) * sizeof(uint8_t));
    if (newStr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return str;
    }

    // Добавление символа в новую строку
    newStr[length] = c;
    newStr[length + 1] = '\0';

    return newStr;
}

