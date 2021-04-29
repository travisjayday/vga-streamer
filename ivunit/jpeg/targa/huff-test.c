#include <stdlib.h>

int test_block(int32_t block[8][8]) {
    int32_t prevdc = 34;
    uint16_t* codebuf = malloc(28 * 8 * sizeof(uint16_t));
    int32_t* outb = malloc(8 * 8 * sizeof(int32_t));
    huff(prevdc, block, codebuf);
    dehuff(prevdc, codebuf, outb);
    printf("\n");
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int decoded = (int) *(outb + r * 8 + c);
            int actual = block[r][c];
            if (decoded != actual) {
                printf("Test failed!");
                printf("at %d,%d", r, c);
                exit(-1);
            }
        }
        printf("\n");
    }
}

int test_huff() {
    /* Note: AC values can be within -1023...1023 inclusive range */
    int32_t block1[8][8] = {
        57, 20, 30, 0, 0, 0, 0, 0, 
        10, -3, -120, 1, 0, 0, 0, 1,
        5, 2, 44, 0, 0, 0, 2, 0,
        1, 0, 0, 140, 0, 3, 0, 0,
        0, 0, 0, 0, -10, 0, 0, 0,
        0, 0, -12, 0, 0, 0, 6, 0,
        0, 0, 0, 0, 0, 7, 0, 0,
        8, 0, 0, -30, 40, 0, 0, 1,
    };
 
    int32_t block2[8][8] = {
        1, 20, -1, 0, 0, 0, 0, 0, 
        10, -3, 99, 1, 0, 0, 0, 1,
        5, 2, 33, 0, 0, 0, 2, 0,
        1, 0, 0, 0, 30, 3, 0, 0,
        0, 0, -31, 0, -10, 0, 0, 0,
        8, 8, -12, 0, 0, 0, 6, 100,
        0, 0, -64, 0, 0, 7, 0, 0,
        8, 0, 0, 0, 0, 0, 12, 1,
    };

    int32_t block3[8][8] = {
        1, 20, -1, 0, 0, 0, 0, 0, 
        10, -3, 99, 1, 0, 0, 0, 1,
        5, 2, 33, 0, 0, 0, 2, 0,
        1, 0, 0, 0, 30, 3, 0, 0,
        0, 0, -31, 0, -10, 0, 0, 0,
        8, 8, -12, 0, 0, 0, 6, 100,
        0, 0, -64, 0, 0, 7, 0, 0,
        8, 0, 0, 0, 0, 0, 12, 1,
    };

    int32_t block4[8][8] = {
        1, 320, -1, 0, 0, -585, 0, 0, 
        10, -3, 799, 1, 0, 0, 0, 1,
        5, 1023, 333, 0, 0, 0, 2, 0,
        1, 0, 20, 0, 30, 3, 0, 325,
        0, 0, -31, 0, -810, 358, 0, 0,
        8, 8, -12, 0, 0, 0, 6, 100,
        0, 0, -64, 0, 0, 7, 0, 0,
        8, 0, 0, 0, 0, 0, 12, 1,
    };

    int32_t block5[8][8] = {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };

    int32_t block6[8][8] = {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 922, 0, 0, 0, 0,
        0, 0, 168, 0, 0, 0, 0, 0,
        0, 723, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
    };

    int32_t block7[8][8] = {
0, 0, 0, 0, 0, 0, 74, 236, 
0, 0, 1, 939, 498, 763, 483, 326, 
0, 0, 84, 1016, 795, 488, 487, 909, 
0, 346, 302, 611, 563, 927, 201, 922, 
0, 306, 13, 951, 561, 88, 163, 346, 
293, 349, 261, 791, 88, 745, 94, 212, 
427, 178, 205, 198, 667, 692, 84, 529, 
14, 386, 116, 577, 289, 317, 476, 135, 
    };


    test_block(block1);
    test_block(block2);
    test_block(block3);
    test_block(block4);
    test_block(block5);
    test_block(block6);
    test_block(block7);

    // test 10 random fully stacked positive blocks
    for (int i = 0; i < 10; i++) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                block5[r][c] = rand() % 1024;
                printf("%d, ", block5[r][c]);
            }
            printf("\n");
        }
        test_block(block5);
    }

    // test 10 random fully stacked neg/pos blocks
    for (int i = 0; i < 10; i++) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                block5[r][c] = (rand() % 1024);
                block5[r][c] *= (rand() % 2 == 1)? -1 : 0; 
                printf("%d, ", block5[r][c]);
            }
            printf("\n");
        }
        test_block(block5);
    }

    int sparseness[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 95};
    // test 10 sparse blocks
    for (int i = 0; i < 100; i++) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (rand() % 100 < sparseness[i / 10]) {
                    block5[r][c] = (rand() % 1024);
                    block5[r][c] *= (rand() % 2 == 1)? -1 : 0; 
                }
                else {
                    block5[r][c] = 0;
                }
                printf("%d, ", block5[r][c]);
            }
            printf("\n");
        }
        test_block(block5);
    }

    printf("Test done!");
}


