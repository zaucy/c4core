#include "c4/test.hpp"
#include "c4/to_str.hpp"
#include "c4/std/string.hpp"
#include "c4/std/vector.hpp"

namespace c4 {


template<class ItoaOrUtoa, class ItoaOrUtoaRdx, class I>
void test_prefixed_number_on_empty_buffer(ItoaOrUtoa fn, ItoaOrUtoaRdx rfn, I num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    char bufc[64];
    size_t ret;
    substr emp; // empty
    substr buf = bufc;

    auto ss2  = to_csubstr(r2);
    auto ss8  = to_csubstr(r8);
    auto ss10 = to_csubstr(r10);
    auto ss16 = to_csubstr(r16);

#define _c4clbuf() \
    memset(buf.str, 0, buf.len);\
    buf[0] = 'a';\
    buf[1] = 'a';\
    buf[2] = '\0';

    _c4clbuf();
    ret = rfn(emp, num, 2);
    EXPECT_EQ(ret, ss2.len) << "num=" << num;
    EXPECT_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 2);
    EXPECT_EQ(buf.first(ret), ss2) << "num=" << num;

    _c4clbuf();
    ret = rfn(emp, num, 8);
    EXPECT_EQ(ret, ss8.len) << "num=" << num;
    EXPECT_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 8);
    EXPECT_EQ(buf.first(ret), ss8) << "num=" << num;

    _c4clbuf();
    ret = rfn(emp, num, 10);
    EXPECT_EQ(ret, ss10.len) << "num=" << num;
    EXPECT_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 10);
    EXPECT_EQ(buf.first(ret), ss10) << "num=" << num;

    _c4clbuf();
    ret = fn(emp, num);
    EXPECT_EQ(ret, ss10.len) << "num=" << num;
    EXPECT_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = fn(buf, num);
    EXPECT_EQ(buf.first(ret), ss10) << "num=" << num;

    _c4clbuf();
    ret = rfn(emp, num, 16);
    EXPECT_EQ(ret, ss16.len) << "num=" << num;
    EXPECT_EQ(buf.first(2), "aa");
    _c4clbuf();
    ret = rfn(buf, num, 16);
    EXPECT_EQ(buf.first(ret), ss16) << "num=" << num;

#undef _c4clbuf
}

size_t call_itoa(substr s, int num)
{
    return itoa(s, num);
}
size_t call_itoa_radix(substr s, int num, int radix)
{
    return itoa(s, num, radix);
}

size_t call_utoa(substr s, unsigned num)
{
    return utoa(s, num);
}
size_t call_utoa_radix(substr s, unsigned num, unsigned radix)
{
    return utoa(s, num, radix);
}

TEST(itoa, prefixed_number_on_empty_buffer)
{
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix, 0, "0b0", "00", "0", "0x0");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix, -10, "-0b1010", "-012", "-10", "-0xa");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix,  10, "0b1010", "012", "10", "0xa");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix, -20, "-0b10100",  "-024",  "-20", "-0x14");
    test_prefixed_number_on_empty_buffer(&call_itoa, &call_itoa_radix,  20, "0b10100",  "024",  "20", "0x14");
}

