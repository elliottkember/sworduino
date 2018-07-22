// bool leftRight = true;
// void police() {
//   EVERY_N_MILLISECONDS(400) {
//     leftRight = !leftRight;
//   }
//   uint8_t red = 0;
//   uint8_t blue = 170;
//   for (int y = 0; y < 12; y++) {
//     for (int x = 0; x < 72; x++) {
//       uint16_t index = y * 72 + x;
//       if (x < 36) {
//         // side
//         Soulmate::led_arr[index] = CHSV(leftRight ? blue : red, 255, 255);
//       } else {
//         // side
//         Soulmate::led_arr[index] = CHSV(leftRight ? red : blue, 255, 255);
//       }
//     }
//   }
// }
