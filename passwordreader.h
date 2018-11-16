#ifndef PASSWORDREADER_H
#define PASSWORDREADER_H

#include <QString.h>

class PasswordReader
{
public:
    PasswordReader();
    static QString askPassword();

private:
    static void enableEcho(bool enable);
};

#endif // PASSWORDREADER_H
