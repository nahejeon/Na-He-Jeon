/*
 * File: gbufferedimage.cpp
 * ------------------------
 * This file implements the gbufferedimage.h interface.
 * See that file for documentation of each member.
 *
 * author: Marty Stepp
 * version: 2014/10/08
 * - removed 'using namespace' statement
 * - added inBounds method, private resize, zero-arg ctor; slight refactor
 * since: 2014/08/04
 */

#include "gbufferedimage.h"
#include <iomanip>
#include "gwindow.h"
#include "platform.h"

static Platform* pp = getPlatform();

GBufferedImage::GBufferedImage() {
    init(0, 0, 0, 0, 0x000000);
}

GBufferedImage::GBufferedImage(double width, double height) {
    init(0, 0, width, height, 0x000000);
}

GBufferedImage::GBufferedImage(double x, double y, double width, double height,
                               int rgbBackground) {
    init(x, y, width, height, rgbBackground);
}

GBufferedImage::GBufferedImage(double x, double y, double width, double height,
                               std::string rgbBackground) {
    init(x, y, width, height, convertColorToRGB(rgbBackground));
}

GRectangle GBufferedImage::getBounds() const {
    return GRectangle(x, y, m_width, m_height);
}

std::string GBufferedImage::getType() const {
    return "GBufferedImage";
}

std::string GBufferedImage::toString() const {
    return "GBufferedImage()";
}

void GBufferedImage::clear() {
    fill(m_backgroundColor);
}

void GBufferedImage::fill(int rgb) {
    checkColor("fill", rgb);
    m_pixels.fill(rgb);
    pp->gbufferedimage_fill(this, rgb);
}

void GBufferedImage::fill(std::string rgb) {
    fill(convertColorToRGB(rgb));
}

void GBufferedImage::fillRegion(double x, double y, double width, double height, int rgb) {
    checkIndex("fillRegion", x, y);
    checkIndex("fillRegion", x + width - 1, y + height - 1);
    checkColor("fillRegion", rgb);
    for (int r = (int) y; r < y + height; r++) {
        for (int c = (int) x; c < x + width; c++) {
            m_pixels[r][c] = rgb;
        }
    }
    pp->gbufferedimage_fillRegion(this, x, y, width, height, rgb);
}

void GBufferedImage::fillRegion(double x, double y, double width, double height, std::string rgb) {
    fillRegion(x, y, width, height, convertColorToRGB(rgb));
}

double GBufferedImage::getHeight() const {
    return m_height;
}

int GBufferedImage::getRGB(double x, double y) const {
    checkIndex("getRGB", x, y);
    return m_pixels[(int) y][(int) x];
}

std::string GBufferedImage::getRGBString(double x, double y) const {
    return convertRGBToColor(getRGB(x, y));
}

double GBufferedImage::getWidth() const {
    return m_width;
}

bool GBufferedImage::inBounds(double x, double y) const {
    return m_pixels.inBounds((int) y, (int) x);
}

void GBufferedImage::resize(double width, double height, bool retain) {
    this->m_width = width;
    this->m_height = height;
    pp->gbufferedimage_resize(this, width, height, retain);
    this->m_pixels.resize((int) this->m_height, (int) this->m_width, retain);
    if (!retain && m_backgroundColor != 0x0) {
        this->m_pixels.fill(m_backgroundColor);
    }
}

void GBufferedImage::setRGB(double x, double y, int rgb) {
    checkIndex("setRGB", x, y);
    checkColor("setRGB", rgb);
    m_pixels[(int) y][(int) x] = rgb;
    pp->gbufferedimage_setRGB(this, x, y, rgb);
}

void GBufferedImage::setRGB(double x, double y, std::string rgb) {
    setRGB(x, y, convertColorToRGB(rgb));
}

void GBufferedImage::checkColor(std::string member, int rgb) const {
    if (rgb < 0x0 || rgb > 0xffffff) {
        error("GBufferedImage::" + member
              + ": color is outside of range 0x000000 through 0xffffff");
    }
}

void GBufferedImage::checkIndex(std::string member, double x, double y) const {
    if (!inBounds(x, y)) {
        error("GBufferedImage::" + member
              + ": (x=" + integerToString((int) x)
              + ", y=" + integerToString((int) y)
              + " is out of valid range of (0, 0) through ("
              + integerToString((int) m_width) + ", "
              + integerToString((int) m_height) + ")");
    }
}

void GBufferedImage::checkSize(std::string member, double width, double height) const {
    if (width < 0 || height < 0) {
        error("GBufferedImage::" + member + ": width/height cannot be negative");
    }
}

void GBufferedImage::init(double x, double y, double width, double height,
                          int rgb) {
    checkSize("constructor", width, height);
    checkColor("constructor", rgb);
    this->x = x;
    this->y = y;
    this->m_width = width;
    this->m_height = height;
    this->m_pixels.resize((int) this->m_height, (int) this->m_width);
    pp->gbufferedimage_constructor(this, x, y, width, height, rgb);

    if (x != 0 || y != 0) {
        setLocation(x, y);
    }
    
    this->m_backgroundColor = rgb;
    if (rgb != 0) {
        fill(rgb);
    }
}
