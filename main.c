#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/keysymdef.h>


#define CONTROL_L XKeysymToKeycode(display, XK_Control_L)
#define SHIFT_L   XKeysymToKeycode(display, XK_Shift_L)


static bool isRun = true;

int run();
void check_combination(Display *display, Window *focus, XEvent *event, KeyCode *combination, size_t size);
void process_selected_text(Display *display, Window *focus, XEvent *event);
unsigned char *getSelectedText(Display *display, Window *focus, unsigned long *size);
unsigned char *getPropertyText(Display *display, Window window, Atom p, unsigned long *sizestr);

void charToString(char c, char* str) {
    str[0] = c;
    str[1] = '\0';
}

int main() {
    
    int state = run();
    
    return state;
}

// Запуск приложения
int run()
{
    Display *display = NULL;
    Window focus = None;
    XEvent event;
    int revert_to;
    
    // Подключение к серверу X
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Невозможно подключиться к серверу X\n");
        XCloseDisplay(display);
        return 1;
    }
    
    // Определение фокусного окна
    if(!XGetInputFocus(display, &focus, &revert_to) || focus == None) {
        fprintf(stderr, "Ошибка при получении фокусного окна\n");
        XCloseDisplay(display);
        return 1;
    }

    // Комбинация клавиш
    KeyCode combination[] = 
    {
        CONTROL_L,
        SHIFT_L
    };
    size_t sizeCombination = sizeof(combination)/sizeof(KeyCode);
    
    // Маска событий, которые будут отлавливаться
    XSelectInput(display, focus, KeyPressMask);
    
    // Запуск цикла обработки событий
    while (isRun) {
        XNextEvent(display, &event);

        if (event.type == KeyPress)
        check_combination(display, &focus, &event, combination, sizeCombination);
        
    }
    
    // Закрытие соединения с сервером X
    XCloseDisplay(display);
    return 0;
}

// Проверяет комбинацию клавиш
void check_combination(Display *display, Window *focus, XEvent *event, KeyCode *combination, size_t size)
{  
    for (size_t i = 0; i < size; i++)
    {
        KeyCode key = combination[i];

        if (event->xkey.keycode != key)
        return;

        XNextEvent(display, event);      
    }

    // Обработка комбинации
    process_selected_text(display, focus, event);
    printf("Нажата комбинация клавиш Ctrl+Shift\n");
}

void process_selected_text(Display *display, Window *focus, XEvent *event)
{
    unsigned long size;
    unsigned char *text = getSelectedText(display, focus, &size);
    printf("%s\n", text);

    for (unsigned long i = 0; i < size; i++)
    {
        char c = text[i];
        char str[2];
        charToString(c, str);
        KeySym keysym = XStringToKeysym(str);
        printf("Keysym: %lx char: %c\n", keysym, c);
    }
    
    
}

unsigned char *getSelectedText(Display *display, Window *focus, unsigned long *size)
{
    Atom selection_primary = XInternAtom(display, "PRIMARY", true);
    Atom utf8 = XInternAtom(display, "UTF8_STRING", true);

    if (selection_primary == None)
    printf("Атом %s пуст или его не существует\n", XGetAtomName(display, selection_primary));
    if (utf8 == None)
    printf("Атом %s пуст или его не существует\n", XGetAtomName(display, utf8));

    /* The selection owner will store the data in a property on this
     * window: */
    Window target_window = XCreateSimpleWindow(display, DefaultRootWindow(display), -10, -10, 1, 1, 0, 0, 0);
    
    /* That's the property used by the owner. Note that it's completely
     * arbitrary. */
    Atom target_property = XInternAtom(display, "TEMP", False);

    /* Request conversion to UTF-8. Not all owners will be able to
     * fulfill that request. */
    XConvertSelection(display, selection_primary, utf8, target_property, target_window,
                      CurrentTime);

    XEvent e;
    while (isRun)
    {
        XNextEvent(display, &e);
        switch (e.type)
        {
            case SelectionNotify:
                if (e.xselection.property == None)
                {
                    printf("Conversion could not be performed.\n");
                }
                else
                {
                    return getPropertyText(display, target_window, target_property, size);
                }
                break;
        }
    }
    return NULL;
}

unsigned char *getPropertyText(Display *display, Window window, Atom p, unsigned long *sizestr)
{
    Atom actual_type_return, incr;
    int actual_format_return;
    unsigned long size, bytes_after_return, nitems_return;
    unsigned char *prop_return = NULL;

    /* Dummy call to get type and size. */
    XGetWindowProperty(display, window, p, 0, 0, False, AnyPropertyType,
                       &actual_type_return, &actual_format_return, &bytes_after_return, &size, &prop_return);
    XFree(prop_return);
    

    incr = XInternAtom(display, "INCR", False);
    if (actual_type_return == incr)
    {
        printf("Data too large and INCR mechanism not implemented\n");
        return NULL;
    }

    /* Read the data in one go. */
    // printf("Property size: %lu\n", size);

    XGetWindowProperty(display, window, p, 0, size, False, AnyPropertyType,
                       &actual_type_return, &actual_format_return, &nitems_return, &bytes_after_return, &prop_return);

    // printf("%s\n", prop_return);
    printf("actual_type_return: %s\n", XGetAtomName(display, actual_type_return));
    printf("actual_format_return: %d\n", actual_format_return);
    printf("nitems_return: %lu\n", nitems_return);
    printf("bytes_after_return: %lu\n", bytes_after_return);
    printf("size: %lu\n", size);

    *sizestr = size;

    // fflush(stdout);

    /* Signal the selection owner that we have successfully read the
     * data. */
    XDeleteProperty(display, window, p);

    return prop_return;
}

void
show_utf8_prop(Display *dpy, Window w, Atom p)
{
   
    Atom da, incr, type;
    int di;
    unsigned long size, dul;
    unsigned char *prop_ret = NULL;

    /* Dummy call to get type and size. */
    XGetWindowProperty(dpy, w, p, 0, 0, False, AnyPropertyType,
                       &type, &di, &dul, &size, &prop_ret);
    XFree(prop_ret);

    incr = XInternAtom(dpy, "INCR", False);
    if (type == incr)
    {
        printf("Data too large and INCR mechanism not implemented\n");
        return;
    }

    /* Read the data in one go. */
    printf("Property size: %lu\n", size);

    XGetWindowProperty(dpy, w, p, 0, size, False, AnyPropertyType,
                       &da, &di, &dul, &dul, &prop_ret);
    printf("%s", prop_ret);
    fflush(stdout);
    XFree(prop_ret);

    /* Signal the selection owner that we have successfully read the
     * data. */
    XDeleteProperty(dpy, w, p);
}