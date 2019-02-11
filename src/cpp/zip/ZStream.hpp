/*
* Copyright © 2018 Denis Zyamaev (code4un@yandex.ru) All rights reserved.
* Authors: Denis Zyamaev (code4un@yandex.ru)
* All rights reserved.
* API: C++ 11
* License: see LICENSE.txt
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must display the names 'Denis Zyamaev' and
* in the credits of the application, if such credits exist.
* The authors of this work must be notified via email (code4un@yandex.ru) in
* this case of redistribution.
* 3. Neither the name of copyright holders nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
* IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

// Include 'pch_cxx'
#include "../pch_cxx.hpp"

// Hack for Windows to avoid binary data corruption & casting end-of-line characters
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

namespace c0de4un
{

	// -------------------------------------------------------- \\

	// ===========================================================
	// Types
	// ===========================================================

	/*
	  * ZStream - utility-class to simpify work with compressing/decompressing
	  * data (directly files or stream).
	  * 
	  * @language C++ 11
	  * 
	  * @authors Z. Denis (c0de4un@yandex.ru)
	  * @version 1.0
	 */
	class ZStream final
	{

	private:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Fields
		// ===========================================================

		// -------------------------------------------------------- \\

	public:

		// -------------------------------------------------------- \\

		// ===========================================================
		// Constructor & destructor
		// ===========================================================

		/*
		 * ZStream constructor.
		 * 
		 * @param bufferSize - initial buffer size. Increased, if required,
		 * by +50% each time.
		 * @throws - can throw exception (bad_alloc).
		*/
		explicit ZStream( const std::uint32_t bufferSize = 65536 );

		/* ZStream destructor */
		~ZStream( );

		// ===========================================================
		// Methods
		// ===========================================================

		/*
		 * Compress the given file using zlib (not gzip).
		 *
		 * @thread_safety - not thread-safe.
		 * @param srcFile - file to compress.
		 * @param dstFile - output file.
		 * @param bufferSize - initial buffer size. Increased, if required,
		 * by +50% each time.
		 * @param compressionLevel - Compression-Level to use, must be in range 0-9.
		 * @return - Z_OK if compression complete.
		 * @throws - can throw exception.
		*/
		static const int & deflateFILE( std::FILE *const srcFile, std::FILE *const dstFile, const std::uint32_t & bufferSize, const int & compressionLevel );

		/*
		 * Decompress given file using zlib (not gzip).
		 * 
		 * @thread_safety - not thread-safe.
		 * @param srcFile - file to decompress (inflate). Must be compressed by the same zlib version and method.
		 * @param dstFile - output file path, must be other then source.
		 * @param bufferSize - initial buffer size. Increased, if required,
		 * by +50% each time.
		 * @return - Z_OK if sucessfull, error-code otherwise.
		 * @throws - can throw exception.
		*/
		static const int inflateFILE( std::FILE *const srcFile, std::FILE *const dstFile, const std::uint32_t & bufferSize );

		// -------------------------------------------------------- \\

	};

	// -------------------------------------------------------- \\

}