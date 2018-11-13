/** \file
 ******************************************************************************
 *  MCX System Library.
 *  This library will give support for basic system functions and objects.
 *
 *  \par Company:
 *  Danfoss
 */
/*
 *  \author    Sebastiano Fiorindo
 *
 *  \par CVS:
 *  \verbatim
 @(#) $Author: DANFOSS\U240254 $
 @(#) $Date: 2013/01/25 11:48:14 $
 @(#) $Source: /CVS/NECOS/swa/nvmlib/sysdef.c,v $\endverbatim
 ******************************************************************************
*/
/*
    Optimized operators: !=, <, <=
*/

#define char   signed_char          /* [internal] */
#define short  signed_short         /* [internal] */
#define int    signed_int           /* [internal] */
#define long   signed_long          /* [internal] */
#define byte   unsigned_char        /* [internal] */
#define word   unsigned_short       /* [internal] */

#define void         object         /* [internal] */
#define signed_int   signed_short   /* [internal] */
#define unsigned_int unsigned_short /* [internal] */


#define Sleep0  SysCall(SysFn_sysIdle)  /* [internal] wait zero milliseconds (switch task) */


#define SysFn_sysOutputDebugString  0x1000  /* [internal] */
#define SysFn_sysIdle               0x1001  /* [internal] */
#define SysFn_sysSetWatchdog        0x1002  /* [internal] */
#define SysFn_sysSuspend            0x1003  /* [internal] release control to other tasks for a specified number of milliseconds */
#define SysFn_sysCreateThread       0x1004  /* [internal] create a new thread */
#define SysFn_sysSetEvent           0x1005  /* [internal] get or set event flags */
#define SysFn_sysNewMainLoop        0x1006  /* called to confirm that an application main loop have completed. Mainly used in profiling application. */

#define SysFn_sysCRC16              0x1011  /* [internal] */
#define SysFn_sysGetInfo            0x1012  /* [internal] */
#define SysFn_sysSetInfo            0x1013  /* [internal] */
#define SysFn_sysMemMove            0x1014  /* 14/11/12: preliminary, to test performance */
#define SysFn_sysGetHWInfo          0x1020  /* [internal] */
#define SysFn_sysSetParam           0x1082  /* [internal] get or set system parameters */

#define SysFn_TString_ReadStringTable   0x1501  /* [internal] */


#define sysParam_SYS            0x01    /* [internal] system parameters group */
#define sysParam_CAN            0x02    /* [internal] CANopen parameters group */
#define sysParam_LCD            0x03    /* [internal] LCD parameters group */
#define sysParam_RTC            0x04    /* [internal] RTC parameters group */
#define sysParam_COM            0x05    /* [internal] COM parameters group */

#define sysParam_MaxLoopTime    0x01000011  /* [internal] */
#define sysParam_CANNodeID      0x02000021  /* [internal] */
#define sysParam_CANBaud        0x02000022  /* [internal] */
#define sysParam_UITarget       0x02000023  /* [internal] */
#define sysParam_Volume         0x03000031  /* [internal] */
#define sysParam_Contrast       0x03000032  /* [internal] */
#define sysParam_Brightness     0x03000033  /* [internal] */
#define sysParam_Negative       0x03000034  /* [internal] */
#define sysParam_rtcTime        0x04000041  /* [internal] */
#define sysParam_COMNodeID      0x05000051  /* [internal] */
#define sysParam_COMBaud        0x05000052  /* [internal] */



struct object  {
    inline byte& GetByteRef(signed_long i)  {
        asm_40;
    };

    void set(object& x)  {
        // must be implemented for each object
        // default implementation will just copy all bytes from "x" to "this"
        // the problem is to get here the sizeof the final object (the inherited class)
        //long i = 0;
        //for(;i<sizeof(x);i++)
        //    GetByteRef(i) = x.GetByteRef(i);
    };

};


// the unsigned must not be a #define, because the first letter is the sign
struct unsigned_char: public signed_char  {
    // duplicate !
    override signed_long shr(signed_long n)  {
        RetVal = Me.shl(0-n);
    };

    // duplicated, because RetVal = Me is different for signed and unsigned Me
    override signed_long PostIncr()  {
        RetVal = Me;
        Me += 1;
    };

    override signed_long PreIncr()  {
        Me += 1;
        RetVal = Me;
    };

    override signed_long PostDecr()  {
        RetVal = Me;
        Me += -1;
    };

    override signed_long PreDecr()  {
        Me += -1;
        RetVal = Me;
    };

};

struct unsigned_short: public signed_short  {
    // duplicate !
    override signed_long shr(signed_long n)  {
        RetVal = Me.shl(0-n);
    };

    // duplicated, because RetVal = Me is different for signed and unsigned Me
    override signed_long PostIncr()  {
        RetVal = Me;
        Me += 1;
    };

    override signed_long PreIncr()  {
        Me += 1;
        RetVal = Me;
    };

    override signed_long PostDecr()  {
        RetVal = Me;
        Me += -1;
    };

    override signed_long PreDecr()  {
        Me += -1;
        RetVal = Me;
    };

};

//struct unsigned_long: public signed_long{};    // UNSUPPORTED !
/*
    NOTE: band, bor, bxor, bnot are bit level operations (i.e. 0xFE.band(0x03) == 0x02)
          and, or, not are logic level operations (i.e. 0xFE.and(0x02) == 0x01)

*/

struct signed_char: public object  {
    signed_char ll_primitivebyte;

    override inline void set(signed_long n)  {
        asm_1_sizeof_Me_BitSize;
    };

    // not, !, reverse logic (i.e.: 0 -> 1, !=0 -> 0)
    inline signed_long not()  {
        asm_R2V_sizeof_Me;
        asm_20;
    };

    // tilde, ~, bit-not (reverse every bit)
    inline signed_long bnot()  {
        asm_R2V_sizeof_Me;
        asm_push32_4294967295;
        asm_23;
        //RetVal = Me.bxor(0xFFFFFFFF);
    };

    inline signed_long band(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_21;
    };

    inline signed_long bor(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_22;
    };

    inline signed_long bxor(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_23;
    };

    inline signed_long and(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_21;
    };

    inline signed_long or(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_22;
    };

    inline signed_long shl(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_24;
    };

    signed_long shr(signed_long n)  {
        RetVal = Me.shl(0-n);
    };

    inline signed_long sum(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_10;
    };

    inline signed_long mul(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_11;
    };

    inline signed_long sub(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_12;
    };

    inline signed_long div(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_13;
    };

    inline signed_long mod(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_14;
    };

    inline signed_long LessThan(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_30;
    };

    inline signed_long LessOrEqual(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_31;
    };

    inline signed_long NotEqual(signed_long n)  {
        asm_R2V_sizeof_Me;
        //asm_12; // sub
        asm_32;     // can't use sub, because I need a boolean result (all bits set or clear) for later processing (e.g. band, bor, ...)
    };

    inline signed_long Equal(signed_long n)  {
        // could be sub + not, after not, the result is boolean (all bits set or clear)
        asm_R2V_sizeof_Me;
        asm_32;
        asm_dup;
        asm_20;
    };

    inline signed_long GreaterOrEqual(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_30;
        asm_dup;
        asm_20;
    };

    inline signed_long GreaterThan(signed_long n)  {
        asm_R2V_sizeof_Me;
        asm_31;
        asm_dup;
        asm_20;
    };

