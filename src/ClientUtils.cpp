#include <conio.h>

#include "ClientUtils.h"
#include "FileWorker.h"

void ClientUtils::messegesGetter(MessegesMutex& mmOut, MessegesMutex& mmIn)
{
    while (true)
    {
        if (_kbhit() && canWrite)
        {
            unsigned char ch = _getch();

            if (ch == '\r' || ch == '\n')
            {
                mmIn.push("\r\x1b[2K");
                mmIn.push(messege.getMessege());
                mmIn.push("\n>> ");

                mmOut.push(messege.getMessege());

                messege.deleteMessege();
            }
            else if (ch == '\b')
            {
                if (!messege.isEmpty())
                {
                    messege.backspaceMessege();
                    mmIn.push("\b \b");
                }
            }
            else if (ch == 224 || ch == 0)
            {
                ch = _getch();

                if (ch == 75)
                {
                    bool isChanged = messege.arrowsFunc(ARROWLEFT);

                    if (isChanged)
                        mmIn.push("\x1b[D");
                }
                else if (ch == 77)
                {
                    bool isChanged = messege.arrowsFunc(ARROWRIGHT);

                    if (isChanged)
                        mmIn.push("\x1b[C");
                }

                else if (ch == 134)
                {
                    mmIn.push("\r\x1b[2K");
                    mmIn.push("You have ended the conversation");
                    mmIn.push("\n");

                    const char* exitMsg = "\x1AThe person you were speaking with has ended the conversation";
                    mmOut.push(exitMsg);
                    break;
                }

                else if (ch == 60)
                {
                    canWrite = false;

                    messege.deleteMessege();

                    string filePath = FileWorker::selectFile();

                    string toSent = "FILE: " + FileWorker::getFileName(filePath) + "\nFILE:\t f5 to Load";

                    mmIn.push("\r\x1b[2K");
                    mmIn.push(toSent);
                    mmIn.push("\n>> ");

                    mmOut.push(toSent);

                    canWrite = true;
                }
            }
            else
            {
                messege.addSybmol(ch);

                string s(1, ch);
                std::string text = messege.getMessege();
                size_t pos = messege.getPositionOfCursor();

                if (pos < text.size())
                {
                    mmIn.push(text.substr(pos - 1));
                    mmIn.push("\x1b[" + std::to_string(text.size() - pos) + "D");
                }
                else
                    mmIn.push(s);
            }
        }
    }
}