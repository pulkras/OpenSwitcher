// If you read it you shold know that X11 is absolutely old shit please try support to develop new Window Meneger
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <fcntl.h>
#include <errno.h>
#include <time.h>


#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <X11/XKBlib.h>

#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-x11.h>


#include <unicode/ustring.h>
#include <unicode/ucnv.h>
#include <unicode/ustdio.h>
#include <unicode/utf8.h>
#include <unicode/uchar.h>


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
KeySym KeyCodeToKeySym(Display * display, KeyCode keycode, unsigned int event_mask);




int main() {
    uint8_t* text = readTextFromStdin();

    KeySym *arr = transform_stdin_to_KeySyms(text);
    int32_t length = strlen((char *)text);

    if (length <= 0)
    {
        return -1;
    }

    for (int i = 0; i < length; i++)
    {
        send_KeySym(arr[i]);
    }
	free(arr);

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

        char* codestr = (char*)malloc(sizeof(char)*15);
        snprintf(codestr, 15, "U+%04X", codepoint);
        // printf("codepoint: %s ", codestr);

        // printf("index: %d codepoint: %s name: %s \n", index-1, codestr, codestr);

        // This is expected to be KeySym = xkb_keysym_t

        arr[index-1] = (KeySym)keysym;	
        U8_NEXT(text, index, length, codepoint);

		free(codestr);
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

    KeyCode keycode = XKeysymToKeycode(display, keysym);

    // eventX должен быть равен клавиатуре, чтобы найти клавиатуру воспользуйтесь sudo evtest
    const char* inputDevice = "/dev/input/event0";
    int fd = open(inputDevice, O_WRONLY);
    if (fd == -1) {
        printf("Ошибка при открытии устройства ввода\n");
        return -1;
    }
    // ---------------------------------------------------
    char* keysymName = XKeysymToString(keysym);

    // printf("Keysym: %s\n", keysymName);


    unsigned int event_mask = ShiftMask | LockMask;

    struct input_event ev;
    UChar32 codepoint = xkb_keysym_to_utf32(keysym);

    // если это буква и если эта буква заглавная
    if (u_isalpha(codepoint) && xkb_keysym_to_upper(keysym) == keysym)
    {
        // заглавная буква
        send_key(fd, &ev, 1, XKeysymToKeycode(display, XK_Shift_L));
        send_key(fd, &ev, 1, keycode);
        send_key(fd, &ev, 0, XKeysymToKeycode(display, XK_Shift_L));
        send_key(fd, &ev, 0, keycode);
    // если это знак и если он равняется себе при нажатии shift
    } else if(!u_isalpha(codepoint) && KeyCodeToKeySym(display, keycode, event_mask) == keysym)
    {
        // заглавный знак
        send_key(fd, &ev, 1, XKeysymToKeycode(display, XK_Shift_L));
        send_key(fd, &ev, 1, keycode);
        send_key(fd, &ev, 0, XKeysymToKeycode(display, XK_Shift_L));
        send_key(fd, &ev, 0, keycode);
    }else
    {
        // строчная буква/знак
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

KeySym KeyCodeToKeySym(Display * display, KeyCode keycode, unsigned int event_mask)
{
    KeySym keysym = NoSymbol;

    //Get the map
    XkbDescPtr keyboard_map = XkbGetMap(display, XkbAllClientInfoMask, XkbUseCoreKbd);
    if (keyboard_map) {
        //What is diff between XkbKeyGroupInfo and XkbKeyNumGroups?
        unsigned char info = XkbKeyGroupInfo(keyboard_map, keycode);
        unsigned int num_groups = XkbKeyNumGroups(keyboard_map, keycode);

        //Get the group
        unsigned int group = 0x00;
        switch (XkbOutOfRangeGroupAction(info)) {
            case XkbRedirectIntoRange:
                /* If the RedirectIntoRange flag is set, the four least significant
                 * bits of the groups wrap control specify the index of a group to
                 * which all illegal groups correspond. If the specified group is
                 * also out of range, all illegal groups map to Group1.
                 */
                group = XkbOutOfRangeGroupInfo(info);
                if (group >= num_groups) {
                    group = 0;
                }
            break;

            case XkbClampIntoRange:
                /* If the ClampIntoRange flag is set, out-of-range groups correspond
                 * to the nearest legal group. Effective groups larger than the
                 * highest supported group are mapped to the highest supported group;
                 * effective groups less than Group1 are mapped to Group1 . For
                 * example, a key with two groups of symbols uses Group2 type and
                 * symbols if the global effective group is either Group3 or Group4.
                 */
                group = num_groups - 1;
            break;

            case XkbWrapIntoRange:
                /* If neither flag is set, group is wrapped into range using integer
                 * modulus. For example, a key with two groups of symbols for which
                 * groups wrap uses Group1 symbols if the global effective group is
                 * Group3 or Group2 symbols if the global effective group is Group4.
                 */
            default:
                if (num_groups != 0) {
                    group %= num_groups;
                }
            break;
        }

        XkbKeyTypePtr key_type = XkbKeyKeyType(keyboard_map, keycode, group);
        unsigned int active_mods = event_mask & key_type->mods.mask;

        int i, level = 0;
        for (i = 0; i < key_type->map_count; i++) {
            if (key_type->map[i].active && key_type->map[i].mods.mask == active_mods) {
                level = key_type->map[i].level;
            }
        }

        keysym = XkbKeySymEntry(keyboard_map, keycode, level, group);
        XkbFreeClientMap(keyboard_map, XkbAllClientInfoMask, true);
    }

    return keysym;
}

