/*
 * libretroshare/src/ft ftFileProvider.h
 *
 * File Transfer for RetroShare.
 *
 * Copyright 2008 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#ifndef FT_FILE_PROVIDER_HEADER
#define FT_FILE_PROVIDER_HEADER

/* 
 * ftFileProvider.
 *
 */

class ftFileProvider
{
public:

	ftFileProvider(std::string path, uint64_t size, std::string hash);
	~ftFileProvider();

	/* array already allocated -
	 * just move chunk_size bytes to void *data buffer.
	 */
virtual bool 	getFileData(uint64_t offset, uint32_t chunk_size, void *data);

	/* File Info */
std::string getHash();
uint64_t getFileSize();


protected:

	std::string file_name;
	std::string hash;
	uint64_t    total_size;

	FILE *fd;

	/* Statistics */
	std::string lastRequestor;
	uint64_t   req_loc;
	uint32_t   req_size;
	time_t     lastTS;   
};


#endif // FT_FILE_PROVIDER_HEADER