TEST(utoa, prefixed_number_on_empty_buffer)
{
    test_prefixed_number_on_empty_buffer(&call_utoa, &call_utoa_radix, 0, "0b0", "00", "0", "0x0");
    test_prefixed_number_on_empty_buffer(&call_utoa, &call_utoa_radix,  10, "0b1010", "012", "10", "0xa");
    test_prefixed_number_on_empty_buffer(&call_utoa, &call_utoa_radix,  20, "0b10100",  "024",  "20", "0x14");
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


template<class ItoaOrUtoa, class ItoaOrUtoaRdx, class I>
void test_toa_radix(ItoaOrUtoa fn, ItoaOrUtoaRdx rfn, substr buf, I num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    size_t ret;

    memset(buf.str, 0, buf.len);
    ret = rfn(buf, num, 2);
    EXPECT_EQ(buf.first(ret), to_csubstr(r2)) << "num=" << num;

    memset(buf.str, 0, ret);
    ret = rfn(buf, num, 8);
    EXPECT_EQ(buf.first(ret), to_csubstr(r8)) << "num=" << num;

    memset(buf.str, 0, ret);
    ret = rfn(buf, num, 10);
    EXPECT_EQ(buf.first(ret), to_csubstr(r10)) << "num=" << num;

    memset(buf.str, 0, ret);
    ret = fn(buf, num);
    EXPECT_EQ(buf.first(ret), to_csubstr(r10)) << "num=" << num;

    memset(buf.str, 0, ret);
    ret = rfn(buf, num, 16);
    EXPECT_EQ(buf.first(ret), to_csubstr(r16)) << "num=" << num;
}

void test_utoa_radix(substr buf, unsigned num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    test_toa_radix(&call_utoa, &call_utoa_radix, buf, num, r2, r8, r10, r16);
}

void test_itoa_radix(substr buf, int num, const char *r2, const char *r8, const char *r10, const char *r16)
{
    size_t ret;

    ASSERT_GE(num, 0);
    test_toa_radix(&call_itoa, &call_itoa_radix, buf, num, r2, r8, r10, r16);

    if(num == 0) return;
    // test negative values
    num *= -1;
    char nbufc[128];
    csubstr nbuf;

#define _c4getn(which) nbufc[0] = '-'; memcpy(nbufc+1, which, strlen(which)+1); nbuf.assign(nbufc, 1 + strlen(which));

    memset(buf.str, 0, buf.len);
    _c4getn(r2);
    ret = itoa(buf, num, 2);
    EXPECT_EQ(buf.first(ret), nbuf) << "num=" << num;

    memset(buf.str, 0, ret);
    _c4getn(r8);
    ret = itoa(buf, num, 8);
    EXPECT_EQ(buf.first(ret), nbuf) << "num=" << num;

    memset(buf.str, 0, ret);
    _c4getn(r10);
    ret = itoa(buf, num, 10);
    EXPECT_EQ(buf.first(ret), nbuf) << "num=" << num;

    memset(buf.str, 0, ret);
    _c4getn(r10);
    ret = itoa(buf, num);
    EXPECT_EQ(buf.first(ret), nbuf) << "num=" << num;

    memset(buf.str, 0, ret);
    _c4getn(r16);
    ret = itoa(buf, num, 16);
    EXPECT_EQ(buf.first(ret), nbuf) << "num=" << num;
#undef _c4getn
}

TEST(itoa, radix_basic)
{
    char bufc[100] = {0};
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    test_itoa_radix(buf,   0,         "0b0",   "00",   "0",   "0x0");
    test_itoa_radix(buf,   1,         "0b1",   "01",   "1",   "0x1");
    test_itoa_radix(buf,   2,        "0b10",   "02",   "2",   "0x2");
    test_itoa_radix(buf,   3,        "0b11",   "03",   "3",   "0x3");
    test_itoa_radix(buf,   4,       "0b100",   "04",   "4",   "0x4");
    test_itoa_radix(buf,   5,       "0b101",   "05",   "5",   "0x5");
    test_itoa_radix(buf,   6,       "0b110",   "06",   "6",   "0x6");
    test_itoa_radix(buf,   7,       "0b111",   "07",   "7",   "0x7");
    test_itoa_radix(buf,   8,      "0b1000",  "010",   "8",   "0x8");
    test_itoa_radix(buf,   9,      "0b1001",  "011",   "9",   "0x9");
    test_itoa_radix(buf,  10,      "0b1010",  "012",  "10",   "0xa");
    test_itoa_radix(buf,  11,      "0b1011",  "013",  "11",   "0xb");
    test_itoa_radix(buf,  12,      "0b1100",  "014",  "12",   "0xc");
    test_itoa_radix(buf,  13,      "0b1101",  "015",  "13",   "0xd");
    test_itoa_radix(buf,  14,      "0b1110",  "016",  "14",   "0xe");
    test_itoa_radix(buf,  15,      "0b1111",  "017",  "15",   "0xf");
    test_itoa_radix(buf,  16,     "0b10000",  "020",  "16",  "0x10");
    test_itoa_radix(buf,  17,     "0b10001",  "021",  "17",  "0x11");
    test_itoa_radix(buf,  31,     "0b11111",  "037",  "31",  "0x1f");
    test_itoa_radix(buf,  32,    "0b100000",  "040",  "32",  "0x20");
    test_itoa_radix(buf,  33,    "0b100001",  "041",  "33",  "0x21");
    test_itoa_radix(buf,  63,    "0b111111",  "077",  "63",  "0x3f");
    test_itoa_radix(buf,  64,   "0b1000000", "0100",  "64",  "0x40");
    test_itoa_radix(buf,  65,   "0b1000001", "0101",  "65",  "0x41");
    test_itoa_radix(buf, 127,   "0b1111111", "0177", "127",  "0x7f");
    test_itoa_radix(buf, 128,  "0b10000000", "0200", "128",  "0x80");
    test_itoa_radix(buf, 129,  "0b10000001", "0201", "129",  "0x81");
    test_itoa_radix(buf, 255,  "0b11111111", "0377", "255",  "0xff");
    test_itoa_radix(buf, 256, "0b100000000", "0400", "256", "0x100");
}

TEST(utoa, radix_basic)
{
    char bufc[100] = {0};
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    test_utoa_radix(buf,   0,         "0b0",   "00",   "0",   "0x0");
    test_utoa_radix(buf,   1,         "0b1",   "01",   "1",   "0x1");
    test_utoa_radix(buf,   2,        "0b10",   "02",   "2",   "0x2");
    test_utoa_radix(buf,   3,        "0b11",   "03",   "3",   "0x3");
    test_utoa_radix(buf,   4,       "0b100",   "04",   "4",   "0x4");
    test_utoa_radix(buf,   5,       "0b101",   "05",   "5",   "0x5");
    test_utoa_radix(buf,   6,       "0b110",   "06",   "6",   "0x6");
    test_utoa_radix(buf,   7,       "0b111",   "07",   "7",   "0x7");
    test_utoa_radix(buf,   8,      "0b1000",  "010",   "8",   "0x8");
    test_utoa_radix(buf,   9,      "0b1001",  "011",   "9",   "0x9");
    test_utoa_radix(buf,  10,      "0b1010",  "012",  "10",   "0xa");
    test_utoa_radix(buf,  11,      "0b1011",  "013",  "11",   "0xb");
    test_utoa_radix(buf,  12,      "0b1100",  "014",  "12",   "0xc");
    test_utoa_radix(buf,  13,      "0b1101",  "015",  "13",   "0xd");
    test_utoa_radix(buf,  14,      "0b1110",  "016",  "14",   "0xe");
    test_utoa_radix(buf,  15,      "0b1111",  "017",  "15",   "0xf");
    test_utoa_radix(buf,  16,     "0b10000",  "020",  "16",  "0x10");
    test_utoa_radix(buf,  17,     "0b10001",  "021",  "17",  "0x11");
    test_utoa_radix(buf,  31,     "0b11111",  "037",  "31",  "0x1f");
    test_utoa_radix(buf,  32,    "0b100000",  "040",  "32",  "0x20");
    test_utoa_radix(buf,  33,    "0b100001",  "041",  "33",  "0x21");
    test_utoa_radix(buf,  63,    "0b111111",  "077",  "63",  "0x3f");
    test_utoa_radix(buf,  64,   "0b1000000", "0100",  "64",  "0x40");
    test_utoa_radix(buf,  65,   "0b1000001", "0101",  "65",  "0x41");
    test_utoa_radix(buf, 127,   "0b1111111", "0177", "127",  "0x7f");
    test_utoa_radix(buf, 128,  "0b10000000", "0200", "128",  "0x80");
    test_utoa_radix(buf, 129,  "0b10000001", "0201", "129",  "0x81");
    test_utoa_radix(buf, 255,  "0b11111111", "0377", "255",  "0xff");
    test_utoa_radix(buf, 256, "0b100000000", "0400", "256", "0x100");
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST(atoi, basic)
{
    char bufc[100] = {0};
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    size_t ret;

#define _woof(val) \
    ret = itoa(buf, val); EXPECT_LT(ret, buf.len); EXPECT_EQ(buf.sub(0, ret), #val)
    _woof(0);
    _woof(1);
    _woof(2);
    _woof(3);
    _woof(4);
    _woof(5);
    _woof(6);
    _woof(7);
    _woof(8);
    _woof(9);
    _woof(10);
    _woof(11);
    _woof(12);
    _woof(13);
    _woof(14);
    _woof(15);
    _woof(16);
    _woof(17);
    _woof(18);
    _woof(19);
    _woof(20);
    _woof(21);
    _woof(100);
    _woof(1000);
    _woof(11);
    _woof(101);
    _woof(1001);
    _woof(10001);
#undef _woof
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void test_ftoa(substr buf, float f, int precision, const char *scient, const char *flt, const char* flex, const char *hexa)
{
    size_t ret;

    memset(buf.str, 0, buf.len);
    ret = ftoa(buf, f, precision, FTOA_SCIENT);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(scient)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = ftoa(buf, f, precision, FTOA_FLOAT);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(flt)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = ftoa(buf, f, precision+1, FTOA_FLEX);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(flex)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = ftoa(buf, f, precision, FTOA_HEXA);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(hexa)) << "num=" << f;
}

void test_dtoa(substr buf, double f, int precision, const char *scient, const char *flt, const char* flex, const char *hexa)
{
    size_t ret;

    memset(buf.str, 0, buf.len);
    ret = dtoa(buf, f, precision, FTOA_SCIENT);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(scient)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = dtoa(buf, f, precision, FTOA_FLOAT);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(flt)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = dtoa(buf, f, precision+1, FTOA_FLEX);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(flex)) << "num=" << f;

    memset(buf.str, 0, ret);
    ret = dtoa(buf, f, precision, FTOA_HEXA);
    EXPECT_EQ(buf.left_of(ret), to_csubstr(hexa)) << "num=" << f;
}


TEST(ftoa, basic)
{
    char bufc[128];
    substr buf(bufc);
    C4_ASSERT(buf.len == sizeof(bufc)-1);

    float f = 1.1234123f;
    double d = 1.1234123f;

    {
        SCOPED_TRACE("precision 0");
        test_ftoa(buf, f, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
        test_dtoa(buf, d, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
    }

    {
        SCOPED_TRACE("precision 1");
        test_ftoa(buf, f, 1, /*scient*/"1.1e+00", /*flt*/"1.1", /*flex*/"1.1", /*hexa*/"0x1.2p+0");
        test_dtoa(buf, d, 1, /*scient*/"1.1e+00", /*flt*/"1.1", /*flex*/"1.1", /*hexa*/"0x1.2p+0");
    }

    {
        SCOPED_TRACE("precision 2");
        test_ftoa(buf, f, 2, /*scient*/"1.12e+00", /*flt*/"1.12", /*flex*/"1.12", /*hexa*/"0x1.20p+0");
        test_dtoa(buf, d, 2, /*scient*/"1.12e+00", /*flt*/"1.12", /*flex*/"1.12", /*hexa*/"0x1.20p+0");
    }

    {
        SCOPED_TRACE("precision 3");
        test_ftoa(buf, f, 3, /*scient*/"1.123e+00", /*flt*/"1.123", /*flex*/"1.123", /*hexa*/"0x1.1f9p+0");
        test_dtoa(buf, d, 3, /*scient*/"1.123e+00", /*flt*/"1.123", /*flex*/"1.123", /*hexa*/"0x1.1f9p+0");
    }

    {
        SCOPED_TRACE("precision 4");
        test_ftoa(buf, f, 4, /*scient*/"1.1234e+00", /*flt*/"1.1234", /*flex*/"1.1234", /*hexa*/"0x1.1f98p+0");
        test_dtoa(buf, d, 4, /*scient*/"1.1234e+00", /*flt*/"1.1234", /*flex*/"1.1234", /*hexa*/"0x1.1f98p+0");
    }

    f = 1.01234123f;
    d = 1.01234123;

    {
        SCOPED_TRACE("precision 0");
        test_ftoa(buf, f, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
        test_dtoa(buf, d, 0, /*scient*/"1e+00", /*flt*/"1", /*flex*/"1", /*hexa*/"0x1p+0");
    }

    {
        SCOPED_TRACE("precision 1");
        test_ftoa(buf, f, 1, /*scient*/"1.0e+00", /*flt*/"1.0", /*flex*/"1", /*hexa*/"0x1.0p+0");
        test_dtoa(buf, d, 1, /*scient*/"1.0e+00", /*flt*/"1.0", /*flex*/"1", /*hexa*/"0x1.0p+0");
    }

    {
        SCOPED_TRACE("precision 2");
        test_ftoa(buf, f, 2, /*scient*/"1.01e+00", /*flt*/"1.01", /*flex*/"1.01", /*hexa*/"0x1.03p+0");
        test_dtoa(buf, d, 2, /*scient*/"1.01e+00", /*flt*/"1.01", /*flex*/"1.01", /*hexa*/"0x1.03p+0");
    }

    {
        SCOPED_TRACE("precision 3");
#ifdef _MSC_VER
        test_ftoa(buf, f, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.032p+0");
        test_dtoa(buf, d, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.032p+0");
#else
        test_ftoa(buf, f, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0");
        test_dtoa(buf, d, 3, /*scient*/"1.012e+00", /*flt*/"1.012", /*flex*/"1.012", /*hexa*/"0x1.033p+0");
#endif
    }

    {
        SCOPED_TRACE("precision 4");
        test_ftoa(buf, f, 4, /*scient*/"1.0123e+00", /*flt*/"1.0123", /*flex*/"1.0123", /*hexa*/"0x1.0329p+0");
        test_dtoa(buf, d, 4, /*scient*/"1.0123e+00", /*flt*/"1.0123", /*flex*/"1.0123", /*hexa*/"0x1.0329p+0");
    }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST(to_str, trimmed_fit_int)
{
    // test that no characters are trimmed at the end of
    // the number due to printf-based implementations
    // needing space for the \0
    int v = 12345678;
    char buf[128];
    substr sp(buf);
    size_t sz = to_str(sp, v);
    sp = sp.left_of(sz);
    EXPECT_EQ(sp, "12345678"); // ehemm.
    char buf2[8+1];
    C4_ASSERT(sizeof(buf2) == sz+1);
    substr sp2(buf2, sizeof(buf2)); // make sure it spans the whole buffer
    sp2 = to_str_sub(sp2, v);
    EXPECT_EQ(sp2, sp); // ehemm.
    std::string str;
    catrs(&str, v);
    EXPECT_EQ(sp, to_csubstr(str)); // ehemm.
}

TEST(to_str, trimmed_fit_float)
{
    // test that no characters are trimmed at the end of
    // the number due to printf-based implementations
    // needing space for the \0
    float v = 1024.1568f;
    char buf[128];
    substr sp(buf);
    size_t sz = to_str(sp, v);
    sp = sp.left_of(sz);
    EXPECT_EQ(sp, "1024.16"); // ehemm.
    char buf2[7 + 1];
    C4_ASSERT(sizeof(buf2) == sz+1);
    substr sp2(buf2, sizeof(buf2)); // make sure it spans the whole buffer
    sp2 = to_str_sub(sp2, v);
    EXPECT_EQ(sp2, sp); // ehemm.
    std::string str;
    catrs(&str, v);
    EXPECT_EQ(sp, to_csubstr(str)); // ehemm.
}

TEST(to_str, trimmed_fit_double)
{
    // test that no characters are trimmed at the end of
    // the number due to printf-based implementations
    // needing space for the \0
    double v = 1024.1568;
    char buf[128];
    substr sp(buf);
    size_t sz = to_str(sp, v);
    sp = sp.left_of(sz);
    EXPECT_EQ(sp, "1024.16"); // ehemm.
    char buf2[7 + 1];
    C4_ASSERT(sizeof(buf2) == sz+1);
    substr sp2(buf2, sizeof(buf2)); // make sure it spans the whole buffer
    sp2 = to_str_sub(sp2, v);
    EXPECT_EQ(sp2, sp); // ehemm.
    std::string str;
    catrs(&str, v);
    EXPECT_EQ(sp, to_csubstr(str)); // ehemm.
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

template< class T >
void test_to_str_fmt_basic(T f, int precision, const char* flt, T fltv, const char *scient, T scientv)
{
    char bufc[64];
    substr buf(bufc);
    substr r;
    T copy;

    r = to_str_sub(buf, fmt(f, precision));
    EXPECT_EQ(r, to_csubstr(flt)) << "num=" << f;
    from_str(r, &copy);
    if(sizeof(T) == sizeof(float))
    {
        EXPECT_FLOAT_EQ((float)fltv, (float)copy);
    }
    else
    {
        EXPECT_DOUBLE_EQ(fltv, copy);
    }

    r = to_str_sub(buf, fmt(f, precision, FTOA_SCIENT));
    EXPECT_EQ(r, to_csubstr(scient)) << "num=" << f;
    from_str(r, &copy);
    if(sizeof(T) == sizeof(float))
    {
        EXPECT_FLOAT_EQ((float)scientv, (float)copy);
    }
    else
    {
        EXPECT_DOUBLE_EQ(scientv, copy);
    }
}

TEST(to_str, fmt_basic)
{
    char bufc[128];
    substr buf(bufc);

    size_t s = 0xff;
    EXPECT_EQ(to_str_sub(buf, fmt( int8_t(0x7f), 16)), "0x7f");
    EXPECT_EQ(to_str_sub(buf, fmt(uint8_t(0xff), 16)), "0xff");

    float f = 256.064f;
    test_to_str_fmt_basic<float>(f, 0, "256", 256.f, "3e+02", 300.f);
    test_to_str_fmt_basic<float>(f, 1, "256.1", 256.1f, "2.6e+02", 260.f);
    test_to_str_fmt_basic<float>(f, 2, "256.06", 256.06f, "2.56e+02", 256.f);
    test_to_str_fmt_basic<float>(f, 3, "256.064", 256.064f, "2.561e+02", 256.1f);
    test_to_str_fmt_basic<float>(f, 4, "256.0640", 256.0640f, "2.5606e+02", 256.06f);
    test_to_str_fmt_basic<float>(f, 5, "256.06400", 256.06400f, "2.56064e+02", 256.064f);

    double d = 256.064;
    test_to_str_fmt_basic<double>(d, 0, "256", 256., "3e+02", 300.);
    test_to_str_fmt_basic<double>(d, 1, "256.1", 256.1, "2.6e+02", 260.);
    test_to_str_fmt_basic<double>(d, 2, "256.06", 256.06, "2.56e+02", 256.);
    test_to_str_fmt_basic<double>(d, 3, "256.064", 256.064, "2.561e+02", 256.1);
    test_to_str_fmt_basic<double>(d, 4, "256.0640", 256.0640, "2.5606e+02", 256.06);
    test_to_str_fmt_basic<double>(d, 5, "256.06400", 256.06400, "2.56064e+02", 256.064);
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

TEST(cat, vars)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = cat(buf, 1, ' ', 2, ' ', 3, ' ', 4);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2 3 4");
}

#ifdef C4_TUPLE_TO_STR
TEST(cat, tuple)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = cat(buf, std::forward_as_tuple(1, ' ', 2, ' ', 3, ' ', 4));
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2 3 4");
}
#endif // C4_TUPLE_TO_STR

TEST(uncat, vars)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    sz = uncat("1 2 3 4", v1, v2, v3, v4);
    EXPECT_NE(sz, csubstr::npos);
    EXPECT_EQ(sz, 7);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}

#ifdef C4_TUPLE_TO_STR
TEST(uncat, tuple)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    auto tp = std::forward_as_tuple(v1, v2, v3, v4);
    sz = uncat("1 2 3 4", tp);
    EXPECT_NE(sz, csubstr::npos);
    EXPECT_EQ(sz, 7);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}
#endif // C4_TUPLE_TO_STR

TEST(catsep, vars)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = catsep(buf, ' ', 1, 2);
    EXPECT_EQ(sz, 3);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2");

    sz = catsep(buf, '/', 1, 2);
    EXPECT_EQ(sz, 3);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1/2");

    sz = catsep(buf, ' ', 1, 2, 3, 4);
    EXPECT_EQ(sz, 7);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2 3 4");

    sz = catsep(buf, '/', 1, 2, 3, 4);
    EXPECT_EQ(sz, 7);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1/2/3/4");
}

#ifdef C4_TUPLE_TO_STR
TEST(catsep, tuple)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = catsep(buf, ' ', std::forward_as_tuple(1, 2));
    EXPECT_EQ(sz, 3);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2");

    sz = catsep(buf, '/', std::forward_as_tuple(1, 2));
    EXPECT_EQ(sz, 3);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1/2");

    sz = catsep(buf, ' ', std::forward_as_tuple(1, 2, 3, 4));
    EXPECT_EQ(sz, 7);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 2 3 4");

    sz = catsep(buf, '/', std::forward_as_tuple(1, 2, 3, 4));
    EXPECT_EQ(sz, 7);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1/2/3/4");
}
#endif // C4_TUPLE_TO_STR

TEST(uncatsep, vars)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    char sep;

    sz = uncatsep("1 2 3 4", sep, v1, v2, v3, v4);
    EXPECT_EQ(sz, 7);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}

#ifdef C4_TUPLE_TO_STR
TEST(uncatsep, tuple)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;
    char sep;

    auto tp = std::forward_as_tuple(v1, v2, v3, v4);
    sz = uncatsep("1 2 3 4", sep, tp);
    EXPECT_EQ(sz, 7);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}
#endif // C4_TUPLE_TO_STR

TEST(format, vars)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3, 4);
    EXPECT_EQ(sz, 19);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3, 4, 5, 6, 7);
    EXPECT_EQ(sz, 19);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", 1, 2, 3);
    EXPECT_EQ(sz, 20);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and {}");

    sz = format(buf, "{} and {} and {} and {}", 1, 2);
    EXPECT_EQ(sz, 21);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and {} and {}");

    sz = format(buf, "{} and {} and {} and {}", 1);
    EXPECT_EQ(sz, 22);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and {} and {} and {}");

    sz = format(buf, "{} and {} and {} and {}");
    EXPECT_EQ(sz, 23);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "{} and {} and {} and {}");
}