    inline void sum_set(signed_long n)  {
        asm_39;
        asm_0_sizeof_Me_BitSize;
    };

    void sub_set(signed_long n)  {
        Me.sum_set(0-n);
    };

    signed_long PostIncr()  {
        RetVal = Me;
        Me += 1;
    };

    signed_long PreIncr()  {
        Me += 1;
        RetVal = Me;
    };

    signed_long PostDecr()  {
        RetVal = Me;
        Me += -1;
    };

    signed_long PreDecr()  {
        Me += -1;
        RetVal = Me;
    };


    /*
    override bit& Bit(signed_char n)  {
        RetVal.GetByteRef(4) = n % 8;
        if (n < 0 || n >= sizeof(Me)*8)  {
            char a[1];
            a[n*0+1] = 0;     // will generate an index out of range
        };
        SetReference(RetVal,Me.GetByteRef(sizeof(Me)-n/8-1));
    };
    */


    inline bit& Bit(signed_long n)  {
        asm_rbit;
    };


    // new bit function (identical for all basic types)
    // this function is faster than Bit, and will keep the same bit order (bit0 on long variables is bit0 of the least significant byte, i.e. the rightmost bit)
    inline bit& RBit(signed_long n)  {
        asm_rbit;
    };

    // new bit function (identical for all basic types)
    // this function is much faster than Bit, but it reverse order of bits (bit0 on long variables is bit0 of the most significant byte, i.e. the leftmost bit)
    // LBit index XOR 24 == RBit index (on PC/little-endian), LBit index XOR 23 == RBit index (on MCX/big-endian),
    inline bit& LBit(signed_long n)  {
        asm_lbit;
    };


};

struct signed_short: public signed_char  {
    signed_char b1;

    // duplicate !
    override signed_long shr(signed_long n)  {
        RetVal = Me.shl(0-n);
    };

    override void sub_set(signed_long n)  {
        Me.sum_set(0-n);
    };

    override signed_long PostIncr()  {
        RetVal = Me;
        Me += 1;
    };

    override signed_long PreIncr()  {
        Me += 1;
        RetVal = Me;
    };

    override signed_long PostDecr()  {
        RetVal = Me;
        Me += -1;
    };

    override signed_long PreDecr()  {
        Me += -1;
        RetVal = Me;
    };

};

struct signed_long: public signed_char  {
    signed_char b1;
    signed_char b2;
    signed_char b3;

    // duplicate !
    override signed_long shr(signed_long n)  {
        RetVal = Me.shl(0-n);
    };

    override void sub_set(signed_long n)  {
        Me.sum_set(0-n);
    };

    override signed_long PostIncr()  {
        RetVal = Me;
        Me += 1;
    };

    override signed_long PreIncr()  {
        Me += 1;
        RetVal = Me;
    };

    override signed_long PostDecr()  {
        RetVal = Me;
        Me += -1;
    };

    override signed_long PreDecr()  {
        Me += -1;
        RetVal = Me;
    };

};


/**
    Divide two integers and round up the result.
    @param a is the first operand. If a is higher than +214748364, or lower than -214748364, a standard division (with no roundup) will be performed.
    @param b is the second operand.

    example:

    @code
        d = 159 / 7;            // d will be 22
        d = RoundDiv(159, 7);   // d will be 23
        d = -159 / 7;           // d will be -22
        d = RoundDiv(-159, 7);  // d will be -23
    @endcode
*/
long RoundDiv(long a, long b)  {
    // if first value is too big, we just divide the values
    if (a >= 0)  {
        if (a <= 0x7FFFFFFF / 10)
            RetVal = ((a * 10 / b) + 5) / 10;
        else
            RetVal = a / b;
    }
    else  {
        if (a >= -214748364)
            RetVal = ((a * 10 / b) - 5) / 10;
        else
            RetVal = a / b;
    };

    /*
        test app:
            OutputDebugNum( "159 / 7", 159 / 7);
            OutputDebugNum( "RoundDiv(159,7)", RoundDiv(159,7));
            OutputDebugNum( "-159 / 7", -159 / 7);
            OutputDebugNum( "RoundDiv(-159,7)", RoundDiv(-159,7));

            OutputDebugNum( "RoundDiv(214748364,5)", RoundDiv(214748364,5));
            OutputDebugNum( "RoundDiv(214748365,2)", RoundDiv(214748365,2));

            OutputDebugNum( "RoundDiv(-214748364,5)", RoundDiv(-214748364,5));
            OutputDebugNum( "RoundDiv(-214748363,2)", RoundDiv(-214748363,2));
            OutputDebugNum( "RoundDiv(-214748365,2)", RoundDiv(-214748365,2));
    */
};


/**
 * Circular rotation of bits to the left for 8-bit values
 *
 *\param value  value to be rotated
 *\param shift  bit shift
 *
 *\return rotated value
 */
unsigned char rotl8(unsigned char value, long shift)
{
    shift = shift & (sizeof(value)*8 - 1);
    if (shift == 0)
      return value;
    return ((value << shift) | (value >> (sizeof(value)*8 - shift)));
}

/**
 * Circular rotation of bits to the left for 16-bit values
 *
 *\param value  value to be rotated
 *\param shift  bit shift
 *
 *\return rotated value
 */
unsigned short rotl16(unsigned short value, long shift)
{
    shift = shift & (sizeof(value)*8 - 1);
    if (shift == 0)
      return value;
    return ((value << shift) | (value >> (sizeof(value)*8 - shift)));
}

/**
 * Circular rotation of bits to the left for 32-bit values
 *
 *\param value  value to be rotated
 *\param shift  bit shift
 *
 *\return rotated value
 */
long rotl32(long value, long shift)
{
    shift = shift & (sizeof(value)*8 - 1);
    if (shift == 0)
      return value;
    return ((value << shift) | (value >> (sizeof(value)*8 - shift)));
}


/**
 * Circular rotation of bits to the right for 8-bit values
 *
 *\param value  value to be rotated
 *\param shift  bit shift
 *
 *\return rotated value
 */
unsigned char rotr8(unsigned char value, long shift)
{
    shift = shift & (sizeof(value)*8 - 1);
    if (shift == 0)
      return value;
    return ((value >> shift) | (value << (sizeof(value)*8 - shift)));
}

/**
 * Circular rotation of bits to the right for 16-bit values
 *
 *\param value  value to be rotated
 *\param shift  bit shift
 *
 *\return rotated value
 */
unsigned short rotr16(unsigned short value, long shift)
{
    shift = shift & (sizeof(value)*8 - 1);
    if (shift == 0)
      return value;
    return ((value >> shift) | (value << (sizeof(value)*8 - shift)));
}

/**
 * Circular rotation of bits to the right for 32-bit values
 *
 *\param value  value to be rotated
 *\param shift  bit shift
 *
 *\return rotated value
 */
long rotr32(long value, long shift)
{
    shift = shift & (sizeof(value)*8 - 1);
    if (shift == 0)
      return value;
    return ((value >> shift) | (value << (sizeof(value)*8 - shift)));
}



struct Pointer: public object  {
    signed_long ptr;
};

struct TBitPointer: public object  {
    signed_long ptr;
    char        bitoffset;
};

struct bit: public signed_char  {
//struct bit: public signed_long  {
    bit ll_primitivebit;
};

struct ArrayBasicType: public object  {
};

struct Array: public object  {
    void            data;       // start of array data

    inline ArrayBasicType& geti(signed_long n)  {
        asm_geti;
    };

};


