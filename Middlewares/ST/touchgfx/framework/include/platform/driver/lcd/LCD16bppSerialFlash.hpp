/******************************************************************************
* Copyright (c) 2018(-2024) STMicroelectronics.
* All rights reserved.
*
* This file is part of the TouchGFX 4.24.2 distribution.
*
* This software is licensed under terms that can be found in the LICENSE file in
* the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
*******************************************************************************/

/**
 * @file platform/driver/lcd/LCD16bppSerialFlash.hpp
 *
 * Declares the touchgfx::LCD16bppSerialFlash class.
 */
#ifndef TOUCHGFX_LCD16BPPSERIALFLASH_HPP
#define TOUCHGFX_LCD16BPPSERIALFLASH_HPP

#include <touchgfx/Bitmap.hpp>
#include <touchgfx/hal/FlashDataReader.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/lcd/LCD16DebugPrinter.hpp>

namespace touchgfx
{
/**
 * This class contains the various low-level drawing routines for drawing bitmaps, texts and
 * rectangles on 16 bits per pixel displays.
 *
 * @see LCD
 *
 * @note All coordinates are expected to be in absolute coordinates!
 */
class LCD16bppSerialFlash : public LCD
{
public:
    /**
     * Creates a LCD16bppSerialFlash object. The FlashDataReader object is used to fetch
     * data from the external flash.
     *
     * @param [in] flashReader Reference to a FlashDataReader object.
     */
    LCD16bppSerialFlash(FlashDataReader& flashReader);

    virtual void drawPartialBitmap(const Bitmap& bitmap, int16_t x, int16_t y, const Rect& rect, uint8_t alpha = 255, bool useOptimized = true);

