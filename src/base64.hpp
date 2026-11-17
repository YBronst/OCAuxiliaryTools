//  base64.hpp
//  Autor Konstantin Pilipchuk
//  mailto:lostd@ukr.net

#ifndef BASE64_HPP_INCLUDED
#define BASE64_HPP_INCLUDED

#pragma once

#include <iterator>
#include <ios>
#include <string>

template<class E = char, class Tr = std::char_traits<E>>
class base64
{
public:
    typedef unsigned char byte_t;
    typedef E             char_type;
    typedef Tr            traits_type;

    // base64 requires max line length <= 72 characters
    // you can fill end of line
    // it may be crlf, crlfsp, noline or other class like it

    struct lf
    {
        template<class OI>
        OI operator()(OI to) const
        {
            *to = Tr::to_char_type('\n'); ++to;
            return to;
        }
    };

    struct crlf
    {
        template<class OI>
        OI operator()(OI to) const
        {
            *to = Tr::to_char_type('\r'); ++to;
            *to = Tr::to_char_type('\n'); ++to;
            return to;
        }
    };

    struct crlfsp
    {
        template<class OI>
        OI operator()(OI to) const
        {
            *to = Tr::to_char_type('\r'); ++to;
            *to = Tr::to_char_type('\n'); ++to;
            *to = Tr::to_char_type(' ');  ++to;
            return to;
        }
    };

    struct noline
    {
        template<class OI>
        OI operator()(OI to) const
        {
            return to;
        }
    };

    struct three2four
    {
        void zero()
        {
            data_[0] = 0;
            data_[1] = 0;
            data_[2] = 0;
        }

        byte_t get_0() const { return data_[0]; }
        byte_t get_1() const { return data_[1]; }
        byte_t get_2() const { return data_[2]; }

        void set_0(byte_t ch) { data_[0] = ch; }
        void set_1(byte_t ch) { data_[1] = ch; }
        void set_2(byte_t ch) { data_[2] = ch; }

        // 0000 0000  1111 1111  2222 2222
        // xxxx xxxx  xxxx xxxx  xxxx xxxx
        // 0000 0011  1111 2222  2233 3333

        int b64_0() const { return (data_[0] & mask_1111_1100) >> 2; }
        int b64_1() const { return ((data_[0] & mask_0000_0011) << 4) + ((data_[1] & mask_1111_0000) >> 4); }
        int b64_2() const { return ((data_[1] & mask_0000_1111) << 2) + ((data_[2] & mask_1100_0000) >> 6); }
        int b64_3() const { return (data_[2] & mask_0011_1111); }

        void b64_0(int ch)
        {
            data_[0] = static_cast<byte_t>(((ch & mask_0011_1111) << 2) | (mask_0000_0011 & data_[0]));
        }

        void b64_1(int ch)
        {
            data_[0] = static_cast<byte_t>(((ch & mask_0011_0000) >> 4) | (mask_1111_1100 & data_[0]));
            data_[1] = static_cast<byte_t>(((ch & mask_0000_1111) << 4) | (mask_0000_1111 & data_[1]));
        }

        void b64_2(int ch)
        {
            data_[1] = static_cast<byte_t>(((ch & mask_0011_1100) >> 2) | (mask_1111_0000 & data_[1]));
            data_[2] = static_cast<byte_t>(((ch & mask_0000_0011) << 6) | (mask_0011_1111 & data_[2]));
        }

        void b64_3(int ch)
        {
            data_[2] = static_cast<byte_t>((ch & mask_0011_1111) | (mask_1100_0000 & data_[2]));
        }

    private:
        byte_t data_[3] = { 0, 0, 0 };
    };

    template<class II, class OI, class State, class Endline>
    II put(II first, II last, OI to, State&, Endline endl_fn) const
    {
        three2four _3to4;
        int line_octets = 0;

        while (first != last)
        {
            _3to4.zero();

            // We take 3 characters at a time
            _3to4.set_0(static_cast<byte_t>(*first));
            ++first;

            if (first == last)
            {
                *to = Tr::to_char_type(base64_chars[_3to4.b64_0()]); ++to;
                *to = Tr::to_char_type(base64_chars[_3to4.b64_1()]); ++to;
                *to = Tr::to_char_type('='); ++to;
                *to = Tr::to_char_type('='); ++to;
                break;
            }

            _3to4.set_1(static_cast<byte_t>(*first));
            ++first;

            if (first == last)
            {
                *to = Tr::to_char_type(base64_chars[_3to4.b64_0()]); ++to;
                *to = Tr::to_char_type(base64_chars[_3to4.b64_1()]); ++to;
                *to = Tr::to_char_type(base64_chars[_3to4.b64_2()]); ++to;
                *to = Tr::to_char_type('='); ++to;
                break;
            }

            _3to4.set_2(static_cast<byte_t>(*first));
            ++first;

            *to = Tr::to_char_type(base64_chars[_3to4.b64_0()]); ++to;
            *to = Tr::to_char_type(base64_chars[_3to4.b64_1()]); ++to;
            *to = Tr::to_char_type(base64_chars[_3to4.b64_2()]); ++to;
            *to = Tr::to_char_type(base64_chars[_3to4.b64_3()]); ++to;

            if (line_octets == 17) // base64 limits the line length to a maximum of 72 characters
            {
                to = endl_fn(to);
                line_octets = 0;
            }
            else
            {
                ++line_octets;
            }
        }

        return first;
    }