/** String support.
    One string will keep a text of up to 200 characters.

    @par To declare a string use the following syntax:
    @code
        String s;           // will create a string of 200 maximum characters
        String small(10);   // will create a string of 10 maximum characters
    @endcode

    @par On string objects you can use some simple operators:
    @code
        s = "test";         // will set string s to text "test"
        s += "xx";          // append the text "xx" at the end of the string s
        if (s == "test")    // this will check if value of string s is equal to string "test"
    @endcode

    @par To replace a character into a string you can do like this:
    @code
        s = "X-X";          // this will set string length to 3 characters, s is now "X-X"
        s[0] = 65+1;        // set 1st character to ascii code 66 (character 'B'), s is now "B-X"
        s[2] = 'A';         // set 3rd character to ascii code 65 (character 'A'), s is now "B-A"
    @endcode

   @warning The String object is not the same as the standard C variable char s[].
   Main difference are that when you assign a string object (s = "test") you actualy copy the text "test" into the string s, replacing any other contained text.
   Another difference is that when you make string comparison (s == "test") the comparison is made on the content, not on the pointer.

*/
struct String: public object  {
    byte Len;                   ///< Length of the string.
    unsigned_short  MaxItems;   ///< Maximum length of the string.
    byte Txt[200];

    /// @return 1 if string is empty, 0 if string is not empty
    signed_long isEmpty()  {
        RetVal = (Len == 0);
    };

    byte& geti(signed_long n)  {
        if (n >= MaxItems)
            SetReference(RetVal,Txt[sizeof(Txt)+n]);    // this will generate an array index error
        else
            SetReference(RetVal,Txt[n]);
    };

    /// Clear the string.
    /// Call this function to remove all characters from the string. Same as s = "".
    void Clear()  {
        Len = 0;
    };

    override void set(String& cSource)  {
        Len = 0;
        Me.AddSubStr(cSource, 0, cSource.Len);
    };

    void sum_set(String& cSource)  {
        AddSubStr(cSource,0,cSource.Len);
    };

    String SubStr(long iStart, long iLength)  {
        RetVal.Len = 0;
        RetVal.AddSubStr(Me,iStart,iLength);
    };

    /** Add a substring from another string.
        @param cSource is the source string, where to get characters
        @param iStart is the start index into cSource (first character have index 0)
        @param iLength is the number of characters to copy

        This function will add, to the subject string, iLength characters, reading them from the string cSource, starting from index iStart.

    */
    void AddSubStr(String& cSource, long iStart, long iLength)  {
        while (iStart < cSource.Len)  {
            if (Len >= MaxItems || iLength==0)
                break;
            Me[Len++] = cSource[iStart++];
            iLength--;
        };
    };


    /**
        Add number in text format to the string.

        @param n is the number.
        @param lpad is the number of digit (including decimal dot and decimal digits, if iDec > 0)
        @param iDec is the number of decimal digits (not including dot)
        @param sgn can be +1 to display plus sign (otherwise + sign will not be shown)

        e.g.:
        @code
            String s;

            ...

            s.AddFormatNum(120,6,1);

            // s will be "  12.0"
        @endcode
    */
    void AddFormatNum(long n, char lpad, char iDec = 0, char sgn = 0)  {
        char start;

        if (n < 0)  {
            sgn = -1;
            n = abs(n);
        };

        if (iDec <= 0)
            iDec = -1;

        long plen;
        plen = Len;
        Len = Len + lpad;
        if (Len > MaxItems)
            Len = MaxItems;
        start = Len - 1;
        while (start >= plen)  {
            if (iDec == 0)  {
                Me[start] = '.';
            }
            else if (n != 0 || iDec > 0)  {
                Me[start] = '0' + (n % 10);
                n = n / 10;
            }
            else if (iDec == -1)  {
                Me[start] = '0';
            }
            else if (sgn)  {
                if (sgn < 0)
                    Me[start] = '-';
                else
                    Me[start] = '+';
                sgn = 0;
            }
            else
                Me[start] = ' ';
            iDec = iDec - 1;        // piu' efficenti dell'autodecremento
            start = start - 1;
        };

    };

    /**
        Add number in hexadecimal format to the string.

        @param iNum is the number.
        @param iDig is the number of digit.

        e.g.:
        @code
            String s;

            ...

            s.AddFormatNumHex(120,4);

            // s will be "0078"
        @endcode
    */
    void AddFormatNumHex(long iNum, char iDig = 8)  {
        AddFormatNumBase(iNum, iDig, '0', 16);
    };

    /**
        Add number in specified base format to the string.

        @param iNum is the number.
        @param iDig is the number of digit.
        @param cPad is the padding character (e.g. '0').
        @param iBase is the required base (e.g.: 2=binary, 8=octal, 10=decimal, 16=hexadecimal, and so on)

        e.g.:
        @code
            String s;

            ...

            s.AddFormatNumBase(120,4,'0',16);

            // s will be "0078"
        @endcode
    */
    void AddFormatNumBase(long iNum, char iDig, char cPad, char iBase)  {
        long i;
        long stop;
        char d;
        bit Neg;
        String pad(1);

        // set new length
        if (Len + iDig > MaxItems)
            iDig = MaxItems - Len;

        stop = Len;
        Len += iDig;
        i = Len();

        Neg = iNum.LBit(7);
        iNum = iNum & 0x7FFFFFFF;   // remove sign

        while (iNum != 0 && i > stop)  {
            d = iNum % iBase;
            if (d < 10)
                d += '0';
            else
                d += 'A' - 10;
            Me[--i] = d;
            iNum = iNum / iBase;
            if (Neg)  {
                Neg = 0;
                // patch for negative numbers: base 2 -> 6, base 16 -> 15
                if (iBase == 2)
                    iNum = iNum | 0x40000000;
                else if (iBase == 16)
                    iNum = iNum | 0x08000000;
            }
        };

        // fill with padding char
        while (i > stop)  {
            Me[--i] = cPad;
        };

    };

    /// OBSOLETE: Use AddFormatNum().
    obsolete void AddFormat(String& format, long n)  {
        long i;
        long j;
        long sz;
        String s(12);

        // calculate correct number size
        for(i=0; i<format.Len; i++)  {
            if (format[i] == '#' || format[i] == '0')
                sz++;
        };
        s.AddFormatNum(n,ToChar(sz));

        // add number to output string
        for(i=0; i<format.Len; i++)  {
            if (format[i] == '#')
                AddSubStr(s,j++,1);
            else if (format[i] == '0')  {
                if (s[j] == ' ')
                    sum_set("0");
                else
                    AddSubStr(s,j,1);
                j++;
            }
            else
                AddSubStr(format,i,1);
        };
    };

    /// OBSOLETE: Use AddFormatNum().
    obsolete void Format(String& format, long n)  {
        Me = "";
        AddFormat(format,n);
    };

    /**
        Translate text inside string into a number.
        @param iStart is the starting point. Must be a reference.
        @param iBase is the required base (e.g.: 2=binary, 8=octal, 10=decimal, 16=hexadecimal, and so on)
        @return the number.
    */
    long ToIntEx(long& iStart, long iBase = 10)  {
        long i;
        char c;

        i = iStart;
        while (i < Len)  {
            c = Me[i];

            if (c >= 'a' && c <= 'f')
                c = c - 32;  // 32 = 'A' - 'a'

            if (c >= '0' && c <= '9')
                RetVal = RetVal * iBase + (c-'0');
            else if (iBase > 10 && c >= 'A' && c <= 'F')
                RetVal = RetVal * iBase + (c-55);   // 55 = 'A'-10
            else if (c == '-' && i==iStart)
                {}    // nothing to do
            else
                break;
            i++;
        };

        if (iStart < Len)
            if (Me[iStart] == '-')
                RetVal = 0-RetVal;

        iStart = i;
    };

