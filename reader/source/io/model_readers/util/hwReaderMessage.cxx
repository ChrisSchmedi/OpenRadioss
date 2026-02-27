//Copyright>    OpenRadioss
//Copyright>    Copyright (C) 1986-2026 Altair Engineering Inc.
//Copyright>
//Copyright>    This program is free software: you can redistribute it and/or modify
//Copyright>    it under the terms of the GNU Affero General Public License as published by
//Copyright>    the Free Software Foundation, either version 3 of the License, or
//Copyright>    (at your option) any later version.
//Copyright>
//Copyright>    This program is distributed in the hope that it will be useful,
//Copyright>    but WITHOUT ANY WARRANTY; without even the implied warranty of
//Copyright>    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//Copyright>    GNU Affero General Public License for more details.
//Copyright>
//Copyright>    You should have received a copy of the GNU Affero General Public License
//Copyright>    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//Copyright>
//Copyright>
//Copyright>    Commercial Alternative: Altair Radioss Software
//Copyright>
//Copyright>    As an alternative to this open-source version, Altair also offers Altair Radioss
//Copyright>    software under a commercial license.  Contact Altair to discuss further if the
//Copyright>    commercial version may interest you: https://www.altair.com/radioss/.
#include <cstring>
#include <sstream>
#include <fstream>

#include "hwReaderMessage.h"
#include <MODEL_IO/hw_cfg_reader_message_pool.h>

bool hwReaderMessageList::ReadMessageFile(std::string filename)
{
    hwReaderMessagePool *messages = new hwReaderMessagePool();

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::string message("Cannot open message file ");
        message += filename;
        this->Add(109002, 2, message.c_str(),
            "", 0, "** ERROR IN ENVIRONMENT");
        delete messages;
        return false;
    }

    std::string line;
    std::getline(file, line, '/'); // extract everything before and including the first '/'
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string buffer;
        std::getline(ss, buffer, '/'); // extract "MESSAGE/"
        int id;
        ss >> id;
        ss.get(); // extract '/'
        std::getline(ss, buffer); // extract rest of line
        if (buffer.find("TITLE") != buffer.npos)
        {
            ReadMessageBlock(file, buffer);
            messages->AddTitle(id, buffer);
        }
        else if (buffer.find("DESCRIPTION") != buffer.npos)
        {
            ReadMessageBlock(file, buffer);
            // if the description starts with "-- FILE :" and "-- LINE :" we ignore this
            // same for "-- BLOCK" and "-- LINE".
            size_t tokenpos = buffer.find("-- FILE");
            if (buffer.npos == tokenpos) tokenpos = buffer.find("-- BLOCK");
            if (buffer.npos != tokenpos)
            {
                size_t eolpos = buffer.find('\n');
                if (buffer.npos != eolpos && eolpos > tokenpos)
                {
                    buffer.erase(0, eolpos + 1); // + 1 to erase also '\n'
                    tokenpos = buffer.find("-- LINE");
                    if (buffer.npos != tokenpos)
                    {
                        eolpos = buffer.find('\n');
                        if (buffer.npos != eolpos && eolpos > tokenpos)
                        {
                            buffer.erase(0, eolpos + 1); // + 1 to erase also '\n'
                        }
                    }
                }
            }
            messages->AddDescription(id, buffer);
        }
        else if (buffer.find("SOLUTION") != buffer.npos)
        {
            ReadMessageBlock(file, buffer);
            messages->AddSolution(id, buffer);
        }
        else
        {
            std::getline(file, buffer, '/'); // unknown, extract everything up to the next '/' and continue
        }
    }

    if (p_owningMessagePool && p_pMessagePool) delete p_pMessagePool;
    p_pMessagePool = messages;
    p_owningMessagePool = true;
    return true;
}

void hwReaderMessageList::ReadMessageBlock(std::ifstream& file, std::string& buffer)
{
    // extract everything up to the next '/'
    std::string buffer2;
    std::getline(file, buffer2, '/');
    // copy non-comment and non-empty lines
    buffer.resize(0);
    std::stringstream ss(buffer2);
    std::string buffer3;
    while (std::getline(ss, buffer3))
    {
        if (buffer3.size() > 0 && buffer3[0] != '#')
        {
            if (buffer.size() > 0) buffer.append("\n");
            buffer.append(buffer3);
        }
    }
    // remove trailing linebreaks and spaces
    while (buffer.size() > 0 && (buffer.back() == '\n' || buffer.back() == '\r' || buffer.back() == ' ')) buffer.pop_back();
}

int hwReaderMessageList::GetErrorCode(std::string errorString)
{
    std::size_t found = errorString.find("ERROR CODE:");
    if (found != std::string::npos)
    {
        std::size_t nextfound = errorString.find(" ", found + 12);
        std::string codestr = errorString.substr(found + 12, nextfound - (found + 11));
        int codeNo = atoi(codestr.c_str());
        return codeNo;
    }
    return 0;
}