    virtual void blitCopy(const uint16_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    virtual void blitCopy(const uint8_t* sourceData, Bitmap::BitmapFormat sourceFormat, const Rect& source, const Rect& blitRect, uint8_t alpha, bool hasTransparentPixels);

    virtual uint16_t* copyFrameBufferRegionToMemory(const Rect& visRegion, const Rect& absRegion, const BitmapId bitmapId);

    virtual Rect copyFrameBufferRegionToMemory(const Rect& visRegion, const Rect& absRegion, uint8_t* dst, int16_t dstWidth, int16_t dstHeight);

    virtual void copyAreaFromTFTToClientBuffer(const Rect& region);

    virtual void fillRect(const Rect& rect, colortype color, uint8_t alpha = 255);

    virtual void fillBuffer(uint8_t* const destination, uint16_t pixelStride, const Rect& rect, const colortype color, const uint8_t alpha);

    virtual uint8_t bitDepth() const
    {
        return 16;
    }

    virtual Bitmap::BitmapFormat framebufferFormat() const
    {
        return Bitmap::RGB565;
    }

    virtual uint16_t framebufferStride() const
    {
        return getFramebufferStride();
    }

    /**
     * Framebuffer stride in bytes. The distance (in bytes) from the start of one
     * framebuffer row, to the next.
     *
     * @return The number of bytes in one framebuffer row.
     */
    FORCE_INLINE_FUNCTION static uint16_t getFramebufferStride()
    {
        assert(HAL::FRAME_BUFFER_WIDTH > 0 && "HAL has not been initialized yet");
        return HAL::FRAME_BUFFER_WIDTH * 2;
    }

    /**
     * Generates a color representation to be used on the LCD, based on 24 bit RGB values.
     *
     * @param  red   Value of the red part (0-255).
     * @param  green Value of the green part (0-255).
     * @param  blue  Value of the blue part (0-255).
     *
     * @return The color representation depending on LCD color format.
     */
    FORCE_INLINE_FUNCTION static uint16_t getNativeColorFromRGB(uint8_t red, uint8_t green, uint8_t blue)
    {
        return ((red << 8) & 0xF800) | ((green << 3) & 0x07E0) | ((blue >> 3) & 0x001F);
    }

    /**
     * Generates a color representation to be used on the LCD, based on 24 bit RGB values.
     *
     * @param  color The color.
     *
     * @return The color representation depending on LCD color format.
     */
    FORCE_INLINE_FUNCTION static uint16_t getNativeColor(colortype color)
    {
        return ((color >> 8) & 0xF800) | ((color >> 5) & 0x07E0) | ((color >> 3) & 0x001F);
    }

    /**
     * Enables the texture mappers for all image formats. This allows drawing any image
     * using Bilinear Interpolation and Nearest Neighbor algorithms, but might use a lot of
     * memory for the drawing algorithms.
     */
    void enableTextureMapperAll();

    /**
     * Enables the texture mappers for L8_RGB565 image format. This allows drawing L8_RGB565
     * images using Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperL8_RGB565_BilinearInterpolation,
     *      enableTextureMapperL8_RGB565_NearestNeighbor
     */
    void enableTextureMapperL8_RGB565();

    /**
     * Enables the texture mappers for L8_RGB565 image format. This allows drawing L8_RGB565
     * images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperL8_RGB565, enableTextureMapperL8_RGB565_NearestNeighbor
     */
    void enableTextureMapperL8_RGB565_BilinearInterpolation();

    /**
     * Enables the texture mappers for L8_RGB565 image format. This allows drawing L8_RGB565
     * images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperL8_RGB565, enableTextureMapperL8_RGB565_BilinearInterpolation
     */
    void enableTextureMapperL8_RGB565_NearestNeighbor();

    /**
     * Enables the texture mappers for L8_RGB888 image format. This allows drawing L8_RGB888
     * images using Bilinear Interpolation and NearestNeighbor algorithms.
     *
     * @see enableTextureMapperL8_RGB888_BilinearInterpolation,
     *      enableTextureMapperL8_RGB888_NearestNeighbor
     */
    void enableTextureMapperL8_RGB888();

    /**
     * Enables the texture mappers for L8_RGB888 image format. This allows drawing L8_RGB888
     * images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperL8_RGB888, enableTextureMapperL8_RGB888_NearestNeighbor
     */
    void enableTextureMapperL8_RGB888_BilinearInterpolation();

    /**
     * Enables the texture mappers for L8_RGB888 image format. This allows drawing L8_RGB888
     * images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperL8_RGB888, enableTextureMapperL8_RGB888_BilinearInterpolation
     */
    void enableTextureMapperL8_RGB888_NearestNeighbor();

    /**
     * Enables the texture mappers for L8_ARGB8888 image format. This allows drawing
     * L8_ARGB8888 images using Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperL8_ARGB8888_BilinearInterpolation,
     *      enableTextureMapperL8_ARGB8888_NearestNeighbor
     */
    void enableTextureMapperL8_ARGB8888();

    /**
     * Enables the texture mappers for L8_ARGB8888 image format. This allows drawing
     * L8_ARGB8888 images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperL8_ARGB8888, enableTextureMapperL8_ARGB8888_NearestNeighbor
     */
    void enableTextureMapperL8_ARGB8888_BilinearInterpolation();

    /**
     * Enables the texture mappers for L8_ARGB8888 image format. This allows drawing
     * L8_ARGB8888 images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperL8_ARGB8888, enableTextureMapperL8_ARGB8888_BilinearInterpolation
     */
    void enableTextureMapperL8_ARGB8888_NearestNeighbor();

    /**
     * Enables the texture mappers for RGB565 image format. This allows drawing RGB565
     * images using Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperRGB565_Opaque_BilinearInterpolation,
     *      enableTextureMapperRGB565_Opaque_NearestNeighbor,
     *      enableTextureMapperRGB565_NonOpaque_BilinearInterpolation,
     *      enableTextureMapperRGB565_NonOpaque_NearestNeighbor
     */
    void enableTextureMapperRGB565();

    /**
     * Enables the texture mappers for Opaque RGB565 image format. This allows drawing
     * RGB565 images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperRGB565
     */
    void enableTextureMapperRGB565_Opaque_BilinearInterpolation();

    /**
     * Enables the texture mappers for NonOpaque RGB565 image format. This allows drawing
     * RGB565 images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperRGB565
     */
    void enableTextureMapperRGB565_NonOpaque_BilinearInterpolation();

    /**
     * Enables the texture mappers for Opaque RGB565 image format. This allows drawing
     * RGB565 images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperRGB565
     */
    void enableTextureMapperRGB565_Opaque_NearestNeighbor();

    /**
     * Enables the texture mappers for NonOpaque RGB565 image format. This allows drawing
     * RGB565 images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperRGB565
     */
    void enableTextureMapperRGB565_NonOpaque_NearestNeighbor();

    /**
     * Enables the texture mappers for ARGB8888 image format. This allows drawing ARGB8888
     * images using Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperARGB8888_BilinearInterpolation,
     *      enableTextureMapperARGB8888_NearestNeighbor
     */
    void enableTextureMapperARGB8888();

    /**
     * Enables the texture mappers for ARGB8888 image format. This allows drawing ARGB8888
     * images using Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperARGB8888, enableTextureMapperARGB8888_NearestNeighbor
     */
    void enableTextureMapperARGB8888_BilinearInterpolation();

    /**
     * Enables the texture mappers for ARGB8888 image format. This allows drawing ARGB8888
     * images using Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperARGB8888, enableTextureMapperARGB8888_BilinearInterpolation
     */
    void enableTextureMapperARGB8888_NearestNeighbor();

    /**
     * Enables the texture mappers for A4 image format. This allows drawing A4 images using
     * Bilinear Interpolation and Nearest Neighbor algorithms.
     *
     * @see enableTextureMapperA4_BilinearInterpolation, enableTextureMapperA4_NearestNeighbor
     */
    void enableTextureMapperA4();

    /**
     * Enables the texture mappers for A4 image format. This allows drawing A4 images using
     * Bilinear Interpolation algorithm.
     *
     * @see enableTextureMapperA4, enableTextureMapperA4_NearestNeighbor
     */
    void enableTextureMapperA4_BilinearInterpolation();

    /**
     * Enables the texture mappers for A4 image format. This allows drawing A4 images using
     * Nearest Neighbor algorithm.
     *
     * @see enableTextureMapperA4, enableTextureMapperA4_BilinearInterpolation
     */
    void enableTextureMapperA4_NearestNeighbor();

    /**
     * Enables all the decompressors available for L8 images (L4, RLE and LZW9).
     * This allows drawing L4, RLE and LZW9 compressed L8 images.
     *
     * @see enableDecompressorL8_L4, enableDecompressorL8_RLE, enableDecompressorL8_LZW9
     */
    void enableDecompressorL8_All();

    /**
     * Enables the decompressor for L8 images compressed with the L4 algorithm.
     * This allows drawing L4 compressed L8 images.
     */
    void enableDecompressorL8_L4();

    /**
     * Enables the decompressor for L8 images compressed with the RLE algorithm.
     * This allows drawing RLE compressed L8 images.
     */
    void enableDecompressorL8_RLE();

    /**
     * Enables the decompressor for L8 images compressed with the LZW9 algorithm.
     * This allows drawing LZW9 compressed L8 images.
     */
    void enableDecompressorL8_LZW9();

    /**
     * Enables the decompressor for RGB images compressed with the QOI algorithm.
     * This allows drawing compressed RGB images.
     */
    void enableDecompressorRGB();

protected:
    virtual DrawTextureMapScanLineBase* getTextureMapperDrawScanLine(const TextureSurface& texture, RenderingVariant renderVariant, uint8_t alpha);

    FlashDataReader& flashReader; ///< Flash reader. Used by routines to read pixel data from the flash.

    /**
     * Find out how much to advance in the display buffer to get to the next pixel.
     *
     * @param  rotatedDisplay Is the display running in portrait mode?
     * @param  textRotation   Rotation to perform.
     *
     * @return How much to advance to get to the next pixel.
     */
    static int nextPixel(bool rotatedDisplay, TextRotation textRotation);

    /**
     * Find out how much to advance in the display buffer to get to the next line.
     *
     * @param  rotatedDisplay Is the display running in portrait mode?
     * @param  textRotation   Rotation to perform.
     *
     * @return How much to advance to get to the next line.
     */
    static int nextLine(bool rotatedDisplay, TextRotation textRotation);

    virtual void drawGlyph(uint16_t* wbuf16, Rect widgetArea, int16_t x, int16_t y, uint16_t offsetX, uint16_t offsetY, const Rect& invalidatedArea, const GlyphNode* glyph, const uint8_t* glyphData, uint8_t byteAlignRow, colortype color, uint8_t bitsPerPixel, uint8_t alpha, TextRotation rotation);

    /**
     * Blits a 2D source-array to the framebuffer performing alpha-blending per pixel as specified.
     *
     * @param   sourceData  The source-array pointer (points to the beginning of the data). The
     *                      sourceData must be stored as 32 bits ARGB8888 values.
     * @param   source      The location and dimensions of the source.
     * @param   blitRect    A rectangle describing what region is to be drawn.
     * @param   alpha       The alpha value to use for blending applied to the whole image (255 =
     *                      solid, no blending)
     */
    void blitCopyARGB8888(const uint32_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * Blits a 2D source-array to the framebuffer never performing alpha-blending per pixel as
     * because it is assumed that all pixels in the bitmap are solid (i.e. alpha for each pixel is
     * 255).
     *
     * @param   sourceData  The source-array pointer (points to the beginning of the data). The
     *                      sourceData must be stored as 32 bits ARGB8888 values.
     * @param   source      The location and dimensions of the source.
     * @param   blitRect    A rectangle describing what region is to be drawn.
     */
    void blitCopyARGB8888Solid(const uint32_t* sourceData, const Rect& source, const Rect& blitRect);

    /**
     * Blits a 2D source-array to the framebuffer performing alpha-blending per pixel as specified.
     * If ARGB8888 is not supported by the DMA a software blend is performed.
     *
     * @param   sourceData  The source-array pointer (points to the beginning of the data). The
     *                      sourceData must be stored as 24 bits ARGB8888 values.
     * @param   source      The location and dimensions of the source.
     * @param   blitRect    A rectangle describing what region is to be drawn.
     * @param   alpha       The alpha value to use for blending applied to the whole image (255 =
     *                      solid, no blending)
     */
    void blitCopyRGB888(const uint8_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * Blits a 2D indexed 8-bit source to the framebuffer performing alpha-blending per
     * pixel as specified if indexed format is not supported by the DMA a software blend is
     * performed.
     *
     * @param  sourceData The source-indexes pointer (points to the beginning of the data). The
     *                    sourceData must be stored as 8- bits indexes.
     * @param  clutData   The source-clut pointer (points to the beginning of the CLUT color
     *                    format and size data followed by colors entries.
     * @param  source     The location and dimensions of the source.
     * @param  blitRect   A rectangle describing what region is to be drawn.
     * @param  alpha      The alpha value to use for blending applied to the whole image (255 =
     *                    solid, no blending)
     */
    void blitCopyL8(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * Blits a 2D indexed 8-bit source to the framebuffer performing alpha-blending per
     * pixel as specified if L8_ARGB8888 is not supported by the DMA a software blend is
     * performed.
     *
     * @param  sourceData The source-indexes pointer (points to the beginning of the data). The
     *                    sourceData must be stored as 8- bits indexes.
     * @param  clutData   The source-clut pointer (points to the beginning of the CLUT color
     *                    format and size data followed by colors entries stored as 32-
     *                    bits (ARGB8888) format.
     * @param  source     The location and dimensions of the source.
     * @param  blitRect   A rectangle describing what region is to be drawn.
     * @param  alpha      The alpha value to use for blending applied to the whole image (255 =
     *                    solid, no blending)
     */
    void blitCopyL8_ARGB8888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    /**
     * Blits a 2D indexed 8-bit source to the framebuffer performing alpha-blending per
     * pixel as specified if L8_RGB565 is not supported by the DMA a software blend is
     * performed.
     *
     * @param  sourceData The source-indexes pointer (points to the beginning of the data). The
     *                    sourceData must be stored as 8- bits indexes.
     * @param  clutData   The source-clut pointer points to the beginning of the CLUT color
     *                    format and size data followed by colors entries stored as 16-
     *                    bits (RGB565) format. If the source have per pixel alpha
     *                    channel, then alpha channel data will be following the clut
     *                    entries data.
     * @param  source     The location and dimensions of the source.
     * @param  blitRect   A rectangle describing what region is to be drawn.
     * @param  alpha      The alpha value to use for blending applied to the whole image (255 =
     *                    solid, no blending)
     */
    void blitCopyL8_RGB565(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

private:
    DrawTextureMapScanLineBase* textureMapper_L8_RGB565_Opaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB565_Opaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB565_Opaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB565_Opaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB888_Opaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB888_Opaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB888_Opaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_RGB888_Opaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_L8_ARGB8888_NonOpaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_ARGB8888_NonOpaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_L8_ARGB8888_NonOpaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_L8_ARGB8888_NonOpaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_NonOpaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_NonOpaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_Opaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_Opaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_NonOpaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_NonOpaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_Opaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_RGB565_Opaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_ARGB8888_NonOpaque_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_ARGB8888_NonOpaque_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_ARGB8888_NonOpaque_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_ARGB8888_NonOpaque_BilinearInterpolation_NoGA;
    DrawTextureMapScanLineBase* textureMapper_A4_NearestNeighbor_GA;
    DrawTextureMapScanLineBase* textureMapper_A4_NearestNeighbor_NoGA;
    DrawTextureMapScanLineBase* textureMapper_A4_BilinearInterpolation_GA;
    DrawTextureMapScanLineBase* textureMapper_A4_BilinearInterpolation_NoGA;

    FORCE_INLINE_FUNCTION static uint32_t expandRgb565(uint16_t c)
    {
        return ((c & 0x07E0) << 16) | (c & ~0x07E0);
    }

    FORCE_INLINE_FUNCTION static uint16_t compactRgb565(uint32_t c)
    {
        return ((c >> 16) & 0x07E0) | (c & ~0x07E0);
    }

    FORCE_INLINE_FUNCTION static uint16_t bilinearInterpolate565(uint16_t c00, uint16_t c10, uint16_t c01, uint16_t c11, uint8_t x, uint8_t y)
    {
        assert(x < 16 && y < 16);
        uint32_t a00 = expandRgb565(c00);
        uint32_t a10 = expandRgb565(c10);
        uint32_t a01 = expandRgb565(c01);
        uint32_t a11 = expandRgb565(c11);

        uint8_t xy = (x * y) >> 3;
        return compactRgb565((a00 * (32 - 2 * y - 2 * x + xy) + a10 * (2 * x - xy) + a01 * (2 * y - xy) + a11 * xy) >> 5);
    }

    FORCE_INLINE_FUNCTION static uint16_t bilinearInterpolate565(uint16_t c00, uint16_t c10, uint8_t x)
    {
        assert(x < 16);
        uint32_t a00 = expandRgb565(c00);
        uint32_t a10 = expandRgb565(c10);

        return compactRgb565((a00 * (32 - 2 * x) + a10 * (2 * x)) >> 5);
    }

    FORCE_INLINE_FUNCTION static uint8_t bilinearInterpolate8(uint8_t c00, uint8_t c10, uint8_t x)
    {
        assert(x < 16);
        uint16_t xy10 = 16 * x;
        uint16_t xy00 = 256 - xy10;

        return (c00 * xy00 + c10 * xy10) >> 8;
    }

    FORCE_INLINE_FUNCTION static uint8_t bilinearInterpolate8(uint8_t c00, uint8_t c10, uint8_t c01, uint8_t c11, uint8_t x, uint8_t y)
    {
        assert(x < 16 && y < 16);
        uint16_t xy11 = x * y;
        uint16_t xy10 = 16 * x - xy11;
        uint16_t xy01 = 16 * y - xy11;
        uint16_t xy00 = 256 - (xy11 + xy10 + xy01);

        return (c00 * xy00 + c10 * xy10 + c01 * xy01 + c11 * xy11) >> 8;
    }

    FORCE_INLINE_FUNCTION static uint32_t bilinearInterpolate888(uint32_t c00, uint32_t c10, uint8_t x)
    {
        assert(x < 16);
        uint16_t xy10 = 16 * x;
        uint16_t xy00 = 256 - xy10;

        return ((((c00 & 0xFF00FF) * xy00 + (c10 & 0xFF00FF) * xy10) >> 8) & 0xFF00FF) |
               ((((c00 & 0x00FF00) * xy00 + (c10 & 0x00FF00) * xy10) >> 8) & 0x00FF00);
    }

    FORCE_INLINE_FUNCTION static uint32_t bilinearInterpolate888(uint32_t c00, uint32_t c10, uint32_t c01, uint32_t c11, uint8_t x, uint8_t y)
    {
        assert(x < 16 && y < 16);
        uint16_t xy11 = x * y;
        uint16_t xy10 = 16 * x - xy11;
        uint16_t xy01 = 16 * y - xy11;
        uint16_t xy00 = 256 - (xy11 + xy10 + xy01);

        return ((((c00 & 0xFF00FF) * xy00 + (c10 & 0xFF00FF) * xy10 + (c01 & 0xFF00FF) * xy01 + (c11 & 0xFF00FF) * xy11) >> 8) & 0xFF00FF) |
               ((((c00 & 0x00FF00) * xy00 + (c10 & 0x00FF00) * xy10 + (c01 & 0x00FF00) * xy01 + (c11 & 0x00FF00) * xy11) >> 8) & 0x00FF00);
    }

    FORCE_INLINE_FUNCTION static uint32_t div255_888(uint32_t val, uint8_t factor)
    {
        return div255rb((val & 0xFF00FF) * factor) | div255g((val & 0x00FF00) * factor);
    }

    FORCE_INLINE_FUNCTION static uint32_t div255_888_FFcheck(uint32_t val, uint8_t factor)
    {
        return factor < 0xFF ? div255_888(val, factor) : val;
    }

    FORCE_INLINE_FUNCTION static uint32_t div31rb(uint16_t val, uint8_t factor)
    {
        uint32_t val32 = (val & 0xF81F) * (factor >> 3);
        return ((val32 + 0x0801 + ((val32 >> 5) & 0xF81F)) >> 5) & 0xF81F;
    }

    FORCE_INLINE_FUNCTION static uint32_t div31g(uint16_t val, uint8_t factor)
    {
        uint32_t val32 = (val & 0x07E0) * factor;
        return ((val32 + 0x0020 + (val32 >> 8)) >> 8) & 0x07E0;
    }

    FORCE_INLINE_FUNCTION static uint32_t div255_565(uint16_t val, uint8_t factor)
    {
        return div31rb(val, factor) | div31g(val, factor);
    }

    FORCE_INLINE_FUNCTION static uint32_t div255_565_FFcheck(uint16_t val, uint8_t factor)
    {
        return factor < 0xFF ? div31rb(val, factor) | div31g(val, factor) : val;
    }

    class DrawTextureMapScanLineBase16 : public DrawTextureMapScanLineBase
    {
    protected:
        FORCE_INLINE_FUNCTION bool overrunCheckNearestNeighbor(uint16_t*& destBits, int& pixelsToDraw, fixed16_16& U, fixed16_16& V, fixed16_16 deltaU, fixed16_16 deltaV, const int16_t maxWidth, const int16_t maxHeight) const;
        FORCE_INLINE_FUNCTION bool overrunCheckBilinearInterpolation(uint16_t*& destBits, int& pixelsToDraw, fixed16_16& U, fixed16_16& V, fixed16_16 deltaU, fixed16_16 deltaV, const int16_t maxWidth, const int16_t maxHeight) const;
    };

    class TextureMapper_L8_RGB565_Opaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_L8_RGB565_Opaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_L8_RGB565_Opaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_L8_RGB565_Opaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint8_t* const textureBits8, const uint16_t* const palette16, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_L8_RGB888_Opaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_L8_RGB888_Opaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_L8_RGB888_Opaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_L8_RGB888_Opaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint8_t* const textureBits8, const uint8_t* const palette8, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_L8_ARGB8888_NonOpaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint32_t* const palette32, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_L8_ARGB8888_NonOpaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint32_t* const palette32, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_L8_ARGB8888_NonOpaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint32_t* const palette32, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint8_t* const textureBits8, const uint32_t* const palette32, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_L8_ARGB8888_NonOpaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const textureBits8, const uint32_t* const palette32, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint8_t* const textureBits8, const uint32_t* const palette32, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_RGB565_NonOpaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint16_t* const textureBits, const uint8_t* const alphaBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_RGB565_NonOpaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint16_t* const textureBits, const uint8_t* alphaBits, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_RGB565_Opaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_RGB565_Opaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_RGB565_NonOpaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint16_t* const textureBits, const uint8_t* const alphaBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint16_t* const textureBits, const uint8_t* const alphaBits, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_RGB565_NonOpaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t* const alphaBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint8_t* const alphaBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_RGB565_Opaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_RGB565_Opaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_ARGB8888_NonOpaque_NearestNeighbor_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint32_t* const textureBits32, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t alpha) const;
    };

    class TextureMapper_ARGB8888_NonOpaque_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint32_t* const textureBits32, const int16_t bitmapWidth, const int UInt, const int VInt) const;
    };

    class TextureMapper_ARGB8888_NonOpaque_BilinearInterpolation_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint32_t* const textureBits32, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint32_t* const textureBits32, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_ARGB8888_NonOpaque_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint32_t* const textureBits32, const int16_t bitmapWidth, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint32_t* const textureBits32, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class TextureMapper_A4_NearestNeighbor_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t a4, const uint8_t alpha) const;
    };

    class TextureMapper_A4_NearestNeighbor_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint8_t a) const;
    };

