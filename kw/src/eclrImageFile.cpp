//=============================================================================
//
//                      Copyright 2009 by KW Software GmbH                     
//                                                                             
//                      KW Software GmbH                                       
//                      Lagesche Str. 32                                       
//                      D-32657 Lemgo                                          
//                      Germany                                                
//                                                                             
//-----------------------------------------------------------------------------
// Module Name: eCLR image file interface implementation
//-----------------------------------------------------------------------------
// Author: Xiangyang Ren                           
//-----------------------------------------------------------------------------
// Change History:
// ----------------------------------------------------------------------------
// 	Jun. 15th, 2009 by X. Ren - created
//
// ============================================================================

// ----------------------------------------------------------------------------
// Pragma�s
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Includes 
// ----------------------------------------------------------------------------

#include "eclrImageFile.h"
#include "eclr.h"
#include "log.h"

// ----------------------------------------------------------------------------
// Definitions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// User Type Declarations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Name:        s_pPersistentName
//
// Description: Defines the constant string used to load and store the 
//              persistent image files.
// ----------------------------------------------------------------------------
const char* PcosImageFile::s_pPersistentName        = "bootfile.img";

// ----------------------------------------------------------------------------
// Name:        s_pTmpName
//
// Description: Defines the constant string used to load and store the
//              temporary image file.
// ----------------------------------------------------------------------------
const char* PcosImageFile::s_pTmpPersistentName     = "_bootfile.img";
   
// ----------------------------------------------------------------------------
// Name:        s_pNonPersistentName
//
// Description: Defines the constant string used to load and store the
//              non persistent image file.
// ----------------------------------------------------------------------------
const char* PcosImageFile::s_pNonPersistentName     = "bootfile.tmp";      

// ----------------------------------------------------------------------------
// Name:        s_pTmpNonPersistentName
//
// Description: Defines the constant string used to load and store the 
//              nonpersistent tmp boot project files.
// ----------------------------------------------------------------------------
const char* PcosImageFile::s_pTmpNonPersistentName  = "_bootfile.tmp";


// ----------------------------------------------------------------------------
// Data Object Declarations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Private Method Declarations
// ----------------------------------------------------------------------------

// ============================================================================
// Start of Implementation
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Private Methods
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Global Methods
// ----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Name:        PcosImageFile::PcosImageFile
//
// Description: Constructor
//
// Return:      None
//
// Parameters:  None
// ---------------------------------------------------------------------------

PcosImageFile::PcosImageFile()
{
    openCnt = 0;
}

// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Open
//
// Description: Opens a new or existing image file.
//
// Return:      bool - true == file opened (created), false otherwise
//
// Parameters:  int mode - support Create, WrOnly, RdOnly, and 
//                         NonPersistent. Do not support RdWr at this time.
//              unsigned length - not used
// ---------------------------------------------------------------------------

bool PcosImageFile::Open(int mode, unsigned length)
{
    bool result = false;
    length = length;

    if (m_pFile == NULL)
    {
        if (mode & Create)
        {
            // create a new image file, open for writing
            m_pTmpName = s_pTmpPersistentName;
            if (mode & NonPersistent)
            {
                m_pTmpName = s_pTmpNonPersistentName;
            }
            m_pImageName = m_pTmpName;
            // create a new bootfile
            m_pFile = EclrFile::Open(m_pTmpName, EclrFile::Open_c);
            MSG(("imagefile: open for writing %s , mode=%d, opencnt=%d, file=%08X", m_pTmpName, mode, openCnt, m_pFile));
        }
        else
        {
            // open for existing image  for reading
            m_pImageName = m_pTmpName;
            if (m_pImageName == NULL)
            {
                // there is no temporary image (image is activated), take the real image
                m_pImageName = s_pPersistentName;
                if (mode & NonPersistent)
                {
                    m_pImageName = s_pNonPersistentName;
                }
            }
            else
            {
                // access to the temorary image file (image not yet activated)
            }
            
            m_pFile = EclrFile::Open(m_pImageName, EclrFile::Open_r);
            MSG(("imagefile: open for reading %s , mode=%d, opencnt=%d, file=%08X", m_pImageName, mode, openCnt, m_pFile));
        }

    }
    if (m_pFile != NULL)
    {
        openCnt++;
        result = true;
    }
    return result;
}

// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Close
//
// Description: Closes the open file referenced by internal handle!
//
// Return:      void
//
// Parameters:  void
// ---------------------------------------------------------------------------

void PcosImageFile::Close()
{
    openCnt--;
    MSG(("imagefile: %s closed, opencnt=%d", m_pImageName, openCnt));
    if (m_pFile != NULL)
    {
        EclrFile::Close(m_pFile);
        m_pFile = NULL;
    }
}

// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Rename
//
// Description: change the name of a file!
//
// Return:      void
//
// Parameters:  void
// ---------------------------------------------------------------------------

