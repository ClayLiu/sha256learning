#ifndef TO_BIGEND_H_HDIFHD
#define TO_BIGEND_H_HDIFHD

#define to_bigend_32(x)             \
(                                   \
      (((x) & 0x000000ffu) << 24)   \
    | (((x) & 0x0000ff00u) << 8)    \
    | (((x) & 0x00ff0000u) >> 8)    \
    | (((x) & 0xff000000u) >> 24)   \
)

#endif