    template<class II, class OI, class State>
    II get(II first, II last, OI to, State& st) const
    {
        three2four _3to4;
        int ch;

        while (first != last)
        {
            // Take octet
            _3to4.zero();

            // -- 0 --
            // Search next valid char...
            while ((ch = get_char_type(*first)) == unknown_char)
            {
                if (++first == last)
                {
                    st |= std::ios_base::failbit | std::ios_base::eofbit;
                    return first; // unexpected EOF
                }
            }

            if (ch == equal_char)
            {
                // Error! First character in octet can't be '='
                st |= std::ios_base::failbit;
                return first;
            }
            else
            {
                _3to4.b64_0(ch);
            }

            // -- 1 --
            // Search next valid char...
            while (++first != last)
            {
                if ((ch = get_char_type(*first)) != unknown_char)
                    break;
            }

            if (first == last)
            {
                st |= std::ios_base::failbit | std::ios_base::eofbit; // unexpected EOF
                return first;
            }

            if (ch == equal_char)
            {
                // Error! Second character in octet can't be '='
                st |= std::ios_base::failbit;
                return first;
            }
            else
            {
                _3to4.b64_1(ch);
            }

            // -- 2 --
            // Search next valid char...
            while (++first != last)
            {
                if ((ch = get_char_type(*first)) != unknown_char)
                    break;
            }

            if (first == last)
            {
                // Error! Unexpected EOF. Must be '=' or base64 character
                st |= std::ios_base::failbit | std::ios_base::eofbit;
                return first;
            }

            if (ch == equal_char)
            {
                // OK!
                _3to4.b64_2(0);
                _3to4.b64_3(0);

                // check for EOF
                if (++first == last)
                {
                    st |= std::ios_base::eofbit;
                }
                else if (get_char_type(*first) == equal_char)
                {
                    ++first; // Skip second '='
                }

                // write 1 byte to output
                *to = static_cast<byte_t>(_3to4.get_0()); ++to;
                return first;
            }
            else
            {
                _3to4.b64_2(ch);
            }

            // -- 3 --
            // Search next valid char...
            while (++first != last)
            {
                if ((ch = get_char_type(*first)) != unknown_char)
                    break;
            }

            if (first == last)
            {
                // Unexpected EOF. Ignore it.
                st |= std::ios_base::eofbit;
                return first;
            }

            if (ch == equal_char)
            {
                // OK!
                _3to4.b64_3(0);

                // write to output 2 bytes
                *to = static_cast<byte_t>(_3to4.get_0()); ++to;
                *to = static_cast<byte_t>(_3to4.get_1()); ++to;

                ++first; // set position to next character
                return first;
            }
            else
            {
                _3to4.b64_3(ch);
            }

            // write to output 3 bytes
            *to = static_cast<byte_t>(_3to4.get_0()); ++to;
            *to = static_cast<byte_t>(_3to4.get_1()); ++to;
            *to = static_cast<byte_t>(_3to4.get_2()); ++to;

            ++first;
        }

        return first;
    }

protected:
    static constexpr byte_t mask_0000_0011 = 0x03;
    static constexpr byte_t mask_1111_1100 = 0xFC;
    static constexpr byte_t mask_1111_0000 = 0xF0;
    static constexpr byte_t mask_0011_0000 = 0x30;
    static constexpr byte_t mask_0011_1100 = 0x3C;
    static constexpr byte_t mask_0000_1111 = 0x0F;
    static constexpr byte_t mask_1100_0000 = 0xC0;
    static constexpr byte_t mask_0011_1111 = 0x3F;

    static constexpr int equal_char   = -1;
    static constexpr int unknown_char = -2;

    static constexpr char base64_chars[64] = {
        'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
        'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
        '0','1','2','3','4','5','6','7','8','9',
        '+','/'
    };

    int get_char_type(int c) const
    {
        if (base64_chars[62] == c)
            return 62;

        if (base64_chars[63] == c)
            return 63;

        if ((base64_chars[0] <= c) && (base64_chars[25] >= c))
            return c - base64_chars[0];

        if ((base64_chars[26] <= c) && (base64_chars[51] >= c))
            return c - base64_chars[26] + 26;

        if ((base64_chars[52] <= c) && (base64_chars[61] >= c))
            return c - base64_chars[52] + 52;

        if (c == Tr::to_int_type('='))
            return equal_char;

        return unknown_char;
    }
};

#endif // BASE64_HPP_INCLUDED
