#include "WebHelper.hpp"
Unishox compressor;
String Decompress(const char * compressed, size_t uncompressed_size) {
  String content("");

  uncompressed_size += 2;    // take a security margin

  // We use a nasty trick here. To avoid allocating twice the buffer,
  // we first extend the buffer of the String object to the target size (maybe overshooting by 7 bytes)
  // then we decompress in this buffer,
  // and finally assign the raw string to the String, which happens to work: String uses memmove(), so overlapping works
  content.reserve(uncompressed_size);
  char * buffer = content.begin();

  int32_t len = compressor.unishox_decompress(compressed, strlen_P(compressed), buffer, uncompressed_size);
  if (len > 0) {
    buffer[len] = 0;    // terminate string with NULL
    content = buffer;         // copy in place
  }
  return content;
}

const char kUnescapeCode[] = "&><\"\'\\";
const char kEscapeCode[] PROGMEM = "&amp;|&gt;|&lt;|&quot;|&apos;|&#92;";