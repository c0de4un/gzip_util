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
#include "main.hpp"

/*
 * Converts string-command to Command-ID.
 * 
 * @param pCommand - command in string.
 * @return - Command-ID. Default is CONSOLE_COMMAND_ID_HELP.
*/
static std::uint8_t getCommandID( const char *const pCommand )
{

	// Handle Command
	if ( pCommand == "help" )
		return( CONSOLE_COMMAND_ID_HELP );

	// Return Default
	return( CONSOLE_COMMAND_ID_HELP );

}

/*
 * Autamtion-method. Runs commands with arguments.
 * 
 * @param pArguments - arguments (my_file.txt;my_file.zip).
 * @param pCommand - Command-ID.
*/
static void handleCommand( const char *const pArguments, const std::uint8_t pCommand )
{

}

/*
 * Decompress (inflate) file.
 * 
 * @param srcFile - file (archive, not gzip) to decompress (inflate).
 * @param dstFile - path to result output.
*/
void decompressFile( const char *const srcFile, const char *const dstFile )
{

	// Input FILE
	std::FILE * inputFILE( nullptr );

	// Output FILE
	std::FILE * outFILE( nullptr );

	// FILE fopen_s errno
	errno_t errCode;

	// Open input (source) FILE
	errCode = fopen_s( &inputFILE, srcFile, "rb" );

	// Check errors
	if ( errCode != 0 || inputFILE == nullptr )
	{

		// Print ERROR-message
		std::cout << "failed to open input-file #" << srcFile << std::endl;

		// Cancel
		return;

	}

	// Open output (destination) FILE
	errCode = fopen_s( &outFILE, dstFile, "wb" );

	// Check errors
	if ( errCode != 0 || inputFILE == nullptr )
	{

		// Print ERROR-message
		std::cout << "failed to open output-file #" << dstFile << std::endl;

		// Close Input FILE
		if ( inputFILE != nullptr )
		{

			std::fclose( inputFILE );
			inputFILE = nullptr;

		}

		// Return
		return;

	}

	// Guarded-Block
	try
	{

		// Inflate (decompress) & write output to result-file.
		if ( c0de4un::ZStream::inflateFILE( inputFILE, outFILE, 16384 ) != Z_OK )
			std::cout << "decompression failed for file#" << srcFile << std::endl;
		else
			std::cout << "decompression completed for file#" << srcFile << std::endl;

		// Close Input FILE
		std::fclose( inputFILE );
		inputFILE = nullptr;

		// Close Output FILE
		std::fclose( outFILE );
		outFILE = nullptr;
	}
	catch ( const std::exception & pException )
	{

		// Print ERROR-message
		std::cout << "failed to decompress file#" << srcFile << ", error: " << pException.what( ) << std::endl;

		// Close Input FILE
		if ( inputFILE != nullptr )
		{

			std::fclose( inputFILE );
			inputFILE = nullptr;

		}

		// Close Output FILE
		if ( outFILE != nullptr )
		{

			std::fclose( outFILE );
			outFILE = nullptr;

		}

	}
	catch ( ... )
	{

		// Print ERROR-message
		std::cout << "failed to decompress file#" << srcFile << ", unknown error " << std::endl;

		// Close Input FILE
		if ( inputFILE != nullptr )
		{

			std::fclose( inputFILE );
			inputFILE = nullptr;

		}

		// Close Output FILE
		if ( outFILE != nullptr )
		{

			std::fclose( outFILE );
			outFILE = nullptr;

		}

	}

}

/*
 * Compress file using zlib (not gzip).
 *
 * @param srcFile - path to a source-file to compress (deflate).
 * @param dstFile - path to compression (deflate) output-file.
 * @param pCompression - compression-level, must be in range 0-9.
 * @throws - can throw exception.
*/
void compressFile( const char *const srcFile, const char *const dstFile, const std::uint32_t & pCompression )
{

	// Input FILE
	std::FILE * inputFILE( nullptr );

	// Output FILE
	std::FILE * outFILE( nullptr );

	// FILE fopen_s errno
	errno_t errCode;

	// Guarded-Block
	try
	{

		// Open input (source) FILE
		errCode = fopen_s( &inputFILE, srcFile, "rb" );

		// Check errors
		if ( errCode != 0 || inputFILE == nullptr )
		{

			// Print ERROR-message
			std::cout << "failed to open input-file #" << srcFile << std::endl;

			// Cancel
			return;

		}

		// Open output (destination) FILE
		errCode = fopen_s( &outFILE, dstFile, "wb" );

		// Check errors
		if ( errCode != 0 || inputFILE == nullptr )
		{

			// Print ERROR-message
			std::cout << "failed to open output-file #" << dstFile << std::endl;

			// Close Input FILE
			if ( inputFILE != nullptr )
			{

				std::fclose( inputFILE );
				inputFILE = nullptr;

			}

			// Cancel
			return;

		}

		// Read, compress & write compressed data
		if ( c0de4un::ZStream::deflateFILE( inputFILE, outFILE, 16384, 1 ) != Z_OK )
			std::cout << "compression failed for file#" << srcFile << std::endl;
		else
			std::cout << "compression complete for file#" << srcFile << "; output written to " << dstFile << std::endl;

		// Close Input FILE
		std::fclose( inputFILE );
		inputFILE = nullptr;

		// Close Output FILE
		std::fclose( outFILE );
		outFILE = nullptr;

	}
	catch ( const std::exception & pException )
	{

		// Print ERROR-message
		std::cout << "failed to compres file#" << srcFile << ", error: " << pException.what( ) << std::endl;

		// Close Input FILE
		if ( inputFILE != nullptr )
		{

			std::fclose( inputFILE );
			inputFILE = nullptr;

		}

		// Close Output FILE
		if ( outFILE != nullptr )
		{

			std::fclose( outFILE );
			outFILE = nullptr;

		}

	}
	catch ( ... )
	{

		// Print ERROR-message
		std::cout << "failed to compres file#" << srcFile << ", unknown error " << std::endl;

		// Close Input FILE
		if ( inputFILE != nullptr )
		{

			std::fclose( inputFILE );
			inputFILE = nullptr;

		}

		// Close Output FILE
		if ( outFILE != nullptr )
		{

			std::fclose( outFILE );
			outFILE = nullptr;

		}

	}

}

/*
 * MAIN
 * 
 * @param argC - number of arguments.
 * @param argV - arguments, where #0 is app name.
*/
int main( int argC, char * argV[] )
{

	// Print Hello World !
	std::cout << "Hello World !" << std::endl;

	// Print Arguments
	if ( argC > 1 )
	{

		// Print DEBUG-message
		std::cout << "Input files: " << std::endl;

		// Print Ipnut-Files
		for ( int i = 1; i < argC; i++ )
		{

			std::cout << argV[i] << std::endl;

		}

	}

	// DEBUG-message
	std::cout << "compressing..." << std::endl;

	//
	char * fileName = "test_img.jpeg";

	//
	char * files[]{ fileName };

	// Compress files
	compressFile( "", "", 6 );

	// DEBUG-message
	std::cout << "compressing complete =^-^=" << std::endl;

	// DEBUG-message
	std::cout << "decompressing..." << std::endl;

	// Decompress
	decompressFile( "test_img.gzip", "decompressionResults/test_img.jpeg" );

	// DEBUG-message
	std::cout << "decompressing complete =^-^=" << std::endl;

	// Pause
	std::cin.get( );

	// Return OK
	return( 0 );

}