    /**
        Translate text inside string into a number.
        @return the number.
    */
    long ToInt()  {
        long i = 0;
        return ToIntEx(i);
    };


    long Equal(String& s2)  {
        if (strcmp(Me, s2) == 0)
            RetVal = 1;
        else
            RetVal = 0;
    };

    long NotEqual(String& s2)  {
        if (strcmp(Me, s2) == 0)
            RetVal = 0;
        else
            RetVal = 1;
    };


    /**
        Scan for the first occurence of character c into the string.
        @return Index of the character (starting from 0), or -1 if character was not found.
    */
    long IndexOfChar(byte& c)  {
        long i;

        for(;i<Len;i++)
            if (Txt[i] == c)
                return i;

        RetVal = -1;
    };

};


/*
struct StringZ: public object  {
    byte Txt[200];

    signed_long isEmpty()  {
        RetVal = (Txt[0] == 0);
    };

    byte& geti(signed_long n)  {
        SetReference(RetVal,Txt[n]);
    };

    void set(String& cSource)  {
        Txt[0] = 0;
        Me += cSource;
    };

    void sum_set(String& cSource)  {
        AddSubStr(cSource,0,cSource.MaxItems);
    };

    void AddSubStr(String& cSource, long iStart, long iLength)  {
        long i;
        i = Len;
        while (cSource[iStart])  {
            if (i >= MaxItems || iStart>=iLength)
                break;
            Me[i++] = cSource[iStart++];
        };
    };

    void AddFormatNum(long n, long lpad)  {
        long origpad;
        char sgn;
        if (Len + lpad <= MaxItems)  {
            origpad = lpad;
            Txt[Len + lpad] = 0;
            if (n < 0)  {
                sgn = -1;
                n = 0-n;
            };
            while (--lpad >= 0)  {
                if (n || (origpad == lpad+1))  {
                    Me[Len+lpad] = '0' + (n % 10);
                    n = n / 10;
                }
                else if (sgn < 0)  {
                    Me[Len+lpad] = '-';
                    sgn = 0;
                }
                else
                    Me[Len+lpad] = ' ';
            };
        };
    };

    long ToInt()  {
        long i;
        char c;
        if (Len < 1)
            ExitFunction;
        while (Me[i])  {
            c = Me[i++];
            if (c >= '0' && c <= '9')
                RetVal = RetVal * 10 + (c-'0');
            else if (c == '-' && i==0)
                ;   // nothing to do
            else
                break;
        };
        if (Me[0] == '-')
            RetVal = 0-RetVal;
    };

};
*/


/*
// Cast to char.
signed_char ToChar(long n)  {
    RetVal = n;
};

// Cast to byte.
unsigned_char ToByte(long n)  {
    RetVal = n;
};

// Cast to word.
word ToWord(long n)  {
    RetVal = n;
};

// Cast to short int.
short ToShort(long n)  {
    RetVal = n;
};
*/


/// Cast to char.
inline noretval signed_char ToChar(nowarning signed_char n)  {
    asm_null;
};

/// Cast to byte.
inline noretval unsigned_char ToByte(nowarning unsigned_char n)  {
    asm_null;
};

/// Cast to word.
inline noretval word ToWord(nowarning word n)  {
    asm_null;
};

/// Cast to short int.
inline noretval short ToShort(nowarning short n)  {
    asm_null;
};


// unary minus operator
signed_long sub(signed_long n)  {
    return 0 - n;
};

// set subj (should be a long variable), to the value of the pointer of val. subj will have value of pointer to val. i.e. subj = &val;
private inline void SetReference(object& subj, object& val)  {
    asm_3;
};

// set subj (should be a reference to pointer), to the value of the pointer of val. i.e. &subj = &val;
private inline void reserved_SetPointer(void& subj, void& val)  {
    asm_3;
};

inline void SysCall(signed_long FnCode)  {
    asm_syscall;
};


long GetSysParam(long iIndex)  {
    SysCall(SysFn_sysSetParam);
};


long SetSysParam(long iIndex, long iValue)  {
    iIndex = iIndex | 0x80000000;
    SysCall(SysFn_sysSetParam);
};



//
// Set the CAN Node ID
// @param iNodeID is the new CAN node ID
//
void CANsetNodeID(short iNodeID) {
    SetSysParam(sysParam_CANNodeID,iNodeID);
}


//
// Get the CAN Node ID
// @return The CAN node ID
//
short CANgetNodeID() {
    return GetSysParam(sysParam_CANNodeID);
}

//
// Get the CAN current speed
// @return The CAN speed (enum value or speed)
//
long CANgetSpeed() {
    return GetSysParam(sysParam_CANBaud);
}

//
// Get the Node ID to be used on the user interface
// @return The target node ID
//
short CANgetUITarget() {
    return GetSysParam(sysParam_UITarget);
}


/**
    Set connection mode of MMI.
    @param iDevice is the device to connect.

    To set connection mode:
    - 0     use MCXLink autodetection and download user interface from MCX.
    - >0    manual connection to specified MCX and download user interface from MCX.
    - -1    use MCXLink autodetection but use current user interface (do not download it from MCX).

    @warning This function will work only on MMI. On other devices it will have no effect.
*/
void CANsetUITarget(long iDevice) {
    SetSysParam(sysParam_UITarget, iDevice);
}


/**
    Connect the MMI to another device.
    @param iDevice is the device to connect. Use 0 to set MCXLink autodetection, use -1 to simply reconnect to current device (and update UI if required).


    The application will stop and MMI will connect to the new device, download new user interface and start it.

    @warning This function will work only on MMI. On other devices it will have no effect.

*/
void MMIConnectToDevice(long iDevice)  {
    if (iDevice >= 0)  {
        // set new target
        CANsetUITarget(iDevice);
    };

    // send trigger to connect
    Can.SendVar(Can.NodeID, 0x5D04, 0x01, 1);
};




/**
    Read the language multiplexer.
    @return the language index.
*/
long GetUILanguage()  {
    // read language multiplexer
    return Can.RequestAndWaitVar(Can.NodeID, 0x5D04, 0x02, 0);
};


/**
    Set language multiplexer.

    @warning on MMI this will also reset the application.

    @param iLang is the language index: 0=default language, 1=first language (default), 2=second language, ...
*/
void SetUILanguage(long iLang)  {
    // write language multiplexer
    Can.SendVar(Can.NodeID, 0x5D04, 0x02, iLang);

    // send trigger to connect
    Can.SendVar(Can.NodeID, 0x5D04, 0x01, 1);

    // return to bios. required to immediatly change the language (if next instruction need to use string table)
    // TODO: check if suspend is supported in MMIGRS 128K, before to enable it.
    //Suspend(1);
};



/*
    04/06/09: unsupported functions to read/write local object dictionary:
        long GetODParam(word iIndex, byte iSubIndex, char iSize = -1);
        long SetODParam(word iIndex, byte iSubIndex, long iValue, char iSize = -1);
*/


/// Notify bios that a new application main loop is going to start.
void internal_StartNewMainLoop(long unused = 0)  {
    SysCall(SysFn_sysNewMainLoop);
};

