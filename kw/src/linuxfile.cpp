#include "eclr.h"

#ifdef FILENAME_AS_UTF8_SUPPORT
EclrFile* EclrFile::Open(const char* fname , enum OpenFlags mode)
{
    FILE *result = NULL;
    // use wchar version
    int nMbyte = strlen(fname);
    int wBufferSize = nMbyte*2;

    return (EclrFile*)result;
}
#else
EclrFile* EclrFile::Open(const char* fname , enum OpenFlags mode)
{
    FILE *result;
    switch(mode)
    {
    case Open_r:
        result = fopen(fname,"rb");
        break;
    case Open_w:
        result = fopen(fname,"wb");
        break;
    case Open_rw:
        result = fopen(fname,"r+b");
        break;
    case Open_c:
        result = fopen(fname,"w+b");
        break;
    default:
        result = (FILE*)NULL;
        break;
    }
    return (EclrFile*)result;
}
#endif //#ifdef FILENAME_AS_UTF8_SUPPORT

#ifdef FILENAME_AS_UTF8_SUPPORT
bool EclrFile::Remove(const char* fname)
{
	bool result = false;
    // use wchar version
    // use wchar version
    int nMbyte = strlen(fname);
    int wBufferSize = nMbyte*2;

    return result;
}
#else
bool EclrFile::Remove(const char* fname)
{
	bool result = false;
    if (::remove(fname) == 0)
    {
        result = true;
    }
    return result;
}
#endif

bool EclrFile::Close(EclrFile* pFile)
{
    bool result = false;
    if (pFile != NULL)
    {
        fclose((FILE*)pFile);
        result = true;
    }
    return result;
}

int EclrFile::Read(EclrFile* pFile, void* buffer, int size)
{
    int nRVal = fread(buffer, 1, size, (FILE*)pFile);
    return nRVal;
}

int EclrFile::Write(EclrFile* pFile, void* buffer, int size)
{
    int nRVal = fwrite(buffer, 1, size, (FILE*)pFile);
    return nRVal;
}

long EclrFile::Seek(EclrFile* pFile, long offset, enum SeekModes seekMode)
{
    int newSeekMode = 0;
    long retVal = (long)0;

    switch(seekMode)
    {
    case Seek_set:
        newSeekMode =(int)SEEK_SET;
        break;
    case Seek_cur:
        newSeekMode = (int)SEEK_CUR;
        break;
    case Seek_end:
        newSeekMode = (int)SEEK_END;
        break;
    default:
        retVal = (long)-1;
        break;
    }
    if(retVal != (long)-1)
    {
        if(fseek((FILE*)pFile, offset, newSeekMode) == 0)
        {
            retVal = ftell((FILE*)pFile);
        }
        else
        {
            retVal = (long)-1;
        }
    }
    return retVal;
}
