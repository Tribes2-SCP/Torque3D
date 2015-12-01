//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include <environment/sky.h>

#include "console/consoleTypes.h"

#include <core/fileObject.h>
#include "core/stream/bitStream.h"

IMPLEMENT_CO_NETOBJECT_V1( Sky );

ConsoleDocClass( Sky,
   "@brief Represents the sky with an artist-created cubemap.\n\n"

   "Sky is a wrapper object type for compatibility with Tribes 2 maps.\n\n"

   "@ingroup Atmosphere"
);

Sky::Sky() : SkyBox()
{
     mMaterialList = "";
}

Sky::~Sky()
{

}

bool Sky::onAdd()
{
   if ( !Parent::onAdd() )
      return false;

   if ( isClientObject() )
   {

   }

   return true;
}

U32 Sky::packUpdate( NetConnection *conn, U32 mask, BitStream *stream )
{
   U32 retMask = Parent::packUpdate( conn, mask, stream );

   stream->write(mMaterialList);

   return retMask;
}

void Sky::unpackUpdate( NetConnection *conn, BitStream *stream )
{
     Parent::unpackUpdate(conn, stream);

     String tmpString( "" );
     stream->read( &tmpString );
     if ( !tmpString.isEmpty() && !tmpString.equal( mMaterialList, String::NoCase ) )
     {
          mMaterialList = tmpString;

          FileObject handle;
          handle.readMemory(mMaterialList);
          if (handle.isEOF())
               Con::errorf("Sky:: Failed to open material list %s", tmpString.c_str());
          else
          {
               while (!handle.isEOF())
               {
                    const U8* line = handle.readLine();

                    // For each line read, we've found a texture name to use in our material.
               }

               handle.close();
          }
     }
     else if (tmpString.isEmpty())
          Con::errorf("Sky:: Must have a valid material list in unpackUpdate!");
}

void Sky::initPersistFields()
{
   Parent::initPersistFields();

   addGroup( "Sky Box" );

   addField( "materialList", TypeStringFilename, Offset( mMaterialList, Sky ),
      "The name of the material list to use for this sky." );

   endGroup( "Sky Box" );
}
