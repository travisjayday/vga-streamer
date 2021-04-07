
/************************/
/*     S-Blocks 1-8     */
/************************/
/* 
 * S-Block defined as bytes. High nibble corresponds to 
 * i + 1 while low nibble corresponds to i. 
*/
uint8_t sblocks[] = {
  0x4e, 0x1d, 0xf2, 0x8b, 0xa3, 0xc6, 0x95, 0x70, 
  0x14, 0x8e, 0x6d, 0xb2, 0xcf, 0x79, 0xa3, 0x05, 
  0xf0, 0x47, 0x2e, 0x1d, 0x6a, 0xbc, 0x59, 0x83, 
  0xcf, 0x28, 0x94, 0x71, 0xb5, 0xe3, 0x0a, 0xd6, 
  0x1f, 0xe8, 0xb6, 0x43, 0x79, 0xd2, 0x0c, 0xa5, 
  0xe0, 0xb7, 0x4a, 0x1d, 0x85, 0x6c, 0x39, 0xf2, 
  0xd3, 0x74, 0x2f, 0xe8, 0x0c, 0xa1, 0x96, 0x5b, 
  0x8d, 0x1a, 0xf3, 0x24, 0x6b, 0xc7, 0x50, 0x9e, 
  0x0a, 0xe9, 0x36, 0x5f, 0xd1, 0x7c, 0x4b, 0x82, 
  0x6d, 0x94, 0xf8, 0x03, 0x1b, 0xc2, 0xa5, 0x7e, 
  0x7d, 0x90, 0x43, 0xa6, 0x82, 0xe5, 0xbc, 0x1f, 
  0xa1, 0x0d, 0x96, 0x78, 0xf4, 0x3e, 0x5b, 0xc2, 
  0xd7, 0x3e, 0x60, 0xa9, 0x21, 0x58, 0xcb, 0xf4, 
  0x6a, 0x09, 0xbc, 0xd7, 0x1f, 0xe3, 0x25, 0x48, 
  0x8d, 0x5b, 0xf6, 0x30, 0x74, 0xc2, 0xa1, 0x9e, 
  0xf3, 0x60, 0x1a, 0x8d, 0x49, 0xb5, 0x7c, 0xe2, 
  0xc2, 0x14, 0xa7, 0x6b, 0x58, 0xf3, 0x0d, 0x9e, 
  0x24, 0xb1, 0xda, 0x87, 0x9f, 0x5c, 0x36, 0xe0, 
  0xbe, 0xc2, 0x74, 0x1d, 0x05, 0xaf, 0x93, 0x68, 
  0x8b, 0x7c, 0xe1, 0xd2, 0xf6, 0x90, 0x4a, 0x35, 
  0x1c, 0xfa, 0x29, 0x86, 0xd0, 0x43, 0x7e, 0xb5, 
  0xe9, 0x5f, 0x82, 0x3c, 0x07, 0xa4, 0xd1, 0x6b, 
  0xfa, 0x24, 0xc7, 0x59, 0x16, 0xed, 0xb0, 0x83, 
  0x34, 0xc2, 0x59, 0xaf, 0xeb, 0x71, 0x06, 0xd8, 
  0xb4, 0xe2, 0x0f, 0xd8, 0xc3, 0x79, 0xa5, 0x16, 
  0x41, 0xdb, 0x3c, 0xe7, 0xfa, 0x86, 0x50, 0x29, 
  0x0d, 0x7b, 0x94, 0xa1, 0x3e, 0xc5, 0xf2, 0x68, 
  0xb6, 0x8d, 0x41, 0x7a, 0x59, 0xf0, 0x2e, 0xc3, 
  0x2d, 0x48, 0xf6, 0x1b, 0x9a, 0xe3, 0x05, 0x7c, 
  0xb7, 0x14, 0xc9, 0x2e, 0x60, 0xda, 0x3f, 0x85, 
  0xf1, 0x8d, 0x3a, 0x47, 0x5c, 0xb6, 0xe0, 0x29, 
  0x12, 0x7e, 0xa4, 0xd8, 0xcf, 0x09, 0x53, 0xb6
};

/************************/
/*  P Permutation Mask  */
/************************/
uint64_t perm_mask_p[] ={
  0x00001203, 0x00500000, 0x00000005, 0x00011000,
  0x00544551, 0x11103131, 0x07050705, 0x00b9008e,
  0x0000db54
};

/************************/
/* IP Permutation Mask  */
/************************/
uint64_t perm_mask_ip[] ={
  0x000000006aaaaaaa, 0x00003cf000003cf0,
  0x0033006600330066, 0x0606040106060401,
  0x1111000011110000, 0x4411441144114411,
  0x1203032112030321, 0x0d06090c02090603,
  0x00aa00a500aa005a, 0x0000663300006633,
  0x00000000e1f0f0f0
};

/**************************/
/* IP^-1 Permutation Mask */
/**************************/
uint64_t perm_mask_ipi[] ={
  0x0000000078f0f0f0, 0x000066cc000066cc,
  0x0055005a0055005a, 0x0406060604060606,
  0x1100000011000000, 0x4114141414414141,
  0x2121033012120330, 0x09060b0806090704,
  0x00cc009900cc0066, 0x00000fc300000fc3,
  0x00000000aaaaaa6a
};

/*************************/
/* PC-1 Permutation Mask */
/*************************/
uint64_t perm_mask_pc1[] ={
  0x0000000010101010, 0x00003898000038d8,
  0x0054005400540074, 0x0000000004000102,
  0x0000000000000000, 0x5500550055004501,
  0x3211012230112122, 0x070b0d0e0a040009,
  0x00550055005a00ca, 0x000033c600009c6c,
  0x00000000f0f0f0f0
};

/*************************/
/* PC-2 Permutation Mask */
/*************************/
uint64_t perm_mask_pc2[] ={
  0x00000000080880ef, 0x00003341000028b8,
  0x0063004600210078, 0x0505040004050404,
  0x0011001000001110, 0x0414401104404114,
  0x0332033030230300, 0x090407030d050802,
  0x00d1003900c40004, 0x0000f8ce0000b2f6,
  0x0000000075104400
};
