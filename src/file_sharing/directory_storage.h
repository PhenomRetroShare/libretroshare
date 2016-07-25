#pragma once

#include <string>
#include <stdint.h>
#include <list>

#include "retroshare/rsids.h"
#include "retroshare/rsfiles.h"

static const uint8_t DIRECTORY_STORAGE_VERSION               =  0x01 ;

static const uint8_t DIRECTORY_STORAGE_TAG_FILE_HASH         =  0x01 ;
static const uint8_t DIRECTORY_STORAGE_TAG_FILE_NAME         =  0x02 ;
static const uint8_t DIRECTORY_STORAGE_TAG_FILE_SIZE         =  0x03 ;
static const uint8_t DIRECTORY_STORAGE_TAG_DIR_NAME          =  0x04 ;
static const uint8_t DIRECTORY_STORAGE_TAG_MODIF_TS          =  0x05 ;
static const uint8_t DIRECTORY_STORAGE_TAG_RECURS_MODIF_TS   =  0x06 ;

class InternalFileHierarchyStorage ;

class DirectoryStorage
{
	public:
		DirectoryStorage(const std::string& local_file_name) ;
        virtual ~DirectoryStorage() {}

        typedef uint32_t EntryIndex ;
        static const EntryIndex NO_INDEX = 0xffffffff;

		void save() const ;

        virtual int searchTerms(const std::list<std::string>& terms, std::list<EntryIndex> &results) const { return 0;}
        virtual int searchHash(const RsFileHash& hash, std::list<EntryIndex> &results) const { return 0;}
        virtual int searchBoolExp(Expression * exp, std::list<EntryIndex> &results) const { return 0; }

		void getFileDetails(EntryIndex i) ;

		// This class allows to abstractly browse the stored directory hierarchy in a depth-first manner.
		// It gives access to sub-files and sub-directories below.
		//
		class DirIterator
		{
			public:
                DirIterator(const DirIterator& d) ;
                DirIterator(DirectoryStorage *d,EntryIndex i) ;

				DirIterator& operator++() ;
                EntryIndex operator*() const ;

                operator bool() const ;			// used in for loops. Returns true when the iterator is valid.

                // info about the directory that is pointed by the iterator

                const std::string& name() const ;
            private:
                EntryIndex mParentIndex ;		// index of the parent dir.
                uint32_t mDirTabIndex ;				// index in the vector of subdirs.
                InternalFileHierarchyStorage *mStorage ;

                friend class DirectoryStorage ;
        };
		class FileIterator
		{
			public:
                FileIterator(DirIterator& d);	// crawls all files in specified directory
                FileIterator(DirectoryStorage *d,EntryIndex e);		// crawls all files in specified directory

				FileIterator& operator++() ;
                EntryIndex operator*() const ;	// current file entry

                operator bool() const ;			// used in for loops. Returns true when the iterator is valid.

                // info about the file that is pointed by the iterator

                std::string name() const ;
                std::string fullpath() const ;
                uint64_t size() const ;
                RsFileHash hash() const ;
                time_t modtime() const ;

            private:
                EntryIndex mParentIndex ;		// index of the parent dir.
                uint32_t   mFileTabIndex ;		// index in the vector of subdirs.
                InternalFileHierarchyStorage *mStorage ;
        };

        virtual DirIterator root() ;					// returns the index of the root directory entry.

        void updateSubDirectoryList(const EntryIndex& indx,const std::set<std::string>& subdirs) ;
        void updateSubFilesList(const EntryIndex& indx,const std::set<std::string>& subfiles,std::set<std::string>& new_files) ;
        void removeDirectory(const EntryIndex& indx) ;

        void updateFile(const EntryIndex& parent_dir,const RsFileHash& hash, const std::string& fname, const uint32_t modf_time) ;
        void updateDirectory(const EntryIndex& parent_dir,const std::string& dname) ;

        void cleanup();

    private:
		void load(const std::string& local_file_name) ;
		void save(const std::string& local_file_name) ;

        void loadNextTag(const unsigned char *data, uint32_t& offset, uint8_t& entry_tag, uint32_t& entry_size) ;
        void saveNextTag(unsigned char *data,uint32_t& offset,uint8_t entry_tag,uint32_t entry_size) ;

        // storage of internal structure. Totally hidden from the outside. EntryIndex is simply the index of the entry in the vector.

        InternalFileHierarchyStorage *mFileHierarchy ;

        RsMutex mDirStorageMtx ;
};

class RemoteDirectoryStorage: public DirectoryStorage
{
public:
    RemoteDirectoryStorage(const RsPeerId& pid) ;
    virtual ~RemoteDirectoryStorage() {}
};

class LocalDirectoryStorage: public DirectoryStorage
{
public:
    LocalDirectoryStorage() ;
    virtual ~LocalDirectoryStorage() {}

    void setSharedDirectoryList(const std::list<SharedDirInfo>& lst) ;
    void getSharedDirectoryList(std::list<SharedDirInfo>& lst) ;

private:
    std::list<SharedDirInfo> mLocalDirs ;

    std::map<RsFileHash,EntryIndex> mHashes ;		// used for fast search access
};











