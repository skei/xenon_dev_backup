 #ifndef lzari_h_included
 #define lzari_h_included
 // �������������������������������������������������������������������������

 // fbuffer = compressed data
 // tbuffer = uncompressed data
 // comp_size = compressed size
void lzari_decompress(unsigned char* fbuffer, unsigned char* tbuffer, long comp_size);

 // �������������������������������������������������������������������������
 #endif