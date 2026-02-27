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




////////////////////////////////////////////////////////////////////////////////////

#if !defined(hwReaderMessage__INCLUDED_)
#define hwReaderMessage__INCLUDED_

#include <MODEL_IO/hw_cfg_reader_message.h>

#define hwReaderMessagePool HWCFGReaderMessagePool

class hwReaderMessageList : public HWCFGReaderMessageList
{
public:
    hwReaderMessageList(const hwReaderMessagePool *pMessagePool = NULL, bool owningMessagePool = false)
        : HWCFGReaderMessageList(pMessagePool, owningMessagePool)
    {}

    bool ReadMessageFile(std::string filename);

private:

    void ReadMessageBlock(std::ifstream& file, std::string& buffer);
    virtual int GetErrorCode(std::string errorString);
    virtual int GetDefaultId(int type) { return 9997 + type; }
};

#endif //! !defined(hwReaderMessage__INCLUDED_)