int PcosImageFile::Rename(const char* pSrcName, const char* pDstName)
{ 

  if(rename(pSrcName, pDstName) != 0)
  {
     return -1;
  }

  return 0;

  /*
  char buf[2048];
  unsigned result, bufLen = 2048;
  EclrFile* pSrcFile = (EclrFile*)NULL;
  EclrFile* pDstFile = (EclrFile*)NULL;
 
  // first open the source file
  pSrcFile = EclrFile::Open(pSrcName , EclrFile::Open_r);
  if(pSrcFile != NULL)
  {
    // open destination file
    pDstFile = EclrFile::Open(pDstName , EclrFile::Open_c);
    if(pDstFile == NULL)
    {
       EclrFile::Close(pSrcFile);
       return -1;
    }
    
    // read the content of source file
    result = EclrFile::Read(pSrcFile, buf, bufLen);
    while(result == bufLen)
    {
      // write the content to the destination file
      EclrFile::Write(pDstFile, buf, bufLen);
      
      // read next block
      result = EclrFile::Read(pSrcFile, buf, bufLen);
    }
    EclrFile::Write(pDstFile, buf, result);
    
  }
    
  // close both file
  EclrFile::Close(pSrcFile);
  EclrFile::Close(pDstFile);

  return 0;    
*/
}

// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Activate
//
// Description: Activate is called by eCLR after an image is downloaded and 
//              verified. Activate renames the temporary image (just 
//              downloaded) to the persistent name. If a previous image 
//              exists than it is backed up and deleted if the temporary file 
//              renamed successfully or restored if failed to rename the 
//              temporary file. If persistent flag is not set than the file is 
//              left with the temporary name so that on cold boot it will not 
//              get loaded (aka no-persistent).
//
// Return:      bool - true == success, false == failure
//
// Parameters:  const char* pAppName - Application name
//              bool isPersistent - if true than it is persistent
//              bool isOK - if true than downloaded file is OK 
//
// ---------------------------------------------------------------------------

bool PcosImageFile::Activate(const char* pAppName, bool isPerstent, bool isOK)
{
    pAppName = pAppName;    // skip warnings
   
    m_pImageName = s_pPersistentName;
    if (isPerstent == false)
    {
        m_pImageName = s_pNonPersistentName;
    }
    
    bool result = true;
    if (isOK == true)
    {
        if (m_pTmpName != NULL)
        {
            // first remove the current image file
            int r = EclrFile::Remove(m_pImageName);
            MSG(("imagefile: remove %s, result=%d", m_pImageName, r));
            // second, rename the temporarily file
            r = rename(m_pTmpName, m_pImageName);
            //r = xcopy(m_pTmpName, m_pImageName);            
            remove(m_pTmpName);
            MSG(("imagefile: rename %s, %s, result=%d", m_pTmpName, m_pImageName, r));
            if (r != 0)
            {
                result = false;
            }
            
        }
        else
        {
            result = false;
        }
    }
    // now the image is activated
    m_pTmpName = NULL;
    MSG(("imagefile: activate %s, result=%d", m_pImageName, result));

    return result;
}


// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Remove
//
// Description: Deletes the image file referenced by internal handle.
//
// Return:      void
//
// Parameters:  void
//
// ---------------------------------------------------------------------------

void PcosImageFile::Remove()
{
    // remove the persistent image file
    int result = EclrFile::Remove(s_pPersistentName);
    result = result; // prevent warning
    MSG(("imagefile: remove %s, opencnt=%d, result=%d", s_pPersistentName, openCnt, result));
}



// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Read
//
// Description: Read requested number of bytes beginning at specified position.
//
// Return:      unsigned - Number of bytes read.
//
// Parameters:  unsigned pos - Offset to actual position
//              void* pData - Pointer to data buffer
//              unsigned length - Number of bytes to read
//
// ---------------------------------------------------------------------------

unsigned PcosImageFile::Read(unsigned pos, void* pData, unsigned length)
{
    EclrFile::Seek(m_pFile, (long)pos, EclrFile::Seek_set);
    unsigned result = EclrFile::Read(m_pFile, pData, length);
    return result; 
}


// ---------------------------------------------------------------------------
// Name:        PcosImageFile::Write
//
// Description: Write method required by eCLR's EclrImageFile class 
//              implementation.
//
// Return:      unsigned - Amount of actual bytes written, zero implies 
//                         failure.
//
// Parameters:  pos - unsigned - Position in file to write data
//              pData - void* - Pointer to data to write
//              length - unsigned - length of data to write
//
// Assumptions: File to write too was already opened and has a valid handle.
// ---------------------------------------------------------------------------

unsigned PcosImageFile::Write(unsigned pos, void* pData, unsigned length)
{
    // for upload write not supported
    EclrFile::Seek(m_pFile, (long)pos, EclrFile::Seek_set);
    unsigned result = EclrFile::Write(m_pFile, pData, length);
    return result; 
}

// ----------------------------------------------------------------------------
// End of Implementation
// ============================================================================
