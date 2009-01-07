#ifndef BZ_VFS_H
#define BZ_VFS_H

#include "common.h"

// system headers
#include <string>
#include <vector>
#include <set>
#include <map>


#define BZVFS_CONFIG           "c"
#define BZVFS_DATA             "d"
#define BZVFS_FTP              "f"
#define BZVFS_HTTP             "h"
#define BZVFS_LUA_USER         "u"
#define BZVFS_LUA_WORLD        "w"
#define BZVFS_LUA_USER_WRITE   "U"
#define BZVFS_LUA_WORLD_WRITE  "W"
#define BZVFS_BASIC BZVFS_CONFIG BZVFS_DATA BZVFS_FTP BZVFS_HTTP


class BzDocket;


/******************************************************************************/

class BzFile { // FIXME -- not implemented
  public:
    BzFile();
    virtual ~BzFile();

    enum Whence { SET = 0, CUR = 1, END = 2 };

    virtual int  size() const = 0;
    virtual int  read(int count, void* buf) = 0;
    virtual int  read(int count, std::string& data) = 0;
    virtual bool readAll(std::string& data) = 0;
    virtual bool readLine(std::string& data) = 0;
    virtual int  write(int count, void* buf) = 0;
    virtual int  write(int count, std::string& data) = 0;
    virtual bool seek(int offset, Whence whence) = 0;
    virtual int  tell() const = 0;
    virtual bool eof() const = 0;
    virtual bool rewind() { return (seek(0, SET) == 0); }
};


/******************************************************************************/

class BzFS {
  public:
    virtual bool fileExists(const std::string& path) = 0;
    virtual int  fileSize(const std::string& path) = 0;
    virtual bool readFile(const std::string& path, std::string& data) = 0;
    virtual bool writeFile(const std::string& path, const std::string& data) = 0;
    virtual bool appendFile(const std::string& path, const std::string& data) = 0;
    virtual BzFile* openFile(const std::string& path, std::string* errMsg = NULL) = 0;

    // '/' is used as the directory separator character
    // files paths are full paths
    // dir paths are full paths, and are terminated with '/'
    virtual bool dirList(const std::string& path, bool recursive,
                         std::vector<std::string>& dirs,
                         std::vector<std::string>& files) = 0;

    inline bool isWritable() const { return writable; }
    inline void setWritable(bool value) { writable = value; }

  private:
    bool writable;
};


/******************************************************************************/

class BzVFS {
  public:
    static std::string filterModes(const std::string& modes,
                                   const std::string& allowed);
  public:
    BzVFS();
    ~BzVFS();

    void clear();
    void reset();

    bool addFS(const std::string& name, BzDocket* docket);
    bool addFS(const std::string& name, const std::string& root);
    bool removeFS(const std::string& name);
    bool setFSWritable(const std::string& name, bool value);

    const BzDocket* getDocket(const std::string& modes) const;

  public:
    bool fileExists(const std::string& path, const std::string& modes);
    int  fileSize(const std::string& path, const std::string& modes);
    bool readFile(const std::string& path,
                  const std::string& modes, std::string& data);
    bool writeFile(const std::string& path,
                   const std::string& modes, const std::string& data);
    bool appendFile(const std::string& path,
                    const std::string& modes, const std::string& data);
    bool dirList(const std::string& path, const std::string& modes,
                 bool recursive,
                 std::vector<std::string>& dirs,
                 std::vector<std::string>& files);

  public:
    static bool rawDirList(const std::string& root, // not included in results
                           const std::string& path, // path starting from root
                           bool recursive,
                           std::vector<std::string>& dirs,
                           std::vector<std::string>& files);

    static std::string cleanDirPath(const std::string& path);
    static std::string cleanFilePath(const std::string& path);

  protected:
    bool safePath(const std::string& path);
    void getSystems(const std::string& modes, std::vector<BzFS*>& fileSystems);

  private:
    typedef std::map<std::string, BzFS*> FSMap;
    FSMap fsMap;
};


extern BzVFS bzVFS;


/******************************************************************************/


#endif // BZ_VFS_H