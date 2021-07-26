#ifndef DHIPS_DEFINES_H
#define DHIPS_DEFINES_H

// send-email python program
#define SCAN_INTERVAL 2
#define SEND_EMAIL_PATH "src/dhips-send-email.py"
#define SEND_EMAIL_COMPILED 0

// Alarm IDs
#define ALARM_I_FILE_MOD            1
#define ALARM_I_FILE_DEL            2
#define ALARM_I_BIN_MOD             3
#define ALARM_I_BIN_DEL             4
#define ALARM_I_FILE_CREATE         5
#define ALARM_I_BIN_CREATE          6

#define ALARM_II_USER_LOGGED_IN     7
#define ALARM_II_USER_LOGGED_OUT    8

#define ALARM_III_PROMISC_ON        9
#define ALARM_III_PROMISC_OFF       10
#define ALARM_III_SNIFFER_FOUND     11

#define ALARM_VII_SUSFILE_FOUND     12
#define ALARM_VII_SUSPROC_FOUND     13

// Special definitions for modules
#define SYSFILE_TYPE                1
#define BINARY_TYPE                 2

#define SNIFFER_TYPE                1
#define WHITELISTED_TYPE            2

#endif // DHIPS_DEFINES_H