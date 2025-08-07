#define MAX_INPUT_LEN 128
#define KEYBOARD_DATA_PORT 0x60
#define KBD_CTRL_PORT 0x64
#define KBD_RESET_CMD 0xFE

#include "include/stdint.h"
#include "include/ports.h"

const char *correct_username = "patrickdodwell";
const char *correct_password = "supersecretpassword12";

// Simple scancode to ASCII (partial US QWERTY)
static const char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b', '\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ',
};

void login();

static inline void io_wait() {
    outb(0x80, 0);
}

int strcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) {
        a++; b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

int strlen(const char* s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

int strncmp(const char* s1, const char* s2, int n) {
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    return n == 0 ? 0 : (unsigned char)*s1 - (unsigned char)*s2;
}

void putchar(char c);       // extern from console.c
void printf(const char* s); // extern from console.c
void clear_screen(void);    // extern from console.c

// Poll keyboard for a key press, blocking
char getchar(void) {
    while (1) {
        uint8_t status = inb(0x64);
        if (status & 1) {  // output buffer full?
            uint8_t scancode = inb(KEYBOARD_DATA_PORT);
            int released = scancode & 0x80;
            if (!released) {
                uint8_t key = scancode & 0x7F;
                if (key < 128) {
                    char c = scancode_to_ascii[key];
                    if (c) return c;
                }
            }
        }
    }
}

// Reads a line of input with optional masking (e.g. for password)
void get_input(const char* prompt, char* buffer, int max_len, int mask_input) {
    int len = 0;
    printf(prompt);
    while (1) {
        char c = getchar();
        if (c == '\r' || c == '\n') {
            putchar('\n');
            buffer[len] = 0;
            break;
        } else if (c == '\b' || c == 0x7F) {
            if (len > 0) {
                len--;
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }
        } else if (len < max_len - 1) {
            buffer[len++] = c;
            if (mask_input) {
                putchar('*');
            } else {
                putchar(c);
            }
        }
    }
}

void shell_run_command(char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        printf("Available commands:\n");
        printf("help         - Show this message\n");
        printf("clear        - Clear the screen\n");
        printf("reboot       - Reboot the system\n");
        printf("time         - Show uptime (dummy)\n");
        printf("echo         - Echo arguments\n");
        printf("version      - Show OS version\n");
        printf("cls          - Clear the screen (alias for clear)\n");
        printf("spoodify     - Launch the Spoodify music player\n");
    } else if (strcmp(cmd, "clear") == 0 || strcmp(cmd, "cls") == 0) {
        clear_screen();
    } else if (strcmp(cmd, "reboot") == 0) {
        printf("Rebooting...\n");
        // TODO: Implement reboot properly
        while (1) {
            outb(KBD_CTRL_PORT, KBD_RESET_CMD); // Send reset command
            io_wait();
        }
    } else if (strcmp(cmd, "time") == 0) {
        // Dummy uptime message
        printf("Uptime: 12345 seconds\n");
    } else if (strncmp(cmd, "echo ", 5) == 0) {
        printf(cmd + 5);
        printf("\n");
    } else if (strcmp(cmd, "version") == 0) {
        printf("Doors 11 OS v0.1-alpha\n");
    } else if (strlen(cmd) == 0) {
        // empty input, do nothing
    } else if (strcmp(cmd, "logout") == 0) {
        login();
    } else if (strcmp(cmd, "spoodify") == 0) {
        printf("Could not launch Spoodify due to 2 reasons:\n\t1. Audio driver not implemented yet\n\t2. Spoodify not implemented yet\n");
    } else {
        printf("Unknown command: ");
        printf(cmd);
        printf("\n");
    }
}

void shell() {
    char input_buffer[MAX_INPUT_LEN];
    int input_len = 0;

    printf("\nC:\\User\\Doors11>");

    while (1) {
        char c = getchar();

        if (c == '\r' || c == '\n') {
            putchar('\n');
            input_buffer[input_len] = 0;
            shell_run_command(input_buffer);
            input_len = 0;
            printf("C:\\User\\Doors11>");
        } else if (c == '\b' || c == 0x7F) {
            if (input_len > 0) {
                input_len--;
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }
        } else if (input_len < MAX_INPUT_LEN - 1) {
            input_buffer[input_len++] = c;
            putchar(c);
        }
    }
}

void login() {
    char username[MAX_INPUT_LEN];
    char password[MAX_INPUT_LEN];

    while (1) {
        get_input("Username>", username, MAX_INPUT_LEN, 0);
        get_input("Password>", password, MAX_INPUT_LEN, 1);

        if (strcmp(username, correct_username) == 0 && strcmp(password, correct_password) == 0) {
            printf("\nLogin successful!\n");
            shell();
            break;  // just in case shell returns
        } else {
            printf("\nInvalid username or password. Please try again.\n");
        }
    }
}
