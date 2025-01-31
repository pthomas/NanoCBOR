/*
 * SPDX-License-Identifier: CC0-1.0
 */

#include "test.h"
#include "nanocbor/nanocbor.h"
#include "nanocbor/stream_encoders/memory_buffer.h"
#include <math.h>
#include <float.h>
#include <CUnit/CUnit.h>

static void print_bytestr(const uint8_t *bytes, size_t len)
{
    printf("\n");
    for(unsigned int idx=0; idx < len; idx++)
    {
        printf("%02X", bytes[idx]);
    }
    printf("\n");
}

static void test_encode_float_specials(void)
{
    uint8_t buf[64];
    memory_encoder stream;
    MemoryStream_Init(&stream, buf, sizeof(buf));

    FnStreamLength len_fn = (FnStreamLength)MemoryStream_Length;
    FnStreamReserve res_fn = (FnStreamReserve)MemoryStream_Reserve;
    FnStreamInsert ins_fn = (FnStreamInsert)MemoryStream_Insert;

    nanocbor_encoder_t enc = NANOCBOR_ENCODER(&stream, len_fn, res_fn, ins_fn);

    nanocbor_fmt_array_indefinite(&enc);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, NAN), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, -NAN), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, INFINITY), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, -INFINITY), 3);

    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, NAN), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, -NAN), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, INFINITY), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, -INFINITY), 3);
    nanocbor_fmt_end_indefinite(&enc);
    print_bytestr(buf, nanocbor_encoded_len(&enc));
}

static void test_encode_float_to_half(void)
{
    uint8_t buf[64];
    memory_encoder stream;
    MemoryStream_Init(&stream, buf, sizeof(buf));

    FnStreamLength len_fn = (FnStreamLength)MemoryStream_Length;
    FnStreamReserve res_fn = (FnStreamReserve)MemoryStream_Reserve;
    FnStreamInsert ins_fn = (FnStreamInsert)MemoryStream_Insert;

    nanocbor_encoder_t enc = NANOCBOR_ENCODER(&stream, len_fn, res_fn, ins_fn);

    nanocbor_fmt_array_indefinite(&enc);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, 1.75), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, 1.9990234375), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, 1.99951171875), 5);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, 2.0009765625), 5);

    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, -1.75), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, -1.9990234375), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, -1.99951171875), 5);
    CU_ASSERT_EQUAL(nanocbor_fmt_float(&enc, -2.0009765625), 5);

    nanocbor_fmt_end_indefinite(&enc);
    print_bytestr(buf, nanocbor_encoded_len(&enc));
}

static void test_encode_double_to_float(void)
{
    uint8_t buf[128];
    memory_encoder stream;
    MemoryStream_Init(&stream, buf, sizeof(buf));

    FnStreamLength len_fn = (FnStreamLength)MemoryStream_Length;
    FnStreamReserve res_fn = (FnStreamReserve)MemoryStream_Reserve;
    FnStreamInsert ins_fn = (FnStreamInsert)MemoryStream_Insert;

    nanocbor_encoder_t enc = NANOCBOR_ENCODER(&stream, len_fn, res_fn, ins_fn);

    nanocbor_fmt_array_indefinite(&enc);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, 1.75), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, 1.9990234375), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, 1.99951171875), 5);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, 2.0009765625), 5);

    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, -1.75), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, -1.9990234375), 3);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, -1.99951171875), 5);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, -2.0009765625), 5);

    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, 1.00000011920928955078125), 5);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, DBL_MIN), 9);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, DBL_MAX), 9);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, 1e39), 9);

    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, FLT_MIN), 5);
    CU_ASSERT_EQUAL(nanocbor_fmt_double(&enc, FLT_MAX), 5);

    nanocbor_fmt_end_indefinite(&enc);
    print_bytestr(buf, nanocbor_encoded_len(&enc));
}

const test_t tests_encoder[] = {
    {
        .f = test_encode_float_specials,
        .n = "Float reduction encoder test",
    },
    {
        .f = test_encode_float_to_half,
        .n = "Float reduction encoder test",
    },
    {
        .f = test_encode_double_to_float,
        .n = "Double reduction encoder test",
    },
    {
        .f = NULL,
        .n = NULL,
    }
};
