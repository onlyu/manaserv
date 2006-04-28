/*
 *  The Mana World Server
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  $Id$
 */

#include "messagein.h"

#include <string>

#include <enet/enet.h>

#include "packet.h"

MessageIn::MessageIn(Packet *packet):
    mPacket(packet),
    mPos(0)
{
    // Read the message ID
    mId = readShort();
}

MessageIn::~MessageIn()
{
    delete mPacket; // To be removed if the packet is deleted elsewhere.
}

char MessageIn::readByte()
{
    if (mPacket)
    {
        if ( mPos < mPacket->length ) // if there is enough to read
        {
            return mPacket->data[mPos++];
        }
        else
        {
            mPos = mPacket->length - 1;
            return 0;
        }
    }
    return -1;
}

short MessageIn::readShort()
{
    short value = -1;
    
    if (mPacket)
    {
        if ( (mPos + sizeof(short)) <= mPacket->length )
        {
            value = ENET_NET_TO_HOST_16(*(short *)(mPacket->data + mPos));
            mPos += sizeof(short);
        }
        else
        {
            // We do nothing. And keep what's left to be read by a function 
            // that requires less length.
        }
    }
    return value;
}

long MessageIn::readLong()
{
    long value = -1;
    
    if (mPacket)
    {
        if ( (mPos + sizeof(long)) <= mPacket->length )
        {
            value = ENET_NET_TO_HOST_32(*(long *)(mPacket->data + mPos));
            mPos += sizeof(long);
        }
        else
        {
            // We do nothing. And keep what's left to be read by a function 
            // that requires less length.
        }
    }
    return value;
}

std::string MessageIn::readString(int length)
{
    int stringLength = 0;
    std::string readString = "";

    if (mPacket)
    {
        // Get string length
        if (length < 0) {
            stringLength = readShort();
        } else {
            stringLength = length;
        }

        // Make sure the string isn't erroneus
        if (mPos + length > mPacket->length) {
            return "";
        }

        // Read the string
        char *tmpString = new char[stringLength + 1];
        memcpy(tmpString, (void*)&mPacket->data[mPos], stringLength);
        tmpString[stringLength] = 0;
        mPos += stringLength;

        readString = tmpString;
        delete[] tmpString;
    }

    return readString;
}