    class TextureMapper_A4_BilinearInterpolation_GA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint16_t* const textureBits, const uint32_t offset, const int16_t bitmapWidth, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapStride, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac, const uint8_t alpha) const;
    };

    class TextureMapper_A4_BilinearInterpolation_NoGA : public DrawTextureMapScanLineBase16
    {
    public:
        virtual void drawTextureMapScanLineSubdivisions(int subdivisions, const int widthModLength, int pixelsToDraw, const int affineLength, float oneOverZRight, float UOverZRight, float VOverZRight, fixed16_16 U, fixed16_16 V, fixed16_16 deltaU, fixed16_16 deltaV, float ULeft, float VLeft, float URight, float VRight, float ZRight, const DrawingSurface& dest, const int destX, const int destY, const TextureSurface& texture, uint8_t alpha, const float dOneOverZdXAff, const float dUOverZdXAff, const float dVOverZdXAff);

    private:
        FORCE_INLINE_FUNCTION void writePixel(uint16_t* const destBits, const uint16_t* const textureBits, const uint32_t offset, const int16_t bitmapWidth, const uint8_t UFrac, const uint8_t VFrac) const;
        void writePixelOnEdge(uint16_t* const destBits, const uint16_t* const textureBits, const int16_t bitmapStride, const int16_t bitmapWidth, const int16_t bitmapHeight, const int UInt, const int VInt, const uint8_t UFrac, const uint8_t VFrac) const;
    };

    class FlashDataProvider
    {
    public:
        FlashDataProvider(FlashDataReader& _flashReader);

        void initialize(const uint8_t* _sourceData, uint32_t _maxBytesToRead);
        bool addressIsAddressable(const void* address) const;
        void increment();

        uint8_t getCurrent() const
        {
            return *currentBytePtr;
        }

    private:
        void readFromFlash();

        FlashDataReader& flashReader;
        const uint8_t* sourceData;
        uint32_t maxBytesToRead;
        uint32_t bytesToRead;
        const uint8_t* bufferEnd;
        const uint8_t* currentBytePtr;
        bool isAddressable;

        static const uint16_t MAX_BYTES_TO_READ = 64;
    };

    class DecompressorL8Base
    {
    public:
        virtual ~DecompressorL8Base()
        {
        }

        virtual void blitCopyRGB565(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha) = 0;
        virtual void blitCopyARGB8888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha) = 0;
    };

    class DecompressorRGBBase
    {
    public:
        virtual ~DecompressorRGBBase()
        {
        }

        virtual void blitCopyRGB565(const uint8_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha) = 0;
        virtual void blitCopyARGB8888(const uint8_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha) = 0;
    };

    class DecompressorL8_L4 : public DecompressorL8Base
    {
    public:
        DecompressorL8_L4(FlashDataReader& _flashReader)
            : flashReader(_flashReader)
        {
        }

        virtual void blitCopyRGB565(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);
        virtual void blitCopyARGB8888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    private:
        uint16_t blendPixelRGB565(uint16_t imagePixel, uint16_t fbPixel, int alpha, int ialpha) const;
        uint16_t blendPixelARGB8888(uint32_t imagePixel, uint16_t fbPixel, int alpha, uint8_t alphapix) const;
        FlashDataReader& flashReader;
    };

    class DecompressorL8_RLE : public DecompressorL8Base
    {
    public:
        DecompressorL8_RLE(FlashDataReader& _flashReader)
            : flashDataProvider(_flashReader)
        {
        }

        virtual void blitCopyRGB565(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);
        virtual void blitCopyARGB8888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    private:
        FlashDataProvider flashDataProvider;
        static const uint16_t BLOCK_SIZE = 1024U;
    };

    class DecompressorL8_LZW9 : public DecompressorL8Base
    {
    public:
        DecompressorL8_LZW9(FlashDataReader& _flashReader)
            : flashDataProvider(_flashReader)
        {
        }

        virtual void blitCopyRGB565(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);
        virtual void blitCopyARGB8888(const uint8_t* sourceData, const uint8_t* clutData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    private:
        FlashDataProvider flashDataProvider;
        static const uint16_t MAX_DICT_SIZE = 512U;
        static const uint16_t BLOCK_SIZE = 1024U;

        struct LZW9DictionaryEntry
        {
            uint8_t character;    // Current character of the entry
            uint8_t length;       // Remaining length of the entry
            uint16_t prefixIndex; // Index to previous character
        };

        LZW9DictionaryEntry lzw9Dictionary[MAX_DICT_SIZE];
        uint8_t entryBuffer[64]; // Having 64byte max entry sizes entails 1 + 2 + ... + 63 + 64 = 2080px size image blocks
    };

    DecompressorL8Base* decompressorL8_L4;
    DecompressorL8Base* decompressorL8_RLE;
    DecompressorL8Base* decompressorL8_LZW9;

    class DecompressorRGB_QOI : public DecompressorRGBBase
    {
    public:
        DecompressorRGB_QOI(FlashDataReader& _flashReader)
            : flashDataProvider(_flashReader)
        {
        }

        virtual void blitCopyRGB565(const uint8_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha);
        virtual void blitCopyARGB8888(const uint8_t* sourceData, const Rect& source, const Rect& blitRect, uint8_t alpha);

    private:
        FlashDataProvider flashDataProvider;
        static const uint16_t BLOCK_SIZE = 1024U;
        static const uint16_t INDEX_TABLE_SIZE = 64U;

        union
        {
            PixelRGB565 indexTable16[INDEX_TABLE_SIZE];
            PixelARGB8888 indexTable32[INDEX_TABLE_SIZE];
        };

        PixelRGB565 uniqueColorsBuffer[INDEX_TABLE_SIZE];
    };

    DecompressorRGBBase* decompressorRGB_QOI;
};

} // namespace touchgfx

#endif // TOUCHGFX_LCD16BPPSERIALFLASH_HPP
