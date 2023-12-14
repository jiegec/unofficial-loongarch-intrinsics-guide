u64 imm12_10 = (imm >> 10) & 0b111;
u64 imm12_8 = (imm >> 8) & 0b11111;
u64 imm9_0 = imm & 0x3FF;
s64 simm9_0 = ((s64)imm9_0 << 54) >> 54;
u64 imm7_0 = imm & 0xFF;
u64 imm7 = (imm >> 7) & 0x1;
u64 imm6 = (imm >> 6) & 0x1;
u64 imm5 = (imm >> 5) & 0x1;
u64 imm5_0 = imm & 0x3F;
u64 imm4 = (imm >> 4) & 0x1;
u64 imm3 = (imm >> 3) & 0x1;
u64 imm2 = (imm >> 2) & 0x1;
u64 imm1 = (imm >> 1) & 0x1;
u64 imm0 = imm & 0x1;

u64 broadcast_value;
u64 broadcast_width;
if (imm12_10 == 0b000) {
  broadcast_value = imm7_0;
  broadcast_width = 8;
} else if (imm12_10 == 0b001) {
  broadcast_value = simm9_0;
  broadcast_width = 16;
} else if (imm12_10 == 0b010) {
  broadcast_value = simm9_0;
  broadcast_width = 32;
} else if (imm12_10 == 0b011) {
  broadcast_value = simm9_0;
  broadcast_width = 64;
} else if (imm12_8 == 0b10000) {
  broadcast_value = imm7_0;
  broadcast_width = 32;
} else if (imm12_8 == 0b10001) {
  broadcast_value = imm7_0 << 8;
  broadcast_width = 32;
} else if (imm12_8 == 0b10010) {
  broadcast_value = imm7_0 << 16;
  broadcast_width = 32;
} else if (imm12_8 == 0b10011) {
  broadcast_value = imm7_0 << 24;
  broadcast_width = 32;
} else if (imm12_8 == 0b10100) {
  broadcast_value = imm7_0;
  broadcast_width = 16;
} else if (imm12_8 == 0b10101) {
  broadcast_value = imm7_0 << 8;
  broadcast_width = 16;
} else if (imm12_8 == 0b10110) {
  broadcast_value = (imm7_0 << 8) | 0xFF;
  broadcast_width = 32;
} else if (imm12_8 == 0b10111) {
  broadcast_value = (imm7_0 << 16) | 0xFFFF;
  broadcast_width = 32;
} else if (imm12_8 == 0b11000) {
  broadcast_value = imm7_0;
  broadcast_width = 8;
} else if (imm12_8 == 0b11001) {
  broadcast_value = imm0 * 0xFF + imm1 * 0xFF00 + imm2 * 0xFF0000 +
                    imm3 * 0xFF000000 + imm4 * 0xFF00000000 +
                    imm5 * 0xFF0000000000 + imm6 * 0xFF000000000000 +
                    imm7 * 0xFF00000000000000;
  broadcast_width = 64;
} else if (imm12_8 == 0b11010) {
  broadcast_value = (imm7 << 31) | ((1 - imm6) << 30) | ((imm6 * 0x1F) << 25) |
                    (imm5_0 << 19);
  broadcast_width = 32;
} else if (imm12_8 == 0b11011) {
  broadcast_value = (imm7 << 31) | ((1 - imm6) << 30) | ((imm6 * 0x1F) << 25) |
                    (imm5_0 << 19);
  broadcast_width = 64;
} else if (imm12_8 == 0b11100) {
  broadcast_value = (imm7 << 63) | ((1 - imm6) << 62) | ((imm6 * 0xFF) << 54) |
                    (imm5_0 << 48);
  broadcast_width = 64;
}

if (broadcast_width == 8) {
  for (int i = 0; i < 32; i++) {
    dst.byte[i] = broadcast_value;
  }
} else if (broadcast_width == 16) {
  for (int i = 0; i < 16; i++) {
    dst.half[i] = broadcast_value;
  }
} else if (broadcast_width == 32) {
  for (int i = 0; i < 8; i++) {
    dst.word[i] = broadcast_value;
  }
} else if (broadcast_width == 64) {
  for (int i = 0; i < 4; i++) {
    dst.dword[i] = broadcast_value;
  }
}