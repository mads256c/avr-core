#ifndef __WIFI_H__
#define __WIFI_H__

#include <HardwareSerial.h>

namespace WiFi
{
struct CommandPointers
{
    char *Command = 0;
    char *Data = 0;
    char *Status = 0;
};

struct IpPointers
{
    char *Ip = 0;
    char *Gateway = 0;
    char *Netmask = 0;
};

static inline void SendCommand(const char *const str)
{
    Serial.write(str);
    Serial.write('\r');
    Serial.write('\n');
}

static inline char GetChar()
{
    int c = Serial.read();

    while (c == -1)
    {
        c = Serial.read();
    }

    return (char)c;
}

// str should point to the end of the string
static inline bool IsOk(const char *const str)
{
    return *(str - 3) == 'O' && *(str - 2) == 'K' && *(str - 1) == '\r' && *str == '\n';
}

// str should point to the end of the string
static inline bool IsError(const char *const str)
{
    return *(str - 6) == 'E' && *(str - 5) == 'R' && *(str - 4) == 'R' && *(str - 3) == 'O' && *(str - 2) == 'R' && *(str - 1) == '\r' && *str == '\n';
}

static inline CommandPointers ReceiveCommand(char *buffer)
{
    CommandPointers pointers;

    char c = GetChar();
    buffer[0] = c;
    uint16_t i = 1;

    // Get command
    while (!(i >= 3 && buffer[i - 2] == '\r' && buffer[i - 1] == '\n'))
    {
        c = GetChar();
        buffer[i] = c;
        i++;
    }

    buffer[i - 2] = 0;
    buffer[i - 1] = 0;

    pointers.Command = buffer;
    pointers.Data = buffer + i;

    while (!(IsOk(buffer + i - 1) || IsError(buffer + i - 1)))
    {
        c = GetChar();
        buffer[i] = c;
        i++;
    }

    if (IsOk(buffer + i - 1))
    {
        pointers.Status = buffer + i - 4;
        buffer[i - 5] = 0;
        buffer[i - 6] = 0;
    }

    if (IsError(buffer + i - 1))
    {
        pointers.Status = buffer + i - 7;
        buffer[i - 8] = 0;
        buffer[i - 9] = 0;
    }

    return pointers;
}

static inline IpPointers ParseIps(char * data)
{
    IpPointers ips;

    char* p = data + 16; //length of +CIPSTA_CUR:ip:"

    ips.Ip = p;

    while (*p != '"')
    {
        p++;
    }

    *p = 0;

    p += 23; //length of \r\n+CIPSTA_CUR:gateway:"

    ips.Gateway = p;

    while (*p != '"')
    {
        p++;
    }

    *p = 0;

    p += 23; //length of \r\n+CIPSTA_CUR:netmask:"

    ips.Netmask = p;

    while (*p != '"')
    {
        p++;
    }

    *p = 0;

    return ips;
}

} // namespace WiFi

#endif