/// Send message to debug console.
/// This function will be ignored when executing on an MCX device, it will only be available into the PC simulator.
/// @param cMsg the message.
default_impl void OutputDebugString(String& cMsg)  {
    SysCall(SysFn_sysOutputDebugString);
    //Com1.WriteStr(cMsg); Com1.WriteStr("\n");
};


/// Send message and a number to debug console.
/// This function will be ignored when executing on an MCX device, it will only be available into the PC simulator.
/// @param cMsg the message.
/// @param iNum value to be shown after cMsg.
void OutputDebugNum(String& cMsg, long iNum)  {
    String s(40);
    s += cMsg;
    s.AddFormatNum(iNum,11);
    OutputDebugString(s);
};


/**
    Set a watchdog.
    If after iTimeout seconds, this function is not called again, the device will be rebooted.
    This is a security function. Call this function in the application main loop, so if your program enter
    an infinite loop due to a bug, the device will not hang but will be rebooted after iTimeout seconds.
    @param iTimeout is the timeout in seconds. Use 0 to disable this function.

    @code

        #include "demolib.c"

        void App_Init()  {
            dsp.DrawTextCR(0,0,"Start");
        };

        void App_Run()  {

            // this function must be called at least every 5 seconds, otherwise device will reboot.
            SetWatchdog(5);

            if (kbd.Key == KEY_ESC)
                Sleep(6000);    // sleep 6 ms, so device will reboot

        };

    @endcode
*/
void SetWatchdog(word iTimeout)  {
    SysCall(SysFn_sysSetWatchdog);
};


///
/// Reboot.
/// Calling this function will reboot the device.
/// @param iRebootMode 0=reboot device, 1=reboot only application.
/// @warning This function will never return.
///
void Reboot(char iRebootMode = 0)  {
    if (iRebootMode == 1)  {
        char tmp[1];
        // this will generate an array access out of range, and will reboot application.
        // we can't use a constant value, because compiler detect the error and will not compile the application.
        tmp[iRebootMode+1] = 0;
    };

    SetWatchdog(1);
    while (1);
};


// copy memory from src to dst, handling overlap (memcpy)
private void reserved_memmove(void& dst, void& src, long iLen)  {

    #ifdef ENABLE_sysMemMove
        SysCall(SysFn_sysMemMove);

    #else

        long i;
        // must handle overlap of two structure
        long dst_addr;
        long src_addr;
        SetReference(dst_addr,dst);
        SetReference(src_addr,src);
        if (dst_addr < src_addr)  {
            for(i=0; i<iLen; i++)
                dst.GetByteRef(i) = src.GetByteRef(i);
        }
        else  {
            for(i=iLen; --i>=0; )
                dst.GetByteRef(i) = src.GetByteRef(i);
        };
    #endif
};


// set all bytes of a range of memory to a value
private void reserved_memset(void& dst, byte v, long iLen)  {
    long i;
    for(i=0; i<iLen; i++)
        dst.GetByteRef(i) = v;
};


//
// return ram offset (relative to start of ram) of the object
//
private long GetRamOffset(void& obj)  {
    long ram_addr;
    long obj_addr;
    SetReference(ram_addr,Sys.GetByteRef(0));   // Sys is the first object allocated into ram
    SetReference(obj_addr,obj.GetByteRef(0));
    return obj_addr - ram_addr;
};

//
// return E2P offset (relative to start of E2P) of the object
//
private long GetE2POffset(void& obj)  {
    long x_addr;
    long obj_addr;
    SetReference(x_addr,ReservedE2P.GetByteRef(0));   // this is the first E2P allocated object
    SetReference(obj_addr,obj.GetByteRef(0));
    return obj_addr - x_addr;
};


/// @return lowest value between nV1 and nV2
long min(long nV1, long nV2)  {
    if (nV1 > nV2)
        return nV2;
    return nV1;
};


/// @return highest value between nV1 and nV2
long max(long nV1, long nV2)  {
    if (nV1 < nV2)
        return nV2;
    return nV1;
};


/// @return absolute value of @param nVal
long abs(long nVal)  {
    if (nVal < 0)  {
        if (nVal == 0x80000000)     // this is a special case
            return 0x7FFFFFFF;
        else
            return 0-nVal;
    }
    else
        return nVal;
};


/// Check if value is between two limits
/// @return 1 if @param nVal is between @param nMin and @param nMax (including limits)
long between(long nVal, long nMin, long nMax)  {
    if (nVal >= nMin && nVal <= nMax)
        return 1;
    return 0;
};


long strcmp(String& a, String& b)  {
    RetVal = strncmp(a,b,a.Len+b.Len);
    if (RetVal == 0)
        RetVal = a.Len - b.Len;
};

long strncmp(String& a, String& b, long len)  {
    long i;
    long k;

    // limit check on characters present in both strings
    if (len > a.Len)  {
        len = a.Len;
        if (len != b.Len)
            RetVal = -1;
    };
    if (len > b.Len)  {
        len = b.Len;
        RetVal = 1;
    };

    // check every characters
    while (i<len && k==0)  {
        k = a[i] - b[i];
        i++;
    };

    if (k)
        RetVal = k;             // return compare result
};

word CRC16(object& a, long iSize, word StartCRC = 0x1D0F)  {
    SysCall(SysFn_sysCRC16);
};



/**
    Get device information.

    Example:
    @code

    void PrintDeviceInfo()  {
        TSysDeviceInfo DevInfo;
        String out;

        DevInfo.GetInfo();

        out = "\nProductCode:  ";
        out.AddFormatNum(DevInfo.ProductCode,8);
        Com1.WriteStr(out);

        out = "\nSerialNumber: ";
        out.AddFormatNum(DevInfo.SerialNumber,10);
        Com1.WriteStr(out);
    };

    @endcode

*/
struct TSysDeviceInfo  {
    byte    StructVersion;      //   =37. Used for future expansion
    long    ProductCode;        ///< Danfoss product code
    long    SerialNumber;       ///< Device serial number (unique within ProductCode)
    word    CustomProdutCode;   //   reserved
    long    BiosDate;           ///< date/time of bios
    byte    BiosVersion;        //   bios version (functional version)
    word    VendorID;           ///< Application Vendor ID
    word    AppID;              ///< Application ID (or application code)
    long    AppDate;            ///< date/time of application
                                // offset 24

    // still unsupported
    word    AppMaxSize;
    word    AppMaxStore;
    word    AppMaxE2P_KB;       // was AppMaxFreqStore
    word    AppRamSize;

    // supported
    byte    ioNumDI;            ///< Number of digital input
    byte    ioNumAI;            ///< Number of analog input
    byte    ioNumDO;            ///< Number of digital output
    byte    ioNumAO;            ///< Number of analog output

    // unsupported
    byte    ioNumPWM;


    // for old bios, this value was reported in KB
    long AppMaxE2P()  {
        RetVal = 1024 - 128;
    };

    void GetInfo()  {
        StructVersion = sizeof(Me);
        SysCall(SysFn_sysGetHWInfo);
    };


    word GetBiosVer()  {
        // unsupported (return 0.00)
    };


    /*
            CST 06C 06D 08  15  20 MMI MYK LCX
        DI   4   6   8   8  18  22   0   0  6
        AI   4   4   4   8  10  16   0   0  4
        DO   6*  6   6   8  15  20   0   0  6      *CSTFR1 have only 5 relay, the other one is not mounted
        AO   1   2   3   4   6   6   0   0  2
    */
    long isMCX08()  {
        return ioNumDO == 8;
    };

