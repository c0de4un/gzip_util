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

// HEADER
#include "ZStream.hpp"

namespace c0de4un
{

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
	ZStream::ZStream( const std::uint32_t bufferSize )
	{
	}

	/* ZStream destructor */
	ZStream::~ZStream( )
	{
	}

	// ===========================================================
	// Methods
	// ===========================================================

	/*
	 * Compress file.
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
	const int & ZStream::deflateFILE( std::FILE *const srcFile, std::FILE *const dstFile, const std::uint32_t & bufferSize, const int & compressionLevel )
	{

		// Set in & out binary mode on MSVC to prevent 'end-of-line' char adding
		SET_BINARY_MODE( stdin );
		SET_BINARY_MODE( stdout );

		// Return cde
		int zRet( 0 );

		// Flush code
		int zFlush( 0 );

		// Elements count to write
		std::uint32_t zOutCount( 0 );

		// Input-buffer for z_stream
		unsigned char * inBuffer;

		// Output-buffer for z_stream
		unsigned char * outBuffer;

		// z_stream
		z_stream zStream;

		// Set z_stream & deflate state
		zStream.zalloc = Z_NULL;
		zStream.zfree = Z_NULL;
		zStream.opaque = Z_NULL;

		// Guarded-Block
		try
		{

			// Allocate input-buffer
			inBuffer = new unsigned char[bufferSize];

			// Allocate output-buffer
			outBuffer = new unsigned char[bufferSize];

			// Check buffers
			if ( inBuffer == nullptr || outBuffer == nullptr )
				throw std::exception( "ZStream::deflateFILE - failed to allocate input output buffer" );

			// Initialze deflate
			zRet = deflateInit( &zStream, compressionLevel );

			// Check z_stream
			if ( zRet != Z_OK )
			{

				switch ( zRet )
				{
					case Z_VERSION_ERROR:
						throw std::exception( "ZStream::deflateFILE - failed to initialize deflate, zlib verion conflict." );
						break;
					case Z_STREAM_ERROR:
						throw std::exception( "ZStream::deflateFILE - failed to initialize deflate, wrong compression level" );
						break;
					case Z_MEM_ERROR:
						throw std::exception( "ZStream::deflateFILE - failed to initialize deflate, don't have enough memory." );
						break;
					default:
						throw std::exception( "ZStream::deflateFILE - failed to initialize deflate, unknown reason." );
				}

			}

			// Read all data from file
			while ( zFlush != Z_FINISH )
			{

				// Read input-file
				zStream.avail_in = fread( inBuffer, sizeof( unsigned char ), bufferSize, srcFile );

				// Check io errors
				if ( ferror( srcFile ) )
					throw std::exception( "ZStream::deflateFILE - io error, can't read input file !" );

				// Set z_stream flush value
				zFlush = feof( srcFile ) ? Z_FINISH : Z_NO_FLUSH;

				// Update z_stream input-buffer.
				zStream.next_in = inBuffer;

				// Update z_stream avail_out to avoid bugs.
				zStream.avail_out = 0;

				// Compress until out of data for the output-buffer.
				while ( zStream.avail_out == 0 )
				{

					// Set z_stream number of elements to output
					zStream.avail_out = bufferSize;

					// Set z_stream output-buffer
					zStream.next_out = outBuffer;

					// Compress & get result.
					zRet = deflate( &zStream, zFlush );

					// Check compression result-status.
					if ( zRet == Z_STREAM_ERROR )
						throw std::exception( "ZStream::deflateFILE - compression failed, stream error" );

					// Count elements to write in the output-file.
					zOutCount = bufferSize - zStream.avail_out;

					// Write output-file
					if ( fwrite( outBuffer, sizeof( unsigned char ), zOutCount, dstFile ) != zOutCount || ferror( dstFile ) )
						throw std::exception( "ZStream::deflateFILE - failed to write output file" );


				}// while ( zStream.avail_out == 0 )

				// Check if all data are compressed
				if ( zStream.avail_in != 0 )
					throw std::exception( "ZStream::deflateFILE - not all input data compressed !" );

			}// while ( zFlush != Z_FINISH )

			// Check flus-code
			//if ( zFlush != Z_STREAM_END ) // May cause bug, when Z_STREAM_END not set event if deflate called last time.
				//throw std::exception( "ZStream::deflateFILE - z_stream not properly closed (finished) !" );

		}
		catch ( const std::exception & pException )
		{

			// Print ERROR-message
			std::cout << "ZStream::deflateFILE - error: " << pException.what( ) << std::endl;

			// Release z_stream resources
			deflateEnd( &zStream );

			// Release input-buffer
			if ( inBuffer != nullptr )
			{

				delete[] inBuffer;
				inBuffer = nullptr;

			}

			//Release output-buffer
			if ( outBuffer != nullptr )
			{

				delete[] outBuffer;
				outBuffer = nullptr;

			}

			// Return ERROR
			return( Z_ERRNO );

		}
		catch ( ... )
		{

			// Print ERROR-message
			std::cout << "ZStream::deflateFILE - unknown error" << std::endl;

			// Release z_stream resources
			deflateEnd( &zStream );

			// Release input-buffer
			if ( inBuffer != nullptr )
			{

				delete[] inBuffer;
				inBuffer = nullptr;

			}

			//Release output-buffer
			if ( outBuffer != nullptr )
			{

				delete[] outBuffer;
				outBuffer = nullptr;

			}

			// Return ERROR
			return( Z_ERRNO );

		}

		// Release z_stream resources
		deflateEnd( &zStream );

		// Release input-buffer
		delete[] inBuffer;
		inBuffer = nullptr;

		//Release output-buffer
		delete[] outBuffer;
		outBuffer = nullptr;

		// Return Z_OK
		return( Z_OK );

	}

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
	const int ZStream::inflateFILE( std::FILE *const srcFile, std::FILE *const dstFile, const std::uint32_t & bufferSize )
	{

		// Set in & out binary mode on MSVC to prevent 'end-of-line' char adding
		SET_BINARY_MODE( stdin );
		SET_BINARY_MODE( stdout );

		// Return cde
		int zRet( 0 );

		// Flush code
		int zFlush( 0 );

		// Elements count to write
		std::uint32_t zOutCount( 0 );

		// Input-buffer for z_stream
		unsigned char * inBuffer;

		// Output-buffer for z_stream
		unsigned char * outBuffer;

		// z_stream
		z_stream zStream;

		// Set z_stream state
		zStream.zalloc = Z_NULL;
		zStream.zfree = Z_NULL;
		zStream.opaque = Z_NULL;
		zStream.avail_in = 0;
		zStream.next_in = Z_NULL;

		// Guarded-Block
		try
		{

			// Allocate input-buffer
			inBuffer = new unsigned char[bufferSize];

			// Allocate output-buffer
			outBuffer = new unsigned char[bufferSize];

			// Check buffers
			if ( inBuffer == nullptr || outBuffer == nullptr )
				throw std::exception( "ZStream::deflateFILE - failed to allocate input output buffer" );

			// Initialize inflate
			zRet = inflateInit( &zStream );

			// Check initialization state
			if ( zRet != Z_OK )
			{

				// Handle code
				switch ( zRet )
				{

				case Z_MEM_ERROR:
					throw std::exception( "ZStream::deflateFILE - failed to initialize decompression stream, not enough memory." );
					break;

				case Z_VERSION_ERROR:
					throw std::exception( "ZStream::deflateFILE - failed to initialize decompression stream, zlib version conflict." );
					break;

				case Z_STREAM_ERROR:
					throw std::exception( "ZStream::deflateFILE - failed to initialize decompression stream, arguments are invalid." );
					break;

				default:
					throw std::exception( "ZStream::deflateFILE - failed to initialize decompression stream, unknown reason." );

				}

			}

			// Read input-file
			do
			{

				// Read & update z_stream input elements counter
				zStream.avail_in = fread( inBuffer, sizeof( unsigned char ), bufferSize, srcFile );

				// Check read-status
				if ( ferror( srcFile ) )
					throw std::exception( "ZStream::deflateFILE - can't read source-file !" );

				// Stop if no data
				if ( zStream.avail_in == 0 )
					break;

				//Update z_stream input buffer
				zStream.next_in = inBuffer;

				// Reset z_stream.avail_out to avoid bug
				zStream.avail_out = 0;

				// Decompress
				do
				{

					// Update z_stream avail_out
					zStream.avail_out = bufferSize;

					// Update z_stream output-buffer
					zStream.next_out = outBuffer;

					// Decompress data (Z_NO_FLUSH means all possinle data, using full buffer size)
					zRet = inflate( &zStream, Z_NO_FLUSH );

					// Check inflate-status
					switch ( zRet )
					{

					case Z_DATA_ERROR:
						throw std::exception( "ZStream::deflateFILE - decompression (inflate) failed, data corrupted." );
						break;

					case Z_MEM_ERROR:
						throw std::exception( "ZStream::deflateFILE - decompression (inflate) failed, insufficent memory" );
						break;

					case Z_BUF_ERROR:
						throw std::exception( "ZStream::deflateFILE - decompression (inflate) failed, data can't fit output-buffer." );
						break;

					case Z_NEED_DICT:
						throw std::exception( "ZStream::deflateFILE - decompression (inflate) failed, dictionary required." );
						break;

					case Z_STREAM_ERROR:
						throw std::exception( "ZStream::deflateFILE - decompression (inflate) failed, stream structure inconsistent (some params are not set)." );
						break;

					}

					// Count output elements
					zOutCount = bufferSize - zStream.avail_out;

					// Write uncompressed output
					if ( fwrite( outBuffer, sizeof( unsigned char ), zOutCount, dstFile ) != zOutCount || ferror( dstFile ) )
						throw std::exception( "failed to write decompressed output" );

				} while ( zStream.avail_out == 0 );

			} while ( zRet != Z_STREAM_END );

			// Release z_stream resources
			inflateEnd( &zStream );

			// Release input-buffer
			delete[] inBuffer;
			inBuffer = nullptr;

			//Release output-buffer
			delete[] outBuffer;
			outBuffer = nullptr;

			// Return
			return( zRet == Z_STREAM_END ? Z_OK : Z_DATA_ERROR );

		}
		catch ( const std::exception & pException )
		{

			// Print ERROR-message
			std::cout << "ZStream::inflateFILE - error: " << pException.what( ) << std::endl;

			// Release z_stream resources
			inflateEnd( &zStream );

			// Release input-buffer
			if ( inBuffer != nullptr )
			{

				delete[] inBuffer;
				inBuffer = nullptr;

			}

			//Release output-buffer
			if ( outBuffer != nullptr )
			{

				delete[] outBuffer;
				outBuffer = nullptr;

			}

			// Return ERROR
			return( Z_ERRNO );

		}
		catch ( ... )
		{

			// Print ERROR-message
			std::cout << "ZStream::inflateFILE - unknown error" << std::endl;

			// Release z_stream resources
			inflateEnd( &zStream );

			// Release input-buffer
			if ( inBuffer != nullptr )
			{

				delete[] inBuffer;
				inBuffer = nullptr;

			}

			//Release output-buffer
			if ( outBuffer != nullptr )
			{

				delete[] outBuffer;
				outBuffer = nullptr;

			}

			// Return ERROR
			return( Z_ERRNO );

		}

		// Return OK
		return( Z_OK ); // Unreachable code, but required by MSVC

	}

	// -------------------------------------------------------- \\

}