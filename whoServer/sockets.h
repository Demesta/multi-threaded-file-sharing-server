
#ifndef SYSPRO2__SOCKETS_H_
#define SYSPRO2__SOCKETS_H_

#include <cstddef>
#include <string>

int socket_read_int(int socket, int* value);
int socket_read_str(int socket, char *buffer, size_t buffer_size);
int socket_write_int(int socket, int value);
int socket_write_size_t(int socket, size_t value);
int socket_write_str(int socket, char *buffer, size_t buffer_size);
int socket_write_string(int socket, std::string& str);

#endif //SYSPRO2__SOCKETS_H_