    long isMCX06()  {
        // NOTE: does not distinguish between MCX06D and MCX061V, CSTFR1, LCX06
        return ioNumDO == 6;
    };

    long isMMIGRS()  {
        return ioNumDO == 0;        // can not distinguish between MMIGRS and MMIMYK
    };

    void GetDeviceFamilyDescr(String& Descr)  {
        if (ioNumDO == 8)
            Descr = "MCX08M";
        else if (ioNumDO == 15)
            Descr = "MCX15B";
        else if (ioNumDO == 20)
            Descr = "MCX20B";
        else if (ioNumDO == 0)
            Descr = "MMIGRS";

        // here we assume to have an MCX06
        else if (ioNumDI == 6)
            Descr = "MCX06C";
        else if (ioNumDI == 8)
            Descr = "MCX06D";
        else
            Descr = "???";
    };

};


/**
    Get device information (extended).

    Example:
    @code

    void PrintDeviceInfo()  {
        TSysDeviceInfo_v2 DevInfo;
        String out;

        DevInfo.GetInfo();

        out = "\nProductCode:  ";
        out.AddFormatNum(DevInfo.ProductCode,8);
        Com1.WriteStr(out);

        out = "\nSerialNumber: ";
        out.AddFormatNum(DevInfo.SerialNumber,10);
        Com1.WriteStr(out);
    };

    @endcode

*/
struct TSysDeviceInfo_v2  {
    byte    StructVersion;      //   =50. Used for future expansion
    long    ProductCode;        ///< Device product code
    long    SerialNumber;       ///< Device serial number (unique within ProductCode)
    word    CustomProdutCode;   //   reserved
    long    BiosDate;           ///< date/time of bios
    byte    BiosVersion;        //   bios version (functional version)
    word    VendorID;           ///< Application Vendor ID
    word    AppID;              ///< Application ID (or application code)
    long    AppDate;            ///< date/time of application
                                // offset 24

    // here we start at offset 24, make sure next fields are always aligned to 32 bit offsets

    word    BiosVer;            //< Bios version

    /** Device bios code.
        Can be used to identify device family.

        <table>
            <tr> <td>4</td><td>MMIGRS (old)</td> </tr>
            <tr> <td>5</td><td>MCX08M</td> </tr>
            <tr> <td>6</td><td>MCX15B</td> </tr>
            <tr> <td>7</td><td>MCX20B</td> </tr>
            <tr> <td>9</td><td>CSTFR1</td> </tr>
            <tr> <td>10</td><td>MCX06C</td> </tr>
            <tr> <td>13</td><td>MMIMYK</td> </tr>
            <tr> <td>16</td><td>MCX06D</td> </tr>
            <tr> <td>17</td><td>MCXPC0</td> </tr>
            <tr> <td>18</td><td>MMILDS</td> </tr>
            <tr> <td>19</td><td>MCX061</td> </tr>
            <tr> <td>20</td><td>MMIGRS</td> </tr>
            <tr> <td>21</td><td>LCX06C</td> </tr>
            <tr> <td>25</td><td>MCX152</td> </tr>
            <tr> <td>26</td><td>LCX06D</td> </tr>
        </table>

    */
    word    BiosCode;           //< Danfoss Bios short code

    long    AppMaxSize;         // maximum application size (flash)
    long    AppMaxStore;        // maximum application storage area (flash)
    long    AppMaxE2P;          // maximum application parameters area (eeprom)
    long    AppRamSize;         // maximum application ram size (ram)
    long    UploadAreaSize;     // maximum upload area size (flash)

    word    SupportedFunctions; // each bit identify a special function supported by this bios (e.g. float)
                                // LBit(0) = float
                                // LBit(1) = multitask (unused)
                                // LBit(2) = multithread (unused)

    byte    ioNumDI;            ///< Number of digital input
    byte    ioNumAI;            ///< Number of analog input
    byte    ioNumDO;            ///< Number of digital output
    byte    ioNumAO;            ///< Number of analog output


    /**
        Get information from device.
    */
    void GetInfo()  {
        StructVersion = sizeof(Me);
        SysCall(SysFn_sysGetHWInfo);
    };


    /**
        Get bios version as HH.LL version number.
        e.g.:
        @code

        void DisplayBiosVer()  {
            TSysDeviceInfo_v2 DevInfo;
            DevInfo.GetInfo();
            dsp.DrawValueCR(0,0,"Bios ver.",DevInfo.GetBiosVer(),5,2);
        };

        @endcode
    */
    word GetBiosVer()  {
        #ifdef MMI_APP
            if (Can.NodeID == Can.GetUITarget())  {
                // local version, we get the bios code directly from device info structure
                if (BiosVer == 0x001E)
                    RetVal = 0;
                else
                    RetVal = (BiosVer >> 8) * 100 + (BiosVer & 0xFF);
            }
            else  {
                // on MMIGRS, read bios version with CANopen and translate into integer
                long ver;
                ver = Can.RequestAndWaitVar(Can.NodeID, 0x100A, 0x00, 0);

                // this calculates the BiosVer in the same way as other MCX
                //RetVal = (ver & 0x0F000000) << -16;     // major version
                //RetVal = RetVal | (((ver & 0x0F00) << -8) * 10 + (ver & 0x000F));

                // get version in decimal format
                RetVal = (ver & 0x0F000000) << -24;     // major version
                RetVal = RetVal * 100 + (((ver & 0x0F00) << -8) * 10 + (ver & 0x000F));
            };
        #else
            // local version, we get the bios code directly from device info structure
            if (BiosVer == 0x001E)
                RetVal = 0;
            else
                RetVal = (BiosVer >> 8) * 100 + (BiosVer & 0xFF);
        #endif
    };


    /*
            CST 06C 06D 08  15  20 MMI MYK LCX
        DI   4   6   8   8  18  22   0   0  6
        AI   4   4   4   8  10  16   0   0  4
        DO   6*  6   6   8  15  20   0   0  6      *CSTFR1 have only 5 relay, the other one is not mounted
        AO   1   2   3   4   6   6   0   0  2
    */
    long isMCX08()  {
        return ioNumDO == 8;
    };

    long isMCX06()  {
        // NOTE: MCX061V and LCX06 is not reported as MCX06 by this function
        // CSTFR1 report 6 DO, so we need to distingish it
        return (ioNumDO == 6) && (ioNumAO != 1) && (BiosCode != 19) && (BiosCode != 21) && (BiosCode != 26);
    };

    long isMMIGRS()  {
        return BiosCode == 33952 || BiosCode == 4 || BiosCode == 20;
    };

    long isMMIMYK()  {
        return BiosCode == 13;
    };

    long BiosSCode()  {
        RetVal = BiosCode;
    };


    /**
        Get family name of device.
        @param Descr will be set to the device family name (e.g. MCX06D, MMIGRS, ...).
    */
    void GetDeviceFamilyDescr(String& Descr)  {
        if (BiosCode == 21)
            Descr = "LCX06C";
        else if (BiosCode == 26)
            Descr = "LCX06D";
        else if (BiosCode == 19)
            Descr = "MCX061";    // MCX06M1V
        else if (BiosCode == 25)
            Descr = "MCX152";    // MCX15B2V
        else if (BiosCode == 17)
            Descr = "MCXPC0";

        else if (ioNumDO == 8)
            Descr = "MCX08M";
        else if (ioNumDO == 15)
            Descr = "MCX15B";
        else if (ioNumDO == 20)
            Descr = "MCX20B";
        else if (isMMIGRS())
            Descr = "MMIGRS";
        else if (isMMIMYK())
            Descr = "MMIMYK";

        // here we assume to have an MCX06
        else if (ioNumDI == 6)
            Descr = "MCX06C";
        else if (ioNumDI == 8)
            Descr = "MCX06D";
        else
            Descr = "??????";
    };


