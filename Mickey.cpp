#include <stdio.h>
#include <stdint.h>

// Define the Mickey state structure
typedef struct {
    uint8_t S[256]; // State
    uint8_t K[16];  // Key
    uint8_t i, j;   // State variables
} MickeyState;

// Initialize the Mickey state with key
void mickey_init(MickeyState *state, const uint8_t *key, int key_length) {
    // Initialize the state with key
    for (int i = 0; i < 256; i++) {
        state->S[i] = i;
    }

    // Initialize variables
    state->i = 0;
    state->j = 0;

    // Permute state with key
    uint8_t j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + state->S[i] + key[i % key_length]) % 256;
        uint8_t temp = state->S[i];
        state->S[i] = state->S[j];
        state->S[j] = temp;
    }
}

// Generate the next byte of keystream
uint8_t mickey_next(MickeyState *state) {
    uint8_t i = state->i;
    uint8_t j = state->j;

    // Update state
    i += 1; // Increment i
    j += state->S[i]; // Update j

    // Swap S[i] and S[j]
    uint8_t temp = state->S[i];
    state->S[i] = state->S[j];
    state->S[j] = temp;

    // Update keystream byte
    uint8_t keystream_byte = state->S[(state->S[i] + state->S[j]) & 0xFF];

    // Update state variables
    state->i = i;
    state->j = j;

    return keystream_byte;
}

int main() {
    MickeyState state;
    uint8_t key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

    // Initialize Mickey state with key
    mickey_init(&state, key, 16);

    // Print input key
    printf("Input key:\n");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", key[i]);
    }
    printf("\n\n");

    // Print first 10 bytes of keystream
    printf("Output keystream:\n");
    for (int i = 0; i < 10; i++) {
        uint8_t keystream_byte = mickey_next(&state);
        printf("%02X ", keystream_byte);
    }
    printf("\n");

    return 0;
}
