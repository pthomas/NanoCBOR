/*
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "nanocbor/nanocbor.h"
#include "nanocbor/stream_encoders/memory_buffer.h"

static void _encode(nanocbor_encoder_t *enc)
{
    nanocbor_fmt_array_indefinite(enc);
    nanocbor_fmt_bool(enc, true);
    nanocbor_fmt_bool(enc, false);
    nanocbor_fmt_uint(enc, UINT32_MAX);
    nanocbor_fmt_int(enc, INT32_MIN);
    nanocbor_fmt_map(enc, 4);
    nanocbor_fmt_uint(enc, 8);
    nanocbor_fmt_int(enc, 30);
    nanocbor_fmt_int(enc, -30);
    nanocbor_fmt_int(enc, 500);
    nanocbor_fmt_int(enc, -500);
    nanocbor_put_tstr(enc, "this is a long string");
    nanocbor_fmt_float(enc, 0.34);
    nanocbor_put_bstr(enc, (uint8_t*)"bytez", sizeof("bytez"));
    nanocbor_fmt_null(enc);
    nanocbor_fmt_decimal_frac(enc, -2, 27315);
    nanocbor_fmt_end_indefinite(enc);
}

int main(void)
{
    memory_encoder stream;

    MemoryStream_Init(&stream, NULL, 0);

    FnStreamLength len_fn = (FnStreamLength)MemoryStream_Length;
    FnStreamReserve res_fn = (FnStreamReserve)MemoryStream_Reserve;
    FnStreamInsert ins_fn = (FnStreamInsert)MemoryStream_Insert;

    nanocbor_encoder_t enc = NANOCBOR_ENCODER(&stream, len_fn, res_fn, ins_fn);

    _encode(&enc);

    size_t required = nanocbor_encoded_len(&enc);

    uint8_t *buf = malloc(required);
    if (!buf) {
        return -1;
    }

    MemoryStream_Init(&stream, buf, required);

    _encode(&enc);

    //printf("Bytes: %u\n", (unsigned)nanocbor_encoded_len(&enc));
    fwrite(buf, 1, nanocbor_encoded_len(&enc), stdout);

    return 0;
}