    /**
        Check if the bios support float operations.

        @return 1 if bios support float operations, 0 if not.

        @warning You need to call GetInfo() method, before use this function.

        @code

        #include "demolib.lib"
        #include "math.lib"

        float a = 12.3;

        void App_Init()  {
        };

        void App_Run()  {
            TSysDeviceInfo_v2 DevInfo;

            DevInfo.GetInfo();

            dsp.DrawValueCR(0,0,"float=", DevInfo.SupportFloat(), 5);

            if (DevInfo.SupportFloat())
                dsp.DrawValueCR(0,1,"a*2=", FloatToIntegerRound(a*2*100), 6,2);

        };

        @endcode

    */
    long SupportFloat()  {
        return SupportedFunctions.LBit(0);
    };


    /**
        Check if the bios support multitask.

        @return 1 if bios support multitask, 0 if not.

        @warning You need to call GetInfo() method, before use this function.

        @code

        #include "demolib.lib"
        #include "math.lib"

        float a = 12.3;

        void App_Init()  {
        };

        void App_Run()  {
            TSysDeviceInfo_v2 DevInfo;

            DevInfo.GetInfo();

            dsp.DrawValueCR(0,0,"multitask=", DevInfo.SupportMultitask(), 5);

        };

        @endcode

    */
    long SupportMultitask()  {
        return SupportedFunctions.LBit(1);
    };

};




/**
    Timer with one second resolution.
    @warning at startup the timer will be started and running.


    Example on how to use the timer:

    @code

    void Test()  {
        TTimerSec tmr;

        // start timer
        tmr.Start();

        // wait 3 seconds
        Sleep(3000);

        // now tmr.ElapsedSec() == 3
    };

    @endcode

*/
struct TTimerSec: public object  {
    long  Sec;

    override void set(TTimerSec& b)  {
        Sec = b.Sec;
    };

    /// Start timer.
    ///
    void Start()  {
        Sys.GetInfo();
        Me.Sec = Sys.SysTime.Sec;
    };

    /// Set timer to an invalid value.
    /// This value can be tested with isClear() method.
    /// This is the same value the timer have at startup, when no Start() method have been called on it.
    ///
    void Clear()  {
        Sec = 0;
    };

    /// return 1 if timer was cleared or no start was called.
    ///
    long isClear()  {
        if (Sec == 0)
            return 1;
        else
            return 0;
    };

    /// return number of seconds since timer was started (0-60 years).
    ///
    long ElapsedSec()  {
        Sys.GetInfo();
        RetVal = Sys.SysTime.Sec - Sec;
    };

    /// Set time in number of seconds
    ///
    void SetTime(long iSec)  {
        Sec = iSec;
    };

    /// Get time in number of seconds
    ///
    long GetTime()  {
        return Sec;
    };

    /// Add seconds to stored time.
    /// @param iSec is the number of seconds.
    void AddSec(long iSec)  {
        // add seconds
        Sec += iSec;
    };


    /// Add milli seconds to stored time.
    /// @param iMilliseconds is the number of milli seconds.
    void Addms(long iMilliseconds)  {
        Sec += iMilliseconds / 1000;
    };


    /**
        Start the timer with an offset on the current time.
        The offset is added to the return value of ElapsedSec().
        The offset is in milliseconds and can be positive or negative, but for TTimerSec only
        seconds are added, milliseconds are lost (i.e. you should specify only multiples of 1000).

        @code
        #include "demolib.c"

        void App_Init()  {
            TTimer t;

            t.EarlyStart(3000);     // start timer at 3 seconds
            Sleep(1000);
            dsp.DrawValueCR(0,1,"t1=",t.Elapsedms(),6,3);   // will show 4.000 sec.

            t.EarlyStart(-1000);    // start timer at -1 second
            Sleep(2000);
            dsp.DrawValueCR(0,2,"t2=",t.Elapsedms(),6,3);   // will show 1.000 sec.
        };

        void App_Run()  {
        };
        @endcode

    */
    void EarlyStart(long iOffset)  {
        Start();
        Addms(0 - iOffset);
    };

};


/**
    Timer with one second resolution, that can be stopped and resumed.

    @warning at startup the timer will be paused.


    Example on how to use the timer:

    @code

    void Test()  {
        TStoppableTimerSec tmr;

        // start timer
        tmr.Start();

        // wait 3 seconds
        Sleep(3000);

        // now tmr.ElapsedSec() == 3

        // pause timer
        tmr.Pause();

        // wait 2 seconds
        Sleep(2000);

        // now tmr.ElapsedSec() == 3 (and it's paused)

        // restart timer
        tmr.Resume();

        // wait 2 seconds
        Sleep(2000);

        // now tmr.ElapsedSec() == 5
    };

    @endcode

*/
struct TStoppableTimerSec: public TTimerSec  {

    /// return number of seconds since timer was started (0-60 years).
    ///
    override long ElapsedSec()  {
        if (Sec <= 0)   // this condition should be isPaused(), but this is more efficent
            RetVal = 0 - Sec;
        else  {
            Sys.GetInfo();
            RetVal = Sys.SysTime.Sec - Sec;
        };
    };

    /// Check if timer was paused.
    /// @return 0 if timer is running, 1 if timer was paused and is idle now.
    ///
    long isPaused()  {
        if (Sec <= 0)
            RetVal = 1;
        else
            RetVal = 0;
    };

    /// Pause the timer.
    /// The timer will be stopped, next calls to ElapsedSec() will always return the elapsed time since this function was called.
    /// If the timer is already paused, nothing will change.
    ///
    void Pause()  {
        // store number of seconds elapsed, to start again the timer later
        if (!isPaused())
            Sec = 0 - Me.ElapsedSec();
    };

    /// Resume the timer.
    /// The timer will be resumed, next calls to ElapsedSec() will return the elapsed time excluding the time the timer was paused.
    /// If the timer is already running, nothing will change.
    ///
    void Resume()  {
        long PauseSec;

        if (isPaused())  {
            PauseSec = Sec;

            // start timer again
            Me.Start();

            // add elapsed seconds of the last pause
            Me.AddSec( PauseSec );
        };
    };
};


/// Timer with one millisecond resolution
struct TTimer: public TTimerSec  {
    word  MilliSec;

    override void set(TTimer& b)  {
        Sec = b.Sec;
        MilliSec = b.MilliSec;
    };

    TTimer sub(TTimer& time2)  {
        RetVal = Me;
        RetVal -= time2;
    };

    void sub_set(TTimer& time2)  {
        long ms;
        Sec = Sec - time2.Sec;
        ms = MilliSec - time2.MilliSec; // can be bigger than a word value and can have sign
        Sec += ms / 1000;               // adjust seconds
        if (ms < 0)  {
            ms = 1000 + ms % 1000;
            Sec--;      // remove 1 sec
        };
        MilliSec = ms % 1000;    // set milliseconds
    };

    // start timer
    override void Start()  {
        Sys.GetInfo();
        Me = Sys.SysTime;
    };