#ifdef C4_TUPLE_TO_STR
TEST(format, tuple)
{
    char buf[256];
    substr sp(buf);
    csubstr result;
    size_t sz;

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2, 3, 4));
    EXPECT_EQ(sz, 19);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2, 3, 4, 5, 6, 7));
    EXPECT_EQ(sz, 19);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and 4");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2, 3));
    EXPECT_EQ(sz, 20);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and 3 and {}");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1, 2));
    EXPECT_EQ(sz, 21);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and 2 and {} and {}");

    sz = format(buf, "{} and {} and {} and {}", std::forward_as_tuple(1));
    EXPECT_EQ(sz, 22);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "1 and {} and {} and {}");

    sz = format(buf, "{} and {} and {} and {}");
    EXPECT_EQ(sz, 23);
    result = sp.left_of(sz);
    EXPECT_EQ(result, "{} and {} and {} and {}");
}
#endif // C4_TUPLE_TO_STR

TEST(unformat, vars)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    sz = unformat("1 and 2 and 3 and 4", "{} and {} and {} and {}", v1, v2, v3, v4);
    EXPECT_EQ(sz, 19);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}

#ifdef C4_TUPLE_TO_STR
TEST(unformat, tuple)
{
    size_t sz;
    int v1 = 0, v2 = 0, v3 = 0, v4 = 0;

    auto tp = std::forward_as_tuple(v1, v2, v3, v4);
    sz = unformat("1 and 2 and 3 and 4", "{} and {} and {} and {}", tp);
    EXPECT_EQ(sz, 19);
    EXPECT_EQ(v1, 1);
    EXPECT_EQ(v2, 2);
    EXPECT_EQ(v3, 3);
    EXPECT_EQ(v4, 4);
}
#endif // C4_TUPLE_TO_STR


