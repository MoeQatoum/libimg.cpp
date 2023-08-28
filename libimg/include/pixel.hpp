#ifndef LIB_IMG_PIXEL_H
#define LIB_IMG_PIXEL_H

#include <array>

#include "common.hpp"
#include "ops.hpp"
#include "types.hpp"

namespace img {

    struct GREY8 : public PIXEL_NOR_OP<GREY8>,
                   public PIXEL_ADD_OP<GREY8>,
                   public PIXEL_SUB_OP<GREY8>,
                   public PIXEL_MUL_OP<GREY8>,
                   public PIXEL_DEV_OP<GREY8> {
        u8 g;

        operator u8() const {
            return g;
        }

        operator GREYa8() const;
    };

    struct GREYa8 : public PIXEL_NOR_OP<GREYa8>,
                    public PIXEL_ADD_OP<GREYa8>,
                    public PIXEL_SUB_OP<GREYa8>,
                    public PIXEL_MUL_OP<GREYa8>,
                    public PIXEL_DEV_OP<GREYa8> {
        u8 g, a;

        operator arr2<u8>() const {
            return {g, a};
        }

        operator GREY8() const;
    };

    //////////////RGB//////////////

    struct RGB8 : public PIXEL_NOR_OP<RGB8>,
                  public PIXEL_ADD_OP<RGB8>,
                  public PIXEL_SUB_OP<RGB8>,
                  public PIXEL_MUL_OP<RGB8>,
                  public PIXEL_DEV_OP<RGB8> {
        u8 r, g, b;

        operator arr3<u8>() const {
            return {r, g, b};
        }
    };

    //////////////RGBA//////////////

    struct RGBa8 : public PIXEL_NOR_OP<RGBa8>,
                   public PIXEL_ADD_OP<RGBa8>,
                   public PIXEL_SUB_OP<RGBa8>,
                   public PIXEL_MUL_OP<RGBa8>,
                   public PIXEL_DEV_OP<RGBa8> {
        u8 r, g, b, a;

        operator arr4<u8>() const {
            return {r, g, b, a};
        }
    };

    //////////////BGR//////////////

    struct BGR8 : public PIXEL_NOR_OP<BGR8>,
                  public PIXEL_ADD_OP<BGR8>,
                  public PIXEL_SUB_OP<BGR8>,
                  public PIXEL_MUL_OP<BGR8>,
                  public PIXEL_DEV_OP<BGR8> {
        u8 b, g, r;

        operator arr3<u8>() const {
            return {r, g, b};
        }
    };

    //////////////BGRA//////////////

    struct BGRa8 : public PIXEL_NOR_OP<BGRa8>,
                   public PIXEL_ADD_OP<BGRa8>,
                   public PIXEL_SUB_OP<BGRa8>,
                   public PIXEL_MUL_OP<BGRa8>,
                   public PIXEL_DEV_OP<BGRa8> {
        u8 b, g, r, a;

        operator arr4<u8>() const {
            return {r, g, b, a};
        }
    };

    struct HDR32 {};

} // namespace img

#endif // LIB_IMG_PIXEL_H