    // set timer to 0
    override void Clear()  {
        Sec = 0;
        MilliSec = 0;
    };

    // return 1 if timer was cleared or no start was called
    override long isClear()  {
        if (Sec == 0 && MilliSec == 0)
            return 1;
        else
            return 0;
    };

    /// Check if time was elapsed and restart timer.
    /// @param iCheck is the time (milliseconds)
    /// @return 0 if time was not elapsed, 1 if it was elapsed. The timer will be restarted immediatly.
    /// @warning the timer will be restarted exactly when it was elapsed, even if the check was made later.
    /// e.g.:
    /// - you start the timer: timer.Start()
    /// - after 12 seconds you check the timer. The return value will be 1: if (timer.CheckAndRestartTimer(5000)) ...
    /// - after another 2 seconds, you the timer again. The return value will be 1 (because you already miss one hit): if (timer.CheckAndRestartTimer(5000)) ...
    /// - after another 2 seconds, you the timer again. The return value will be 0: if (timer.CheckAndRestartTimer(5000)) ...
    ///
    long CheckAndRestartTimer(long iCheck)  {
        Sys.GetInfo();
        if (Elapsedms() >= iCheck)  {
            RetVal = 1;

            // add milliseconds
            Addms(iCheck);
        }
        else
            RetVal = 0;
    };

    /// return number of milliseconds elapsed since timer was started (0-20 days, no rewind).
    ///
    long Elapsedms()  {
        // the only difference is that the code for this function is shorter and the timer is started by default
        Sys.GetInfo();
        RetVal = Sys.SysTime.Sec - Sec;
        if (RetVal >= 20*24*60*60)
            RetVal = 20*24*60*60 * 1000;
        else
            RetVal = RetVal * 1000 + Sys.SysTime.MilliSec - MilliSec;
    };


    /**
        Return number of milliseconds elapsed since timer was started (0-20 days, no rewind).
        The timer will also be restarted.
        This function ensure no milliseconds will be lost between the time you read elapsed ms and
        the time you restart the timer.
        @warning This function is only supported on TTimer objects.
    */
    long GetElapsedmsAndRestart()  {
        RetVal = Elapsedms();
        Me = Sys.SysTime;
    };


    override void Addms(long iMilliseconds)  {
        // add milliseconds
        Sec += iMilliseconds / 1000;
        MilliSec += iMilliseconds % 1000;

        // normalize result
        Sec += MilliSec / 1000;
        MilliSec = MilliSec % 1000;
    };


    /**
        Start the timer with an offset on the current time.
        The offset is added to the return value of Elapsedms().
        The offset is in milliseconds and can be positive or negative.

        @code
        #include "demolib.c"

        void App_Init()  {
            TTimer t;

            t.EarlyStart(3000);     // start timer at 3 seconds
            Sleep(1000);
            dsp.DrawValueCR(0,1,"t1=",t.Elapsedms(),6,3);   // will show 4.000 sec.

            t.EarlyStart(-1000);    // start timer at -1 second
            Sleep(2000);
            dsp.DrawValueCR(0,2,"t2=",t.Elapsedms(),6,3);   // will show 1.000 sec.
        };

        void App_Run()  {
        };
        @endcode

    */
    override void EarlyStart(long iOffset)  {
        Start();
        Addms(0 - iOffset);
    };

};


/**
    Handle a reverse counter in milliseconds.

    Use ReverseCountms() to setup the counter, and Remainingms() to check if time was elapsed.

    e.g.:
    @code
    TReverseTimer tmr;

    // initialize reverse count to 5 seconds
    tmr.ReverseCountms(5000);

    while(1)  {
        // display elapsed time to the console
        OutputDebugNum("Remaining:",tmr.Remainingms());
    };
    @endcode

*/
struct TReverseTimer: public TTimer  {


    /**
        Init reverse timer.
        @param iCountms is the number of milliseconds to initialize the counter.
    */
    void ReverseCountms(long iCountms)  {
        Me.Start();
        Me.Addms(iCountms);
    };

    /**
        Get remaining timer.
        @return number of milliseconds remaining before timer reach 0. Once the timer reach 0, it will stop.
    */
    long Remainingms()  {
        if (Sec <= 0)
            RetVal = Sec * -1000;
        else  {
            RetVal = Me.Elapsedms();
            if (RetVal > 0)
                RetVal = 0;
            else
                RetVal = 0 - RetVal;
        };
    };

    // unsupported
    /*
    /// Pause the timer.
    /// The timer will be stopped, next calls to ElapsedSec() will always return the elapsed time since this function was called.
    ///
    override void Pause()  {
        // store number of seconds elapsed, to start again the timer later
        long PauseSec;

        PauseSec = Me.ElapsedSec();

        if (PauseSec > 0)
            Sec = 0;    // time elapsed
        else
            Sec = Me.ElapsedSec();  // this value is negative
    };

    /// Resume the timer.
    /// The timer will be resumed, next calls to ElapsedSec() will return the elapsed time excluding the time the timer was paused.
    ///
    override void Resume()  {
        long PauseSec;

        // if reverse timer has not reached zero...
        if (Sec)  {
            PauseSec = Sec;

            // start timer again
            Me.Start();

            // add elapsed seconds of the last pause
            Me.AddSec( 0 - PauseSec );
        };
    };
    */
};


struct TSysInfo :public object  {
    byte   SysInfoVersion;          // used for future expansion
    byte   LastVMError;             // last error code, to be used after restart of application
                                    //word   Filler1;                //$$$$$$$$$$ filler to align to 32 bit
    long   LastVMErrorPC;           // Program Counter of last error, to be used after restart of application
    TTimer SysTime;                 // system timer (long Sec + word MilliSec)
                                    //word   Filler2;                //$$$$$$$$$$ filler to align to 32 bit
    long   SysMaxLoopTime;          // MaxLoopTime of the bios main loop
    long   MaxStackSize;            // Maximum stack size used by the VM application
    bit    PersistentMemChanged;    // the persistent memory (EEPROM) have been changed
    bit    DisableDownload;         // 1=application download is not allowed
    //bit    CriticalError_NoReboot;
    //bit    CriticalError_Array;
    //bit    CriticalError_Stack;
    //bit    CriticalError_Free[5];
                                    //byte   Filler3[3];              //$$$$$$$$$$ filler to align to 32 bit


    void GetInfo()  {
        SysCall(SysFn_sysGetInfo);
        SysMaxLoopTime = GetSysParam(sysParam_MaxLoopTime);
    };

    void SetInfo()  {
        SetSysParam(sysParam_MaxLoopTime,SysMaxLoopTime);
    };
};


/// Wait iMillisec milliseconds.
void Sleep(long iMillisec)  {
    TTimer t;
    t.Start();
    SysCall(SysFn_sysIdle);     // return control to bios
    while (t.Elapsedms() < iMillisec)  {
        SysCall(SysFn_sysIdle); // return control to bios
    };
};




/*
    NOTE: The function is documented into multitask.lib.

    The function stay here because it was defined here the first time, but it should stay into multitask.lib.
*/
void Suspend(long iMillisec, long WakeUpEvents = 0)  {
    SysCall(SysFn_sysSuspend);      // return control to bios and suspend this task for iSleep milliseconds.

    //     WARNING: Supported only on bios newer than 30th May 2008.
};


// global system variables
//byte Ram[0];    // give access to system ram
TSysInfo Sys;