TEST(printf, basic)
{
    char bufc[128];
    substr buf = bufc;
    size_t ret;

    memset(buf.str, 0, buf.len);
    ret = sprintf(buf, "hello world");
    EXPECT_EQ(buf.first(ret), "hello world");

    memset(buf.str, 0, buf.len);
    ret = sprintf(buf, "%d %d %d %d", 1, 2, 3, 4);
    EXPECT_EQ(buf.first(ret), "1 2 3 4");
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
TEST(catrs, basic)
{
    std::vector<char> buf;

    catrs(&buf, 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1234");
    catrs(&buf, 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "5678");
}

TEST(catrs, basic_append)
{
    std::vector<char> buf;

    catrs(append, &buf, 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1234");
    catrs(append, &buf, 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "12345678");
    catrs(append, &buf, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "123456789012345678");
}

TEST(catseprs, basic)
{
    std::vector<char> buf;

    catseprs(&buf, ' ', 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1 2 3 4");
    catseprs(&buf, ' ', 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "5 6 7 8");

    catseprs(&buf, ',', 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1,2,3,4");
    catseprs(&buf, ',', 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "5,6,7,8");

    catseprs(&buf, '/', 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1/2/3/4");
    catseprs(&buf, '/', 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "5/6/7/8");

    catseprs(&buf, "///", 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1///2///3///4");
    catseprs(&buf, "///", 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "5///6///7///8");
}

TEST(catseprs, basic_append)
{
    std::vector<char> buf;

    catseprs(append, &buf, ' ', 1, 2, 3, 4);
    EXPECT_EQ(to_csubstr(buf), "1 2 3 4");
    catseprs(append, &buf, ' ', 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "1 2 3 45 6 7 8");
    catseprs(append, &buf, ' ', 9, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    EXPECT_EQ(to_csubstr(buf), "1 2 3 45 6 7 89 0 1 2 3 4 5 6 7 8");
}

TEST(formatrs, basic)
{
    std::vector<char> buf;

    formatrs(&buf, "{} goes with food, {} goes with heat, {} anytime", "wine", "beer", "coffee");
    EXPECT_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat, coffee anytime");
}

TEST(formatrs, basic_append)
{
    std::vector<char> buf;

    formatrs(append, &buf, "{} goes with food", "wine");
    EXPECT_EQ(to_csubstr(buf), "wine goes with food");
    formatrs(append, &buf, ", {} goes with heat", "beer");
    EXPECT_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat");
    formatrs(append, &buf, ", {} anytime", "coffee");
    EXPECT_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat, coffee anytime");

    formatrs(append, &buf, ". And water. {} glass of {}cl in the morning clears you up for the day", 1, 40);
    EXPECT_EQ(to_csubstr(buf), "wine goes with food, beer goes with heat, coffee anytime. And water. 1 glass of 40cl in the morning clears you up for the day");
}

} // namespace c4
