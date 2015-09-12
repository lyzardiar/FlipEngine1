#pragma once
#include <stdio.h> // required for jpeglib.h
extern "C"
{
#include "jpeglib/jpeglib.h" 
}
#include <setjmp.h>

class ImageLoaderJPG
{
public:
	ImageLoaderJPG();
	~ImageLoaderJPG();

	void loadImage(const char* pszFileName);

	static void error_exit (j_common_ptr cinfo);

	/* output error messages via Irrlicht logger. */
	static void output_message(j_common_ptr cinfo);

	/*	Initialize source.  This is called by jpeg_read_header() before any
	data is actually read.  Unlike init_destination(), it may leave
	bytes_in_buffer set to 0 (in which case a fill_input_buffer() call
	will occur immediately). */
	static void init_source (j_decompress_ptr cinfo);

	/*	This is called whenever bytes_in_buffer has reached zero and more
	data is wanted.  In typical applications, it should read fresh data
	into the buffer (ignoring the current state of next_input_byte and
	bytes_in_buffer), reset the pointer & count to the start of the
	buffer, and return TRUE indicating that the buffer has been reloaded.
	It is not necessary to fill the buffer entirely, only to obtain at
	least one more byte.  bytes_in_buffer MUST be set to a positive value
	if TRUE is returned.  A FALSE return should only be used when I/O
	suspension is desired (this mode is discussed in the next section). */
	static boolean fill_input_buffer (j_decompress_ptr cinfo);

	/* Skip num_bytes worth of data.  The buffer pointer and count should
	be advanced over num_bytes input bytes, refilling the buffer as
	needed.  This is used to skip over a potentially large amount of
	uninteresting data (such as an APPn marker).  In some applications
	it may be possible to optimize away the reading of the skipped data,
	but it's not clear that being smart is worth much trouble; large
	skips are uncommon.  bytes_in_buffer may be zero on return.
	A zero or negative skip count should be treated as a no-op. */
	static void skip_input_data (j_decompress_ptr cinfo, long num_bytes);

	/* Terminate source --- called by jpeg_finish_decompress() after all
	data has been read.  Often a no-op. */
	static void term_source (j_decompress_ptr cinfo);

public:
	int				m_nImageWidth;
	int				m_nImageHeight;
	unsigned char*	m_pImageData;

};

