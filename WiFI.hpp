#ifndef __WIFI_H__
#define __WIFI_H__

#include <string.h>

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

// str should point to the end of the string
static inline bool IsFail(const char *const str)
{
    return *(str - 5) == 'F' && *(str - 4) == 'A' && *(str - 3) == 'I' && *(str - 2) == 'L' && *(str - 1) == '\r' && *str == '\n';
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

    while (!(IsOk(buffer + i - 1) || IsError(buffer + i - 1) || IsFail(buffer + i - 1)))
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

    if (IsFail(buffer + i - 1))
    {
        pointers.Status = buffer + i - 6;

        buffer[i - 7] = 0;
        buffer[i - 8] = 0;
    }

    buffer[i - 1] = 0;
    buffer[i - 2] = 0;

    buffer[i] = 0;

    return pointers;
}

static inline IpPointers ParseIps(char *data)
{
    IpPointers ips;

    char *p = data + 16; //length of +CIPSTA_CUR:ip:"

    ips.Ip = p;

    while (*p != '"')
    {
        p++;
    }

    *p = 0;

    p += 24; //length of \r\n+CIPSTA_CUR:gateway:"

    ips.Gateway = p;

    while (*p != '"')
    {
        p++;
    }

    *p = 0;

    p += 24; //length of \r\n+CIPSTA_CUR:netmask:"

    ips.Netmask = p;

    while (*p != '"')
    {
        p++;
    }

    *p = 0;

    return ips;
}

static inline char *ParseMac(char *data)
{
    char *p = data + 15;

    p[17] = 0;

    return Util::ToUpper(p);
}

static inline void Connect(const char *const gateway)
{
    Serial.write("AT+CIPSTART=\"TCP\",\"");
    Serial.write(gateway);
    Serial.write("\",80");
    Serial.write('\r');
    Serial.write('\n');
}

static inline void AddDevice(const char *const mac, const char *const ip, const char *const gateway)
{
    char buffer[200] = "GET /api/adddevice.php?name=helloworld&type=0&mac=AA:BB:CC:DD:EE:FF&ip=";

    char *p = buffer + 50; // length of GET /api/adddevice.php?name=helloworld&type=0&mac=
    strcpy(p, mac);
    buffer[67] = '&';
    p = buffer + 71; // length of GET /api/adddevice.php?name=helloworld&type=0&mac=AA:BB:CC:DD:EE:FF&ip=
    strcpy(p, ip);
    p = buffer + 71 + strlen(ip);
    strcpy(p, " HTTP/1.1\r\nHost: ");
    p += 17; // length of " HTTP/1.1\r\nHost: "
    strcpy(p, gateway);
    p += strlen(gateway);
    strcpy(p, "\r\n\r\n");
    p += 4;
    *p = 0;

    const uint16_t len = strlen(buffer);

    Serial.write("AT+CIPSEND=");
    char buf[10] = {0};
    Serial.write(Util::UintToString<uint16_t>(len, buf, 10));
    Serial.write('\r');
    Serial.write('\n');

    delay(1000);
    Serial.write(buffer);
}

static inline void WaitForClosed()
{
    char buffer[8] = {0}; //CLOSED\r\n

    while (!(buffer[0] == 'C' && buffer[1] == 'L' && buffer[2] == 'O' && buffer[3] == 'S' && buffer[4] == 'E' && buffer[5] == 'D' && buffer[6] == '\r' && buffer[7] == '\n'))
    {
        buffer[0] = buffer[1];
        buffer[1] = buffer[2];
        buffer[2] = buffer[3];
        buffer[3] = buffer[4];
        buffer[4] = buffer[5];
        buffer[5] = buffer[6];
        buffer[6] = buffer[7];
        buffer[7] = GetChar();
    }
}

static inline void WaitForReady()
{
    char buffer[7] = {0}; //ready\r\n
    while (!(buffer[0] == 'r' && buffer[1] == 'e' && buffer[2] == 'a' && buffer[3] == 'd' && buffer[4] == 'y' && buffer[5] == '\r' && buffer[6] == '\n'))
    {
        buffer[0] = buffer[1];
        buffer[1] = buffer[2];
        buffer[2] = buffer[3];
        buffer[3] = buffer[4];
        buffer[4] = buffer[5];
        buffer[5] = buffer[6];
        buffer[6] = GetChar();
    }
}

} // namespace WiFi

#endif