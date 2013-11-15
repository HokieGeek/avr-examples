typedef struct {
    int pinId;
    long lastDebounceTime;
    int state;
    // void* handler;
    //void (*handler)(int);
} DEBOUNCE_ITEM;

DEBOUNCE_ITEM* debouncedButtons;
