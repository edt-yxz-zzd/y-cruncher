/* BaseFile_Windows.ipp
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 12/30/2015
 * Last Modified    : 12/30/2015
 * 
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <Windows.h>
#include "PublicLibs/BasicLibs/StringTools/Unicode.h"
#include "BaseFile_Windows.h"
namespace ymp{
namespace FileIO{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
const upL_t BLOCK_SIZE = (upL_t)64 << 20;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  BaseFile
bool BaseFile::open(std::string path){
    close();

    m_filehandle = CreateFileW(
        StringTools::utf8_to_wstr(path).c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        NULL,
        NULL
    );
    if (m_filehandle == INVALID_HANDLE_VALUE){
        return false;
    }

    m_path = std::move(path);
    return true;
}
bool BaseFile::create(std::string path, ufL_t bytes){
    close();

    m_filehandle = CreateFileW(
        StringTools::utf8_to_wstr(path).c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_FLAG_WRITE_THROUGH,
        NULL
    );
    if (m_filehandle == INVALID_HANDLE_VALUE){
        return false;
    }

    LARGE_INTEGER t;
    t.QuadPart = (LONGLONG)bytes;
    SetFilePointerEx(m_filehandle, t, NULL, FILE_BEGIN);
    SetEndOfFile(m_filehandle);

    m_path = std::move(path);
    return true;
}
void BaseFile::close(bool delete_file){
    if (m_path.empty()){
        return;
    }

    CloseHandle(m_filehandle);
    m_filehandle = nullptr;

    if (delete_file){
        _wremove(StringTools::utf8_to_wstr(m_path).c_str());
    }

    m_path.clear();
}
bool BaseFile::set_ptr(ufL_t offset){
    LARGE_INTEGER t;
    t.QuadPart = (LONGLONG)offset;
    return SetFilePointerEx(m_filehandle, t, nullptr, FILE_BEGIN) != 0;
}
void BaseFile::flush(){
    FlushFileBuffers(m_filehandle);
}
upL_t BaseFile::read(void* T, upL_t bytes){
    upL_t total_read = 0;

    DWORD bytes_read;
    while (bytes > BLOCK_SIZE){
        int ret = !ReadFile(m_filehandle, T, (DWORD)BLOCK_SIZE, &bytes_read, nullptr);
        if (ret || bytes_read != BLOCK_SIZE){
            total_read += bytes_read;
            return total_read;
        }
        total_read += BLOCK_SIZE;
        bytes -= BLOCK_SIZE;
        T = (void*)((upL_t)T + BLOCK_SIZE);
    }

    ReadFile(m_filehandle, T, (DWORD)bytes, &bytes_read, nullptr);
    total_read += bytes_read;
    return total_read;
}
upL_t BaseFile::write(const void* T, upL_t bytes){
    upL_t total_written = 0;

    DWORD bytes_written;
    while (bytes > BLOCK_SIZE){
        int ret = !WriteFile(m_filehandle, T, (DWORD)BLOCK_SIZE, &bytes_written, nullptr);
        if (ret || bytes_written != BLOCK_SIZE){
            total_written += bytes_written;
            return total_written;
        }
        total_written += BLOCK_SIZE;
        bytes -= BLOCK_SIZE;
        T = (void*)((upL_t)T + BLOCK_SIZE);
    }

    WriteFile(m_filehandle, T, (DWORD)bytes, &bytes_written, nullptr);
    total_written += bytes_written;
    return total_written;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
}
