#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "test.h"
#include "lib/mylib/mylib.h"
#include "lib/mylib2/mylib2.h"
#include "lib/legal/legal.h"
#include "modules/oi/m_oi.h"

#include "mbedtls/sha256.h"

void ascii_to_hex(const unsigned char *ascii_array, int ascii_length, char *hex_array)
{
    for (int i = 0, j = 0; i < ascii_length; ++i, j += 2)
    {
        sprintf(hex_array + j, "%02X", ascii_array[i]);
    }
}

void print_hash(unsigned char hash[32])
{
    for (int i = 0; i < 32; i++)
    {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int main()
{
    printf("AAA RENAN SOARES!!!\n");
    int a = 5, b = 3;
    printf("Add: %d\n", add(a, b));
    printf("Subtract: %d\n", subtract(a, b));
    printf("Multiply: %d\n", multiply(a, b));
    printf("Divide: %d\n", divide(a, b));

    modulo_oi();
    teste();
    testesMyLIB();
    auto_lib();

    const char *input = "Hello, world!";
    unsigned char hash[32];

    // Inicializa o contexto SHA-256
    mbedtls_sha256_context ctx;
    mbedtls_sha256_init(&ctx);

    // Inicia o processo de hashing
    mbedtls_sha256_starts(&ctx, 0); // 0 para SHA-256, 1 para SHA-224
    mbedtls_sha256_update(&ctx, (unsigned char *)input, strlen(input));
    mbedtls_sha256_finish(&ctx, hash);

    // Limpa o contexto
    mbedtls_sha256_free(&ctx);

    // Imprime o hash
    printf("SHA-256 hash: ");
    print_hash(hash);

    InitWindow(800, 450, "raylib [core] example - basic window");

    char hashHex[65] = {0};
    ascii_to_hex(hash, sizeof(hash), hashHex);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(hashHex, 0, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
