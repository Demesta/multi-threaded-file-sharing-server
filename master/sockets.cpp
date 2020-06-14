#include "sockets.h"

#include <unistd.h>

int __socket_safe_read(int socket, void *buffer, size_t buffer_size)
{
    char *buffer_ptr = (char *) buffer;
    size_t current_index = 0;

    while (current_index < buffer_size)
    {
        int read_size = read(socket, buffer_ptr + current_index, buffer_size - current_index);
        if (read_size < 0)
        {
            return -1;
        }
        current_index += read_size;
    }

    return buffer_size;
}

int __socket_safe_write(int socket, void *buffer, size_t buffer_size)
{
    char *buffer_ptr = (char *) buffer;
    size_t current_index = 0;

    while (current_index < buffer_size)
    {
        int write_size = write(socket, buffer_ptr + current_index, buffer_size - current_index);
        if (write_size < 0)
        {
            return -1;
        }
        current_index += write_size;
    }

    return buffer_size;
}

int socket_read_int(int socket, int *value)
{
    return __socket_safe_read(socket, value, sizeof(int));
}

int socket_read_str(int socket, char *buffer, size_t buffer_size)
{
    size_t str_size;
    if (__socket_safe_read(socket, &str_size, sizeof(size_t)) < 0)
    {
        return -1;
    }

    if ((str_size + 1) > buffer_size)
    {
        return -1;
    }

    if(__socket_safe_read(socket, buffer, str_size) < 0) {
        return -1;
    }

    buffer[str_size] = '\0';

    return str_size + 1;
}

int socket_write_int(int socket, int value)
{
    return __socket_safe_write(socket, &value, sizeof(int));
}

int socket_write_size_t(int socket, size_t value)
{
    return __socket_safe_write(socket, &value, sizeof(size_t));
}

int socket_write_str(int socket, char *buffer, size_t buffer_size)
{
    if (__socket_safe_write(socket, &buffer_size, sizeof(size_t)) < 0)
    {
        return -1;
    }

    return __socket_safe_write(socket, buffer, buffer_size);
}

int socket_write_string(int socket, std::string str)
{
    char *str_ptr = const_cast<char *>(str.c_str());
    size_t str_size = str.length();
    return socket_write_str(socket, str_ptr, str_size);
}