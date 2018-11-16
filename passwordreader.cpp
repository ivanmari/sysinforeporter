#include "passwordreader.h"
#include <iostream>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#ifdef Q_OS_LINUX
#include <termios.h>
#include <unistd.h>
#endif

QString
PasswordReader::askPassword()
{
    std::cout << "Enter password:";
    enableEcho(false);
    std::string password;
    std::cin >> password;
    enableEcho(true);
    std::cout << std::endl;

    return QString(password.c_str());
}


void
PasswordReader::enableEcho(bool enable)
{
#ifdef Q_OS_WIN

    HANDLE stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD  mode;

    GetConsoleMode(stdinHandle, &mode);

    if (!enable)
    {
        mode &= ~ENABLE_ECHO_INPUT;
    }
    else
    {
        mode |= ENABLE_ECHO_INPUT;
    }

    SetConsoleMode(stdinHandle, mode);

#endif
#ifdef Q_OS_LINUX
    struct termios tty;

    tcgetattr(STDIN_FILENO, &tty);

    if (!enable)
    {
        tty.c_lflag &= ~ECHO;
    }
    else
    {
        tty.c_lflag |= ECHO;
    }

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}
