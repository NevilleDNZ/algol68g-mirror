//! @file rts-plotutils.c
//! @author J. Marcel van der Veer
//!
//! @section Copyright
//!
//! This file is part of Algol68G - an Algol 68 compiler-interpreter.
//! Copyright 2001-2023 J. Marcel van der Veer [algol68g@xs4all.nl].
//!
//! @section License
//!
//! This program is free software; you can redistribute it and/or modify it 
//! under the terms of the GNU General Public License as published by the 
//! Free Software Foundation; either version 3 of the License, or 
//! (at your option) any later version.
//!
//! This program is distributed in the hope that it will be useful, but 
//! WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
//! or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
//! more details. You should have received a copy of the GNU General Public 
//! License along with this program. If not, see [http://www.gnu.org/licenses/].

//! @section Synopsis
//!
//! Gnuplot's libplot interface. 

// This file contains the Algol68G interface to libplot. Note that Algol68G is not
// a binding for libplot. When GNU plotutils are not installed then the routines in
// this file will give a runtime error when called. You can also choose to not
// define them in "prelude.c". 

#include "a68g.h"
#include "a68g-genie.h"
#include "a68g-numbers.h"
#include "a68g-prelude.h"

#if defined (HAVE_GNU_PLOTUTILS)

// This part contains names for 24-bit colours recognised by libplot.
// The table below is based on the "rgb.txt" file distributed with X11R6. 

struct COLOUR_INFO
{
  char *name;
  int red, green, blue;
};

typedef struct COLOUR_INFO colour_info;

#define COLOUR_MAX 65535
#define COLOUR_NAMES 668

const colour_info A68_COLOURS[COLOUR_NAMES + 1] = {
  {"aliceblue", 0xf0, 0xf8, 0xff},
  {"aluminium", 0xaa, 0xac, 0xb7},
  {"aluminum", 0xaa, 0xac, 0xb7},
  {"antiquewhite", 0xfa, 0xeb, 0xd7},
  {"antiquewhite1", 0xff, 0xef, 0xdb},
  {"antiquewhite2", 0xee, 0xdf, 0xcc},
  {"antiquewhite3", 0xcd, 0xc0, 0xb0},
  {"antiquewhite4", 0x8b, 0x83, 0x78},
  {"aquamarine", 0x7f, 0xff, 0xd4},
  {"aquamarine1", 0x7f, 0xff, 0xd4},
  {"aquamarine2", 0x76, 0xee, 0xc6},
  {"aquamarine3", 0x66, 0xcd, 0xaa},
  {"aquamarine4", 0x45, 0x8b, 0x74},
  {"azure", 0xf0, 0xff, 0xff},
  {"azure1", 0xf0, 0xff, 0xff},
  {"azure2", 0xe0, 0xee, 0xee},
  {"azure3", 0xc1, 0xcd, 0xcd},
  {"azure4", 0x83, 0x8b, 0x8b},
  {"beige", 0xf5, 0xf5, 0xdc},
  {"bisque", 0xff, 0xe4, 0xc4},
  {"bisque1", 0xff, 0xe4, 0xc4},
  {"bisque2", 0xee, 0xd5, 0xb7},
  {"bisque3", 0xcd, 0xb7, 0x9e},
  {"bisque4", 0x8b, 0x7d, 0x6b},
  {"black", 0x00, 0x00, 0x00},
  {"blanchedalmond", 0xff, 0xeb, 0xcd},
  {"blue", 0x00, 0x00, 0xff},
  {"blue1", 0x00, 0x00, 0xff},
  {"blue2", 0x00, 0x00, 0xee},
  {"blue3", 0x00, 0x00, 0xcd},
  {"blue4", 0x00, 0x00, 0x8b},
  {"blueviolet", 0x8a, 0x2b, 0xe2},
  {"bondi1", 0x02, 0x48, 0x8f},
  {"brown", 0xa5, 0x2a, 0x2a},
  {"brown1", 0xff, 0x40, 0x40},
  {"brown2", 0xee, 0x3b, 0x3b},
  {"brown3", 0xcd, 0x33, 0x33},
  {"brown4", 0x8b, 0x23, 0x23},
  {"burlywood", 0xde, 0xb8, 0x87},
  {"burlywood1", 0xff, 0xd3, 0x9b},
  {"burlywood2", 0xee, 0xc5, 0x91},
  {"burlywood3", 0xcd, 0xaa, 0x7d},
  {"burlywood4", 0x8b, 0x73, 0x55},
  {"cadetblue", 0x5f, 0x9e, 0xa0},
  {"cadetblue1", 0x98, 0xf5, 0xff},
  {"cadetblue2", 0x8e, 0xe5, 0xee},
  {"cadetblue3", 0x7a, 0xc5, 0xcd},
  {"cadetblue4", 0x53, 0x86, 0x8b},
  {"chartreuse", 0x7f, 0xff, 0x00},
  {"chartreuse1", 0x7f, 0xff, 0x00},
  {"chartreuse2", 0x76, 0xee, 0x00},
  {"chartreuse3", 0x66, 0xcd, 0x00},
  {"chartreuse4", 0x45, 0x8b, 0x00},
  {"chocolate", 0xd2, 0x69, 0x1e},
  {"chocolate1", 0xff, 0x7f, 0x24},
  {"chocolate2", 0xee, 0x76, 0x21},
  {"chocolate3", 0xcd, 0x66, 0x1d},
  {"chocolate4", 0x8b, 0x45, 0x13},
  {"coral", 0xff, 0x7f, 0x50},
  {"coral1", 0xff, 0x72, 0x56},
  {"coral2", 0xee, 0x6a, 0x50},
  {"coral3", 0xcd, 0x5b, 0x45},
  {"coral4", 0x8b, 0x3e, 0x2f},
  {"cornflowerblue", 0x64, 0x95, 0xed},
  {"cornsilk", 0xff, 0xf8, 0xdc},
  {"cornsilk1", 0xff, 0xf8, 0xdc},
  {"cornsilk2", 0xee, 0xe8, 0xcd},
  {"cornsilk3", 0xcd, 0xc8, 0xb1},
  {"cornsilk4", 0x8b, 0x88, 0x78},
  {"cyan", 0x00, 0xff, 0xff},
  {"cyan1", 0x00, 0xff, 0xff},
  {"cyan2", 0x00, 0xee, 0xee},
  {"cyan3", 0x00, 0xcd, 0xcd},
  {"cyan4", 0x00, 0x8b, 0x8b},
  {"darkblue", 0x00, 0x00, 0x8b},
  {"darkcyan", 0x00, 0x8b, 0x8b},
  {"darkgoldenrod", 0xb8, 0x86, 0x0b},
  {"darkgoldenrod1", 0xff, 0xb9, 0x0f},
  {"darkgoldenrod2", 0xee, 0xad, 0x0e},
  {"darkgoldenrod3", 0xcd, 0x95, 0x0c},
  {"darkgoldenrod4", 0x8b, 0x65, 0x08},
  {"darkgray", 0xa9, 0xa9, 0xa9},
  {"darkgreen", 0x00, 0x64, 0x00},
  {"darkgrey", 0xa9, 0xa9, 0xa9},
  {"darkkhaki", 0xbd, 0xb7, 0x6b},
  {"darkmagenta", 0x8b, 0x00, 0x8b},
  {"darkolivegreen", 0x55, 0x6b, 0x2f},
  {"darkolivegreen1", 0xca, 0xff, 0x70},
  {"darkolivegreen2", 0xbc, 0xee, 0x68},
  {"darkolivegreen3", 0xa2, 0xcd, 0x5a},
  {"darkolivegreen4", 0x6e, 0x8b, 0x3d},
  {"darkorange", 0xff, 0x8c, 0x00},
  {"darkorange1", 0xff, 0x7f, 0x00},
  {"darkorange2", 0xee, 0x76, 0x00},
  {"darkorange3", 0xcd, 0x66, 0x00},
  {"darkorange4", 0x8b, 0x45, 0x00},
  {"darkorchid", 0x99, 0x32, 0xcc},
  {"darkorchid1", 0xbf, 0x3e, 0xff},
  {"darkorchid2", 0xb2, 0x3a, 0xee},
  {"darkorchid3", 0x9a, 0x32, 0xcd},
  {"darkorchid4", 0x68, 0x22, 0x8b},
  {"darkred", 0x8b, 0x00, 0x00},
  {"darksalmon", 0xe9, 0x96, 0x7a},
  {"darkseagreen", 0x8f, 0xbc, 0x8f},
  {"darkseagreen1", 0xc1, 0xff, 0xc1},
  {"darkseagreen2", 0xb4, 0xee, 0xb4},
  {"darkseagreen3", 0x9b, 0xcd, 0x9b},
  {"darkseagreen4", 0x69, 0x8b, 0x69},
  {"darkslateblue", 0x48, 0x3d, 0x8b},
  {"darkslategray", 0x2f, 0x4f, 0x4f},
  {"darkslategray1", 0x97, 0xff, 0xff},
  {"darkslategray2", 0x8d, 0xee, 0xee},
  {"darkslategray3", 0x79, 0xcd, 0xcd},
  {"darkslategray4", 0x52, 0x8b, 0x8b},
  {"darkslategrey", 0x2f, 0x4f, 0x4f},
  {"darkslategrey1", 0x97, 0xff, 0xff},
  {"darkslategrey2", 0x8d, 0xee, 0xee},
  {"darkslategrey3", 0x79, 0xcd, 0xcd},
  {"darkslategrey4", 0x52, 0x8b, 0x8b},
  {"darkturquoise", 0x00, 0xce, 0xd1},
  {"darkviolet", 0x94, 0x00, 0xd3},
  {"deeppink", 0xff, 0x14, 0x93},
  {"deeppink1", 0xff, 0x14, 0x93},
  {"deeppink2", 0xee, 0x12, 0x89},
  {"deeppink3", 0xcd, 0x10, 0x76},
  {"deeppink4", 0x8b, 0x0a, 0x50},
  {"deepskyblue", 0x00, 0xbf, 0xff},
  {"deepskyblue1", 0x00, 0xbf, 0xff},
  {"deepskyblue2", 0x00, 0xb2, 0xee},
  {"deepskyblue3", 0x00, 0x9a, 0xcd},
  {"deepskyblue4", 0x00, 0x68, 0x8b},
  {"dimgray", 0x69, 0x69, 0x69},
  {"dimgrey", 0x69, 0x69, 0x69},
  {"dodgerblue", 0x1e, 0x90, 0xff},
  {"dodgerblue1", 0x1e, 0x90, 0xff},
  {"dodgerblue2", 0x1c, 0x86, 0xee},
  {"dodgerblue3", 0x18, 0x74, 0xcd},
  {"dodgerblue4", 0x10, 0x4e, 0x8b},
  {"firebrick", 0xb2, 0x22, 0x22},
  {"firebrick1", 0xff, 0x30, 0x30},
  {"firebrick2", 0xee, 0x2c, 0x2c},
  {"firebrick3", 0xcd, 0x26, 0x26},
  {"firebrick4", 0x8b, 0x1a, 0x1a},
  {"floralwhite", 0xff, 0xfa, 0xf0},
  {"forestgreen", 0x22, 0x8b, 0x22},
  {"gainsboro", 0xdc, 0xdc, 0xdc},
  {"ghostwhite", 0xf8, 0xf8, 0xff},
  {"gold", 0xff, 0xd7, 0x00},
  {"gold1", 0xff, 0xd7, 0x00},
  {"gold2", 0xee, 0xc9, 0x00},
  {"gold3", 0xcd, 0xad, 0x00},
  {"gold4", 0x8b, 0x75, 0x00},
  {"goldenrod", 0xda, 0xa5, 0x20},
  {"goldenrod1", 0xff, 0xc1, 0x25},
  {"goldenrod2", 0xee, 0xb4, 0x22},
  {"goldenrod3", 0xcd, 0x9b, 0x1d},
  {"goldenrod4", 0x8b, 0x69, 0x14},
  {"gray", 0xbe, 0xbe, 0xbe},
  {"gray0", 0x00, 0x00, 0x00},
  {"gray1", 0x03, 0x03, 0x03},
  {"gray2", 0x05, 0x05, 0x05},
  {"gray3", 0x08, 0x08, 0x08},
  {"gray4", 0x0a, 0x0a, 0x0a},
  {"gray5", 0x0d, 0x0d, 0x0d},
  {"gray6", 0x0f, 0x0f, 0x0f},
  {"gray7", 0x12, 0x12, 0x12},
  {"gray8", 0x14, 0x14, 0x14},
  {"gray9", 0x17, 0x17, 0x17},
  {"gray10", 0x1a, 0x1a, 0x1a},
  {"gray11", 0x1c, 0x1c, 0x1c},
  {"gray12", 0x1f, 0x1f, 0x1f},
  {"gray13", 0x21, 0x21, 0x21},
  {"gray14", 0x24, 0x24, 0x24},
  {"gray15", 0x26, 0x26, 0x26},
  {"gray16", 0x29, 0x29, 0x29},
  {"gray17", 0x2b, 0x2b, 0x2b},
  {"gray18", 0x2e, 0x2e, 0x2e},
  {"gray19", 0x30, 0x30, 0x30},
  {"gray20", 0x33, 0x33, 0x33},
  {"gray21", 0x36, 0x36, 0x36},
  {"gray22", 0x38, 0x38, 0x38},
  {"gray23", 0x3b, 0x3b, 0x3b},
  {"gray24", 0x3d, 0x3d, 0x3d},
  {"gray25", 0x40, 0x40, 0x40},
  {"gray26", 0x42, 0x42, 0x42},
  {"gray27", 0x45, 0x45, 0x45},
  {"gray28", 0x47, 0x47, 0x47},
  {"gray29", 0x4a, 0x4a, 0x4a},
  {"gray30", 0x4d, 0x4d, 0x4d},
  {"gray31", 0x4f, 0x4f, 0x4f},
  {"gray32", 0x52, 0x52, 0x52},
  {"gray33", 0x54, 0x54, 0x54},
  {"gray34", 0x57, 0x57, 0x57},
  {"gray35", 0x59, 0x59, 0x59},
  {"gray36", 0x5c, 0x5c, 0x5c},
  {"gray37", 0x5e, 0x5e, 0x5e},
  {"gray38", 0x61, 0x61, 0x61},
  {"gray39", 0x63, 0x63, 0x63},
  {"gray40", 0x66, 0x66, 0x66},
  {"gray41", 0x69, 0x69, 0x69},
  {"gray42", 0x6b, 0x6b, 0x6b},
  {"gray43", 0x6e, 0x6e, 0x6e},
  {"gray44", 0x70, 0x70, 0x70},
  {"gray45", 0x73, 0x73, 0x73},
  {"gray46", 0x75, 0x75, 0x75},
  {"gray47", 0x78, 0x78, 0x78},
  {"gray48", 0x7a, 0x7a, 0x7a},
  {"gray49", 0x7d, 0x7d, 0x7d},
  {"gray50", 0x7f, 0x7f, 0x7f},
  {"gray51", 0x82, 0x82, 0x82},
  {"gray52", 0x85, 0x85, 0x85},
  {"gray53", 0x87, 0x87, 0x87},
  {"gray54", 0x8a, 0x8a, 0x8a},
  {"gray55", 0x8c, 0x8c, 0x8c},
  {"gray56", 0x8f, 0x8f, 0x8f},
  {"gray57", 0x91, 0x91, 0x91},
  {"gray58", 0x94, 0x94, 0x94},
  {"gray59", 0x96, 0x96, 0x96},
  {"gray60", 0x99, 0x99, 0x99},
  {"gray61", 0x9c, 0x9c, 0x9c},
  {"gray62", 0x9e, 0x9e, 0x9e},
  {"gray63", 0xa1, 0xa1, 0xa1},
  {"gray64", 0xa3, 0xa3, 0xa3},
  {"gray65", 0xa6, 0xa6, 0xa6},
  {"gray66", 0xa8, 0xa8, 0xa8},
  {"gray67", 0xab, 0xab, 0xab},
  {"gray68", 0xad, 0xad, 0xad},
  {"gray69", 0xb0, 0xb0, 0xb0},
  {"gray70", 0xb3, 0xb3, 0xb3},
  {"gray71", 0xb5, 0xb5, 0xb5},
  {"gray72", 0xb8, 0xb8, 0xb8},
  {"gray73", 0xba, 0xba, 0xba},
  {"gray74", 0xbd, 0xbd, 0xbd},
  {"gray75", 0xbf, 0xbf, 0xbf},
  {"gray76", 0xc2, 0xc2, 0xc2},
  {"gray77", 0xc4, 0xc4, 0xc4},
  {"gray78", 0xc7, 0xc7, 0xc7},
  {"gray79", 0xc9, 0xc9, 0xc9},
  {"gray80", 0xcc, 0xcc, 0xcc},
  {"gray81", 0xcf, 0xcf, 0xcf},
  {"gray82", 0xd1, 0xd1, 0xd1},
  {"gray83", 0xd4, 0xd4, 0xd4},
  {"gray84", 0xd6, 0xd6, 0xd6},
  {"gray85", 0xd9, 0xd9, 0xd9},
  {"gray86", 0xdb, 0xdb, 0xdb},
  {"gray87", 0xde, 0xde, 0xde},
  {"gray88", 0xe0, 0xe0, 0xe0},
  {"gray89", 0xe3, 0xe3, 0xe3},
  {"gray90", 0xe5, 0xe5, 0xe5},
  {"gray91", 0xe8, 0xe8, 0xe8},
  {"gray92", 0xeb, 0xeb, 0xeb},
  {"gray93", 0xed, 0xed, 0xed},
  {"gray94", 0xf0, 0xf0, 0xf0},
  {"gray95", 0xf2, 0xf2, 0xf2},
  {"gray96", 0xf5, 0xf5, 0xf5},
  {"gray97", 0xf7, 0xf7, 0xf7},
  {"gray98", 0xfa, 0xfa, 0xfa},
  {"gray99", 0xfc, 0xfc, 0xfc},
  {"gray100", 0xff, 0xff, 0xff},
  {"green", 0x00, 0xff, 0x00},
  {"green1", 0x00, 0xff, 0x00},
  {"green2", 0x00, 0xee, 0x00},
  {"green3", 0x00, 0xcd, 0x00},
  {"green4", 0x00, 0x8b, 0x00},
  {"greenyellow", 0xad, 0xff, 0x2f},
  {"grey", 0xbe, 0xbe, 0xbe},
  {"grey0", 0x00, 0x00, 0x00},
  {"grey1", 0x03, 0x03, 0x03},
  {"grey2", 0x05, 0x05, 0x05},
  {"grey3", 0x08, 0x08, 0x08},
  {"grey4", 0x0a, 0x0a, 0x0a},
  {"grey5", 0x0d, 0x0d, 0x0d},
  {"grey6", 0x0f, 0x0f, 0x0f},
  {"grey7", 0x12, 0x12, 0x12},
  {"grey8", 0x14, 0x14, 0x14},
  {"grey9", 0x17, 0x17, 0x17},
  {"grey10", 0x1a, 0x1a, 0x1a},
  {"grey11", 0x1c, 0x1c, 0x1c},
  {"grey12", 0x1f, 0x1f, 0x1f},
  {"grey13", 0x21, 0x21, 0x21},
  {"grey14", 0x24, 0x24, 0x24},
  {"grey15", 0x26, 0x26, 0x26},
  {"grey16", 0x29, 0x29, 0x29},
  {"grey17", 0x2b, 0x2b, 0x2b},
  {"grey18", 0x2e, 0x2e, 0x2e},
  {"grey19", 0x30, 0x30, 0x30},
  {"grey20", 0x33, 0x33, 0x33},
  {"grey21", 0x36, 0x36, 0x36},
  {"grey22", 0x38, 0x38, 0x38},
  {"grey23", 0x3b, 0x3b, 0x3b},
  {"grey24", 0x3d, 0x3d, 0x3d},
  {"grey25", 0x40, 0x40, 0x40},
  {"grey26", 0x42, 0x42, 0x42},
  {"grey27", 0x45, 0x45, 0x45},
  {"grey28", 0x47, 0x47, 0x47},
  {"grey29", 0x4a, 0x4a, 0x4a},
  {"grey30", 0x4d, 0x4d, 0x4d},
  {"grey31", 0x4f, 0x4f, 0x4f},
  {"grey32", 0x52, 0x52, 0x52},
  {"grey33", 0x54, 0x54, 0x54},
  {"grey34", 0x57, 0x57, 0x57},
  {"grey35", 0x59, 0x59, 0x59},
  {"grey36", 0x5c, 0x5c, 0x5c},
  {"grey37", 0x5e, 0x5e, 0x5e},
  {"grey38", 0x61, 0x61, 0x61},
  {"grey39", 0x63, 0x63, 0x63},
  {"grey40", 0x66, 0x66, 0x66},
  {"grey41", 0x69, 0x69, 0x69},
  {"grey42", 0x6b, 0x6b, 0x6b},
  {"grey43", 0x6e, 0x6e, 0x6e},
  {"grey44", 0x70, 0x70, 0x70},
  {"grey45", 0x73, 0x73, 0x73},
  {"grey46", 0x75, 0x75, 0x75},
  {"grey47", 0x78, 0x78, 0x78},
  {"grey48", 0x7a, 0x7a, 0x7a},
  {"grey49", 0x7d, 0x7d, 0x7d},
  {"grey50", 0x7f, 0x7f, 0x7f},
  {"grey51", 0x82, 0x82, 0x82},
  {"grey52", 0x85, 0x85, 0x85},
  {"grey53", 0x87, 0x87, 0x87},
  {"grey54", 0x8a, 0x8a, 0x8a},
  {"grey55", 0x8c, 0x8c, 0x8c},
  {"grey56", 0x8f, 0x8f, 0x8f},
  {"grey57", 0x91, 0x91, 0x91},
  {"grey58", 0x94, 0x94, 0x94},
  {"grey59", 0x96, 0x96, 0x96},
  {"grey60", 0x99, 0x99, 0x99},
  {"grey61", 0x9c, 0x9c, 0x9c},
  {"grey62", 0x9e, 0x9e, 0x9e},
  {"grey63", 0xa1, 0xa1, 0xa1},
  {"grey64", 0xa3, 0xa3, 0xa3},
  {"grey65", 0xa6, 0xa6, 0xa6},
  {"grey66", 0xa8, 0xa8, 0xa8},
  {"grey67", 0xab, 0xab, 0xab},
  {"grey68", 0xad, 0xad, 0xad},
  {"grey69", 0xb0, 0xb0, 0xb0},
  {"grey70", 0xb3, 0xb3, 0xb3},
  {"grey71", 0xb5, 0xb5, 0xb5},
  {"grey72", 0xb8, 0xb8, 0xb8},
  {"grey73", 0xba, 0xba, 0xba},
  {"grey74", 0xbd, 0xbd, 0xbd},
  {"grey75", 0xbf, 0xbf, 0xbf},
  {"grey76", 0xc2, 0xc2, 0xc2},
  {"grey77", 0xc4, 0xc4, 0xc4},
  {"grey78", 0xc7, 0xc7, 0xc7},
  {"grey79", 0xc9, 0xc9, 0xc9},
  {"grey80", 0xcc, 0xcc, 0xcc},
  {"grey81", 0xcf, 0xcf, 0xcf},
  {"grey82", 0xd1, 0xd1, 0xd1},
  {"grey83", 0xd4, 0xd4, 0xd4},
  {"grey84", 0xd6, 0xd6, 0xd6},
  {"grey85", 0xd9, 0xd9, 0xd9},
  {"grey86", 0xdb, 0xdb, 0xdb},
  {"grey87", 0xde, 0xde, 0xde},
  {"grey88", 0xe0, 0xe0, 0xe0},
  {"grey89", 0xe3, 0xe3, 0xe3},
  {"grey90", 0xe5, 0xe5, 0xe5},
  {"grey91", 0xe8, 0xe8, 0xe8},
  {"grey92", 0xeb, 0xeb, 0xeb},
  {"grey93", 0xed, 0xed, 0xed},
  {"grey94", 0xf0, 0xf0, 0xf0},
  {"grey95", 0xf2, 0xf2, 0xf2},
  {"grey96", 0xf5, 0xf5, 0xf5},
  {"grey97", 0xf7, 0xf7, 0xf7},
  {"grey98", 0xfa, 0xfa, 0xfa},
  {"grey99", 0xfc, 0xfc, 0xfc},
  {"grey100", 0xff, 0xff, 0xff},
  {"honeydew", 0xf0, 0xff, 0xf0},
  {"honeydew1", 0xf0, 0xff, 0xf0},
  {"honeydew2", 0xe0, 0xee, 0xe0},
  {"honeydew3", 0xc1, 0xcd, 0xc1},
  {"honeydew4", 0x83, 0x8b, 0x83},
  {"hotpink", 0xff, 0x69, 0xb4},
  {"hotpink1", 0xff, 0x6e, 0xb4},
  {"hotpink2", 0xee, 0x6a, 0xa7},
  {"hotpink3", 0xcd, 0x60, 0x90},
  {"hotpink4", 0x8b, 0x3a, 0x62},
  {"indianred", 0xcd, 0x5c, 0x5c},
  {"indianred1", 0xff, 0x6a, 0x6a},
  {"indianred2", 0xee, 0x63, 0x63},
  {"indianred3", 0xcd, 0x55, 0x55},
  {"indianred4", 0x8b, 0x3a, 0x3a},
  {"ivory", 0xff, 0xff, 0xf0},
  {"ivory1", 0xff, 0xff, 0xf0},
  {"ivory2", 0xee, 0xee, 0xe0},
  {"ivory3", 0xcd, 0xcd, 0xc1},
  {"ivory4", 0x8b, 0x8b, 0x83},
  {"khaki", 0xf0, 0xe6, 0x8c},
  {"khaki1", 0xff, 0xf6, 0x8f},
  {"khaki2", 0xee, 0xe6, 0x85},
  {"khaki3", 0xcd, 0xc6, 0x73},
  {"khaki4", 0x8b, 0x86, 0x4e},
  {"lavender", 0xe6, 0xe6, 0xfa},
  {"lavenderblush", 0xff, 0xf0, 0xf5},
  {"lavenderblush1", 0xff, 0xf0, 0xf5},
  {"lavenderblush2", 0xee, 0xe0, 0xe5},
  {"lavenderblush3", 0xcd, 0xc1, 0xc5},
  {"lavenderblush4", 0x8b, 0x83, 0x86},
  {"lawngreen", 0x7c, 0xfc, 0x00},
  {"lemonchiffon", 0xff, 0xfa, 0xcd},
  {"lemonchiffon1", 0xff, 0xfa, 0xcd},
  {"lemonchiffon2", 0xee, 0xe9, 0xbf},
  {"lemonchiffon3", 0xcd, 0xc9, 0xa5},
  {"lemonchiffon4", 0x8b, 0x89, 0x70},
  {"lightblue", 0xad, 0xd8, 0xe6},
  {"lightblue1", 0xbf, 0xef, 0xff},
  {"lightblue2", 0xb2, 0xdf, 0xee},
  {"lightblue3", 0x9a, 0xc0, 0xcd},
  {"lightblue4", 0x68, 0x83, 0x8b},
  {"lightcoral", 0xf0, 0x80, 0x80},
  {"lightcyan", 0xe0, 0xff, 0xff},
  {"lightcyan1", 0xe0, 0xff, 0xff},
  {"lightcyan2", 0xd1, 0xee, 0xee},
  {"lightcyan3", 0xb4, 0xcd, 0xcd},
  {"lightcyan4", 0x7a, 0x8b, 0x8b},
  {"lightgoldenrod", 0xee, 0xdd, 0x82},
  {"lightgoldenrod1", 0xff, 0xec, 0x8b},
  {"lightgoldenrod2", 0xee, 0xdc, 0x82},
  {"lightgoldenrod3", 0xcd, 0xbe, 0x70},
  {"lightgoldenrod4", 0x8b, 0x81, 0x4c},
  {"lightgoldenrodyellow", 0xfa, 0xfa, 0xd2},
  {"lightgray", 0xd3, 0xd3, 0xd3},
  {"lightgreen", 0x90, 0xee, 0x90},
  {"lightgrey", 0xd3, 0xd3, 0xd3},
  {"lightpink", 0xff, 0xb6, 0xc1},
  {"lightpink1", 0xff, 0xae, 0xb9},
  {"lightpink2", 0xee, 0xa2, 0xad},
  {"lightpink3", 0xcd, 0x8c, 0x95},
  {"lightpink4", 0x8b, 0x5f, 0x65},
  {"lightsalmon", 0xff, 0xa0, 0x7a},
  {"lightsalmon1", 0xff, 0xa0, 0x7a},
  {"lightsalmon2", 0xee, 0x95, 0x72},
  {"lightsalmon3", 0xcd, 0x81, 0x62},
  {"lightsalmon4", 0x8b, 0x57, 0x42},
  {"lightseagreen", 0x20, 0xb2, 0xaa},
  {"lightskyblue", 0x87, 0xce, 0xfa},
  {"lightskyblue1", 0xb0, 0xe2, 0xff},
  {"lightskyblue2", 0xa4, 0xd3, 0xee},
  {"lightskyblue3", 0x8d, 0xb6, 0xcd},
  {"lightskyblue4", 0x60, 0x7b, 0x8b},
  {"lightslateblue", 0x84, 0x70, 0xff},
  {"lightslategray", 0x77, 0x88, 0x99},
  {"lightslategrey", 0x77, 0x88, 0x99},
  {"lightsteelblue", 0xb0, 0xc4, 0xde},
  {"lightsteelblue1", 0xca, 0xe1, 0xff},
  {"lightsteelblue2", 0xbc, 0xd2, 0xee},
  {"lightsteelblue3", 0xa2, 0xb5, 0xcd},
  {"lightsteelblue4", 0x6e, 0x7b, 0x8b},
  {"lightyellow", 0xff, 0xff, 0xe0},
  {"lightyellow1", 0xff, 0xff, 0xe0},
  {"lightyellow2", 0xee, 0xee, 0xd1},
  {"lightyellow3", 0xcd, 0xcd, 0xb4},
  {"lightyellow4", 0x8b, 0x8b, 0x7a},
  {"limegreen", 0x32, 0xcd, 0x32},
  {"linen", 0xfa, 0xf0, 0xe6},
  {"magenta", 0xff, 0x00, 0xff},
  {"magenta1", 0xff, 0x00, 0xff},
  {"magenta2", 0xee, 0x00, 0xee},
  {"magenta3", 0xcd, 0x00, 0xcd},
  {"magenta4", 0x8b, 0x00, 0x8b},
  {"maroon", 0xb0, 0x30, 0x60},
  {"maroon1", 0xff, 0x34, 0xb3},
  {"maroon2", 0xee, 0x30, 0xa7},
  {"maroon3", 0xcd, 0x29, 0x90},
  {"maroon4", 0x8b, 0x1c, 0x62},
  {"mediumaquamarine", 0x66, 0xcd, 0xaa},
  {"mediumblue", 0x00, 0x00, 0xcd},
  {"mediumorchid", 0xba, 0x55, 0xd3},
  {"mediumorchid1", 0xe0, 0x66, 0xff},
  {"mediumorchid2", 0xd1, 0x5f, 0xee},
  {"mediumorchid3", 0xb4, 0x52, 0xcd},
  {"mediumorchid4", 0x7a, 0x37, 0x8b},
  {"mediumpurple", 0x93, 0x70, 0xdb},
  {"mediumpurple1", 0xab, 0x82, 0xff},
  {"mediumpurple2", 0x9f, 0x79, 0xee},
  {"mediumpurple3", 0x89, 0x68, 0xcd},
  {"mediumpurple4", 0x5d, 0x47, 0x8b},
  {"mediumseagreen", 0x3c, 0xb3, 0x71},
  {"mediumslateblue", 0x7b, 0x68, 0xee},
  {"mediumspringgreen", 0x00, 0xfa, 0x9a},
  {"mediumturquoise", 0x48, 0xd1, 0xcc},
  {"mediumvioletred", 0xc7, 0x15, 0x85},
  {"midnightblue", 0x19, 0x19, 0x70},
  {"mintcream", 0xf5, 0xff, 0xfa},
  {"mistyrose", 0xff, 0xe4, 0xe1},
  {"mistyrose1", 0xff, 0xe4, 0xe1},
  {"mistyrose2", 0xee, 0xd5, 0xd2},
  {"mistyrose3", 0xcd, 0xb7, 0xb5},
  {"mistyrose4", 0x8b, 0x7d, 0x7b},
  {"moccasin", 0xff, 0xe4, 0xb5},
  {"navajowhite", 0xff, 0xde, 0xad},
  {"navajowhite1", 0xff, 0xde, 0xad},
  {"navajowhite2", 0xee, 0xcf, 0xa1},
  {"navajowhite3", 0xcd, 0xb3, 0x8b},
  {"navajowhite4", 0x8b, 0x79, 0x5e},
  {"navy", 0x00, 0x00, 0x80},
  {"navyblue", 0x00, 0x00, 0x80},
  {"oldlace", 0xfd, 0xf5, 0xe6},
  {"olivedrab", 0x6b, 0x8e, 0x23},
  {"olivedrab1", 0xc0, 0xff, 0x3e},
  {"olivedrab2", 0xb3, 0xee, 0x3a},
  {"olivedrab3", 0x9a, 0xcd, 0x32},
  {"olivedrab4", 0x69, 0x8b, 0x22},
  {"orange", 0xff, 0xa5, 0x00},
  {"orange1", 0xff, 0xa5, 0x00},
  {"orange2", 0xee, 0x9a, 0x00},
  {"orange3", 0xcd, 0x85, 0x00},
  {"orange4", 0x8b, 0x5a, 0x00},
  {"orangered", 0xff, 0x45, 0x00},
  {"orangered1", 0xff, 0x45, 0x00},
  {"orangered2", 0xee, 0x40, 0x00},
  {"orangered3", 0xcd, 0x37, 0x00},
  {"orangered4", 0x8b, 0x25, 0x00},
  {"orchid", 0xda, 0x70, 0xd6},
  {"orchid1", 0xff, 0x83, 0xfa},
  {"orchid2", 0xee, 0x7a, 0xe9},
  {"orchid3", 0xcd, 0x69, 0xc9},
  {"orchid4", 0x8b, 0x47, 0x89},
  {"palegoldenrod", 0xee, 0xe8, 0xaa},
  {"palegreen", 0x98, 0xfb, 0x98},
  {"palegreen1", 0x9a, 0xff, 0x9a},
  {"palegreen2", 0x90, 0xee, 0x90},
  {"palegreen3", 0x7c, 0xcd, 0x7c},
  {"palegreen4", 0x54, 0x8b, 0x54},
  {"paleturquoise", 0xaf, 0xee, 0xee},
  {"paleturquoise1", 0xbb, 0xff, 0xff},
  {"paleturquoise2", 0xae, 0xee, 0xee},
  {"paleturquoise3", 0x96, 0xcd, 0xcd},
  {"paleturquoise4", 0x66, 0x8b, 0x8b},
  {"palevioletred", 0xdb, 0x70, 0x93},
  {"palevioletred1", 0xff, 0x82, 0xab},
  {"palevioletred2", 0xee, 0x79, 0x9f},
  {"palevioletred3", 0xcd, 0x68, 0x89},
  {"palevioletred4", 0x8b, 0x47, 0x5d},
  {"papayawhip", 0xff, 0xef, 0xd5},
  {"peachpuff", 0xff, 0xda, 0xb9},
  {"peachpuff1", 0xff, 0xda, 0xb9},
  {"peachpuff2", 0xee, 0xcb, 0xad},
  {"peachpuff3", 0xcd, 0xaf, 0x95},
  {"peachpuff4", 0x8b, 0x77, 0x65},
  {"peru", 0xcd, 0x85, 0x3f},
  {"pink", 0xff, 0xc0, 0xcb},
  {"pink1", 0xff, 0xb5, 0xc5},
  {"pink2", 0xee, 0xa9, 0xb8},
  {"pink3", 0xcd, 0x91, 0x9e},
  {"pink4", 0x8b, 0x63, 0x6c},
  {"plum", 0xdd, 0xa0, 0xdd},
  {"plum1", 0xff, 0xbb, 0xff},
  {"plum2", 0xee, 0xae, 0xee},
  {"plum3", 0xcd, 0x96, 0xcd},
  {"plum4", 0x8b, 0x66, 0x8b},
  {"powderblue", 0xb0, 0xe0, 0xe6},
  {"purple", 0xa0, 0x20, 0xf0},
  {"purple1", 0x9b, 0x30, 0xff},
  {"purple2", 0x91, 0x2c, 0xee},
  {"purple3", 0x7d, 0x26, 0xcd},
  {"purple4", 0x55, 0x1a, 0x8b},
  {"red", 0xff, 0x00, 0x00},
  {"red1", 0xff, 0x00, 0x00},
  {"red2", 0xee, 0x00, 0x00},
  {"red3", 0xcd, 0x00, 0x00},
  {"red4", 0x8b, 0x00, 0x00},
  {"rosybrown", 0xbc, 0x8f, 0x8f},
  {"rosybrown1", 0xff, 0xc1, 0xc1},
  {"rosybrown2", 0xee, 0xb4, 0xb4},
  {"rosybrown3", 0xcd, 0x9b, 0x9b},
  {"rosybrown4", 0x8b, 0x69, 0x69},
  {"royalblue", 0x41, 0x69, 0xe1},
  {"royalblue1", 0x48, 0x76, 0xff},
  {"royalblue2", 0x43, 0x6e, 0xee},
  {"royalblue3", 0x3a, 0x5f, 0xcd},
  {"royalblue4", 0x27, 0x40, 0x8b},
  {"saddlebrown", 0x8b, 0x45, 0x13},
  {"salmon", 0xfa, 0x80, 0x72},
  {"salmon1", 0xff, 0x8c, 0x69},
  {"salmon2", 0xee, 0x82, 0x62},
  {"salmon3", 0xcd, 0x70, 0x54},
  {"salmon4", 0x8b, 0x4c, 0x39},
  {"sandybrown", 0xf4, 0xa4, 0x60},
  {"seagreen", 0x2e, 0x8b, 0x57},
  {"seagreen1", 0x54, 0xff, 0x9f},
  {"seagreen2", 0x4e, 0xee, 0x94},
  {"seagreen3", 0x43, 0xcd, 0x80},
  {"seagreen4", 0x2e, 0x8b, 0x57},
  {"seashell", 0xff, 0xf5, 0xee},
  {"seashell1", 0xff, 0xf5, 0xee},
  {"seashell2", 0xee, 0xe5, 0xde},
  {"seashell3", 0xcd, 0xc5, 0xbf},
  {"seashell4", 0x8b, 0x86, 0x82},
  {"sienna", 0xa0, 0x52, 0x2d},
  {"sienna1", 0xff, 0x82, 0x47},
  {"sienna2", 0xee, 0x79, 0x42},
  {"sienna3", 0xcd, 0x68, 0x39},
  {"sienna4", 0x8b, 0x47, 0x26},
  {"skyblue", 0x87, 0xce, 0xeb},
  {"skyblue1", 0x87, 0xce, 0xff},
  {"skyblue2", 0x7e, 0xc0, 0xee},
  {"skyblue3", 0x6c, 0xa6, 0xcd},
  {"skyblue4", 0x4a, 0x70, 0x8b},
  {"slateblue", 0x6a, 0x5a, 0xcd},
  {"slateblue1", 0x83, 0x6f, 0xff},
  {"slateblue2", 0x7a, 0x67, 0xee},
  {"slateblue3", 0x69, 0x59, 0xcd},
  {"slateblue4", 0x47, 0x3c, 0x8b},
  {"slategray", 0x70, 0x80, 0x90},
  {"slategray1", 0xc6, 0xe2, 0xff},
  {"slategray2", 0xb9, 0xd3, 0xee},
  {"slategray3", 0x9f, 0xb6, 0xcd},
  {"slategray4", 0x6c, 0x7b, 0x8b},
  {"slategrey", 0x70, 0x80, 0x90},
  {"slategrey1", 0xc6, 0xe2, 0xff},
  {"slategrey2", 0xb9, 0xd3, 0xee},
  {"slategrey3", 0x9f, 0xb6, 0xcd},
  {"slategrey4", 0x6c, 0x7b, 0x8b},
  {"snow", 0xff, 0xfa, 0xfa},
  {"snow1", 0xff, 0xfa, 0xfa},
  {"snow2", 0xee, 0xe9, 0xe9},
  {"snow3", 0xcd, 0xc9, 0xc9},
  {"snow4", 0x8b, 0x89, 0x89},
  {"springgreen", 0x00, 0xff, 0x7f},
  {"springgreen1", 0x00, 0xff, 0x7f},
  {"springgreen2", 0x00, 0xee, 0x76},
  {"springgreen3", 0x00, 0xcd, 0x66},
  {"springgreen4", 0x00, 0x8b, 0x45},
  {"steelblue", 0x46, 0x82, 0xb4},
  {"steelblue1", 0x63, 0xb8, 0xff},
  {"steelblue2", 0x5c, 0xac, 0xee},
  {"steelblue3", 0x4f, 0x94, 0xcd},
  {"steelblue4", 0x36, 0x64, 0x8b},
  {"tan", 0xd2, 0xb4, 0x8c},
  {"tan1", 0xff, 0xa5, 0x4f},
  {"tan2", 0xee, 0x9a, 0x49},
  {"tan3", 0xcd, 0x85, 0x3f},
  {"tan4", 0x8b, 0x5a, 0x2b},
  {"thistle", 0xd8, 0xbf, 0xd8},
  {"thistle1", 0xff, 0xe1, 0xff},
  {"thistle2", 0xee, 0xd2, 0xee},
  {"thistle3", 0xcd, 0xb5, 0xcd},
  {"thistle4", 0x8b, 0x7b, 0x8b},
  {"tomato", 0xff, 0x63, 0x47},
  {"tomato1", 0xff, 0x63, 0x47},
  {"tomato2", 0xee, 0x5c, 0x42},
  {"tomato3", 0xcd, 0x4f, 0x39},
  {"tomato4", 0x8b, 0x36, 0x26},
  {"turquoise", 0x40, 0xe0, 0xd0},
  {"turquoise1", 0x00, 0xf5, 0xff},
  {"turquoise2", 0x00, 0xe5, 0xee},
  {"turquoise3", 0x00, 0xc5, 0xcd},
  {"turquoise4", 0x00, 0x86, 0x8b},
  {"violet", 0xee, 0x82, 0xee},
  {"violetred", 0xd0, 0x20, 0x90},
  {"violetred1", 0xff, 0x3e, 0x96},
  {"violetred2", 0xee, 0x3a, 0x8c},
  {"violetred3", 0xcd, 0x32, 0x78},
  {"violetred4", 0x8b, 0x22, 0x52},
  {"wheat", 0xf5, 0xde, 0xb3},
  {"wheat1", 0xff, 0xe7, 0xba},
  {"wheat2", 0xee, 0xd8, 0xae},
  {"wheat3", 0xcd, 0xba, 0x96},
  {"wheat4", 0x8b, 0x7e, 0x66},
  {"white", 0xff, 0xff, 0xff},
  {"whitesmoke", 0xf5, 0xf5, 0xf5},
  {"yellow", 0xff, 0xff, 0x00},
  {"yellow1", 0xff, 0xff, 0x00},
  {"yellow2", 0xee, 0xee, 0x00},
  {"yellow3", 0xcd, 0xcd, 0x00},
  {"yellow4", 0x8b, 0x8b, 0x00},
  {"yellowgreen", 0x9a, 0xcd, 0x32},
  {NO_TEXT, 0, 0, 0}
};

//! @brief Searches colour in the list.

BOOL_T string_to_colour (NODE_T * p, char *name, int *iindex)
{
  A68_REF z_ref = heap_generator (p, M_C_STRING, (int) (1 + strlen (name)));
  char *z = DEREF (char, &z_ref);
  int i, j;
  BOOL_T k;
// First remove formatting from name: spaces and capitals are irrelevant.
  j = 0;
  for (i = 0; name[i] != NULL_CHAR; i++) {
    if (name[i] != BLANK_CHAR) {
      z[j++] = (char) TO_LOWER (name[i]);
    }
    z[j] = 0;
  }
// Now search with the famous British Library Method.
  k = A68_FALSE;
  for (i = 0; i < COLOUR_NAMES && !k; i++) {
    if (!strcmp (NAME (&A68_COLOURS[i]), z)) {
      k = A68_TRUE;
      *iindex = i;
    }
  }
  return k;
}

//! @brief Scans string for an integer.

BOOL_T scan_int (char **z, int *k)
{
  char *y = *z;
  while (y[0] != NULL_CHAR && !IS_DIGIT (y[0])) {
    y++;
  }
  if (y[0] != NULL_CHAR) {
    (*k) = strtol (y, z, 10);
    return (BOOL_T) (errno == 0);
  } else {
    return A68_FALSE;
  }
}

//! @brief PROC (REF FILE, STRING, STRING) make device

void genie_make_device (NODE_T * p)
{
  int size;
  A68_REF ref_device, ref_page, ref_file;
  A68_FILE *file;
// Pop arguments.
  POP_REF (p, &ref_page);
  POP_REF (p, &ref_device);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  file = FILE_DEREF (&ref_file);
  if (DEVICE_MADE (&DEVICE (file))) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_ALREADY_SET);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
// Fill in page_size.
  size = a68_string_size (p, ref_page);
  if (INITIALISED (&(A68_PAGE_SIZE (&DEVICE (file)))) && !IS_NIL (A68_PAGE_SIZE (&DEVICE (file)))) {
    UNBLOCK_GC_HANDLE (&A68_PAGE_SIZE (&DEVICE (file)));
  }
  A68_PAGE_SIZE (&DEVICE (file)) = heap_generator (p, M_STRING, 1 + size);
  BLOCK_GC_HANDLE (&A68_PAGE_SIZE (&DEVICE (file)));
  ASSERT (a_to_c_string (p, DEREF (char, &A68_PAGE_SIZE (&DEVICE (file))), ref_page) != NO_TEXT);
// Fill in device.
  size = a68_string_size (p, ref_device);
  if (INITIALISED (&(DEVICE (&DEVICE (file)))) && !IS_NIL (DEVICE (&DEVICE (file)))) {
    UNBLOCK_GC_HANDLE (&DEVICE (&DEVICE (file)));
  }
  DEVICE (&DEVICE (file)) = heap_generator (p, M_STRING, 1 + size);
  BLOCK_GC_HANDLE (&DEVICE (&DEVICE (file)));
  ASSERT (a_to_c_string (p, DEREF (char, &DEVICE (&DEVICE (file))), ref_device) != NO_TEXT);
  DEVICE_MADE (&DEVICE (file)) = A68_TRUE;
  PUSH_VALUE (p, A68_TRUE, A68_BOOL);
}

//! @brief Closes the plotter.

BOOL_T close_device (NODE_T * p, A68_FILE * f)
{
  CHECK_INIT (p, INITIALISED (f), M_FILE);
  if (!OPENED (f)) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_FILE_NOT_OPEN);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  if (!(DEVICE_OPENED (&DEVICE (f)))) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_NOT_OPEN);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  if (DEVICE_MADE (&DEVICE (f))) {
    if (!IS_NIL (DEVICE (&DEVICE (f)))) {
      UNBLOCK_GC_HANDLE (&(DEVICE (&DEVICE (f))));
    }
    if (!IS_NIL (A68_PAGE_SIZE (&DEVICE (f)))) {
      UNBLOCK_GC_HANDLE (&(A68_PAGE_SIZE (&DEVICE (f))));
    }
  }
  if (pl_closepl_r (PLOTTER (&DEVICE (f))) < 0) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_CLOSING_DEVICE);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  if (pl_deletepl_r (PLOTTER (&DEVICE (f))) < 0) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_CLOSING_DEVICE);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  if (STREAM (&DEVICE (f)) != NO_STREAM && fclose (STREAM (&DEVICE (f))) != 0) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_CLOSING_FILE);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  DEVICE_OPENED (&DEVICE (f)) = A68_FALSE;
  return A68_TRUE;
}

//! @brief Sets up the plotter prior to using it.

plPlotter *set_up_device (NODE_T * p, A68_FILE * f)
{
  A68_REF ref_filename;
  char *filename, *device_type;
// First set up the general device, then plotter-specific things.
  CHECK_INIT (p, INITIALISED (f), M_FILE);
  ref_filename = IDENTIFICATION (f);
// This one in front as to quickly select the plotter.
  if (DEVICE_OPENED (&DEVICE (f))) {
    if (DEVICE_HANDLE (&DEVICE (f)) < 0) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    return PLOTTER (&DEVICE (f));
  }
// Device not set up yet.
  if (!OPENED (f)) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_FILE_NOT_OPEN);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  if (READ_MOOD (f)) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_FILE_WRONG_MOOD, "read");
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  if (WRITE_MOOD (f)) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_FILE_WRONG_MOOD, "write");
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  if (!DRAW (&CHANNEL (f))) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_CHANNEL_DOES_NOT_ALLOW, "drawing");
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  if (!DEVICE_MADE (&DEVICE (f))) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_NOT_SET);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  device_type = DEREF (char, &DEVICE (&DEVICE (f)));
  if (!strcmp (device_type, "X")) {
#if defined (X_DISPLAY_MISSING)
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_INVALID_PARAMETER, "X plotter missing", "");
    exit_genie (p, A68_RUNTIME_ERROR);
#else
//
// Supported plotter type - X Window System
//
    char *z = DEREF (char, &A68_PAGE_SIZE (&DEVICE (f))), size[BUFFER_SIZE];
// Establish page size.
    if (!scan_int (&z, &(WINDOW_X_SIZE (&DEVICE (f))))) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (!scan_int (&z, &(WINDOW_Y_SIZE (&DEVICE (f))))) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (z[0] != NULL_CHAR) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
// Make the X window.
    FD (f) = -1;
    PLOTTER_PARAMS (&DEVICE (f)) = pl_newplparams ();
    if (PLOTTER_PARAMS (&DEVICE (f)) == NULL) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_ALLOCATE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    ASSERT (snprintf (size, SNPRINTF_SIZE, "%dx%d", WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f))) >= 0);
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "BITMAPSIZE", size);
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "BG_COLOR", (void *) "black");
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "VANISH_ON_DELETE", (void *) "no");
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "X_AUTO_FLUSH", (void *) "yes");
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "USE_DOUBLE_BUFFERING", (void *) "no");
    PLOTTER (&DEVICE (f)) = pl_newpl_r ("X", NULL, NULL, stderr, PLOTTER_PARAMS (&DEVICE (f)));
    if (PLOTTER (&DEVICE (f)) == NULL) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (pl_openpl_r (PLOTTER (&DEVICE (f))) < 0) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    (void) pl_space_r (PLOTTER (&DEVICE (f)), 0, 0, WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f)));
    (void) pl_bgcolorname_r (PLOTTER (&DEVICE (f)), "black");
    (void) pl_colorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_pencolorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_fillcolorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_filltype_r (PLOTTER (&DEVICE (f)), 0);
    DRAW_MOOD (f) = A68_TRUE;
    DEVICE_OPENED (&DEVICE (f)) = A68_TRUE;
    X_COORD (&DEVICE (f)) = 0;
    Y_COORD (&DEVICE (f)) = 0;
    return PLOTTER (&DEVICE (f));
#endif
  } else if (!strcmp (device_type, "gif")) {
//
// Supported plotter type - pseudo GIF
//
    char *z = DEREF (char, &A68_PAGE_SIZE (&DEVICE (f))), size[BUFFER_SIZE];
// Establish page size.
    if (!scan_int (&z, &(WINDOW_X_SIZE (&DEVICE (f))))) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (!scan_int (&z, &(WINDOW_Y_SIZE (&DEVICE (f))))) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (z[0] != NULL_CHAR) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
// Open the output file for drawing.
    CHECK_REF (p, ref_filename, M_ROWS);
    filename = DEREF (char, &ref_filename);
    errno = 0;
    if ((STREAM (&DEVICE (f)) = fopen (filename, "wb")) == NO_STREAM) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_CANNOT_OPEN_NAME, filename);
      exit_genie (p, A68_RUNTIME_ERROR);
    } else {
      READ_MOOD (f) = A68_FALSE;
      WRITE_MOOD (f) = A68_FALSE;
      CHAR_MOOD (f) = A68_FALSE;
      DRAW_MOOD (f) = A68_TRUE;
    }
// Set up plotter.
    PLOTTER_PARAMS (&DEVICE (f)) = pl_newplparams ();
    if (PLOTTER_PARAMS (&DEVICE (f)) == NULL) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_ALLOCATE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    ASSERT (snprintf (size, SNPRINTF_SIZE, "%dx%d", WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f))) >= 0);
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "BITMAPSIZE", size);
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "BG_COLOR", (void *) "black");
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "GIF_ANIMATION", (void *) "no");
    PLOTTER (&DEVICE (f)) = pl_newpl_r ("gif", NULL, STREAM (&DEVICE (f)), stderr, PLOTTER_PARAMS (&DEVICE (f)));
    if (PLOTTER (&DEVICE (f)) == NULL) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (pl_openpl_r (PLOTTER (&DEVICE (f))) < 0) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    (void) pl_space_r (PLOTTER (&DEVICE (f)), 0, 0, WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f)));
    (void) pl_bgcolorname_r (PLOTTER (&DEVICE (f)), "black");
    (void) pl_colorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_pencolorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_fillcolorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_filltype_r (PLOTTER (&DEVICE (f)), 0);
    DRAW_MOOD (f) = A68_TRUE;
    DEVICE_OPENED (&DEVICE (f)) = A68_TRUE;
    X_COORD (&DEVICE (f)) = 0;
    Y_COORD (&DEVICE (f)) = 0;
    return PLOTTER (&DEVICE (f));
  } else if (!strcmp (device_type, "pnm")) {
//
// Supported plotter type - Portable aNyMap
//
    char *z = DEREF (char, &A68_PAGE_SIZE (&DEVICE (f))), size[BUFFER_SIZE];
// Establish page size.
    if (!scan_int (&z, &(WINDOW_X_SIZE (&DEVICE (f))))) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (!scan_int (&z, &(WINDOW_Y_SIZE (&DEVICE (f))))) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (z[0] != NULL_CHAR) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
// Open the output file for drawing.
    CHECK_REF (p, ref_filename, M_ROWS);
    filename = DEREF (char, &ref_filename);
    errno = 0;
    if ((STREAM (&DEVICE (f)) = fopen (filename, "wb")) == NO_STREAM) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_CANNOT_OPEN_NAME, filename);
      exit_genie (p, A68_RUNTIME_ERROR);
    } else {
      READ_MOOD (f) = A68_FALSE;
      WRITE_MOOD (f) = A68_FALSE;
      CHAR_MOOD (f) = A68_FALSE;
      DRAW_MOOD (f) = A68_TRUE;
    }
// Set up plotter.
    ASSERT (snprintf (size, SNPRINTF_SIZE, "%dx%d", WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f))) >= 0);
    PLOTTER_PARAMS (&DEVICE (f)) = pl_newplparams ();
    if (PLOTTER_PARAMS (&DEVICE (f)) == NULL) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_ALLOCATE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "BITMAPSIZE", size);
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "BG_COLOR", (void *) "black");
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "PNM_PORTABLE", (void *) "no");
    PLOTTER (&DEVICE (f)) = pl_newpl_r ("pnm", NULL, STREAM (&DEVICE (f)), stderr, PLOTTER_PARAMS (&DEVICE (f)));
    if (PLOTTER (&DEVICE (f)) == NULL) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (pl_openpl_r (PLOTTER (&DEVICE (f))) < 0) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    (void) pl_space_r (PLOTTER (&DEVICE (f)), 0, 0, WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f)));
    (void) pl_bgcolorname_r (PLOTTER (&DEVICE (f)), "black");
    (void) pl_colorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_pencolorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_fillcolorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_filltype_r (PLOTTER (&DEVICE (f)), 0);
    DRAW_MOOD (f) = A68_TRUE;
    DEVICE_OPENED (&DEVICE (f)) = A68_TRUE;
    X_COORD (&DEVICE (f)) = 0;
    Y_COORD (&DEVICE (f)) = 0;
    return PLOTTER (&DEVICE (f));
  } else if (!strcmp (device_type, "png")) {
#if defined (X_DISPLAY_MISSING)
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_INVALID_PARAMETER, "PNG plotter missing", "");
    exit_genie (p, A68_RUNTIME_ERROR);
#else
//
// Supported plotter type - PNG
//
    char *z = DEREF (char, &A68_PAGE_SIZE (&DEVICE (f))), size[BUFFER_SIZE];
// Establish page size.
    if (!scan_int (&z, &(WINDOW_X_SIZE (&DEVICE (f))))) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (!scan_int (&z, &(WINDOW_Y_SIZE (&DEVICE (f))))) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (z[0] != NULL_CHAR) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_PAGE_SIZE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
// Open the output file for drawing.
    CHECK_REF (p, ref_filename, M_ROWS);
    filename = DEREF (char, &ref_filename);
    errno = 0;
    if ((STREAM (&DEVICE (f)) = fopen (filename, "wb")) == NO_STREAM) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_CANNOT_OPEN_NAME, filename);
      exit_genie (p, A68_RUNTIME_ERROR);
    } else {
      READ_MOOD (f) = A68_FALSE;
      WRITE_MOOD (f) = A68_FALSE;
      CHAR_MOOD (f) = A68_FALSE;
      DRAW_MOOD (f) = A68_TRUE;
    }
// Set up plotter.
    PLOTTER_PARAMS (&DEVICE (f)) = pl_newplparams ();
    if (PLOTTER_PARAMS (&DEVICE (f)) == NULL) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_ALLOCATE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    ASSERT (snprintf (size, SNPRINTF_SIZE, "%dx%d", WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f))) >= 0);
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "BITMAPSIZE", size);
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "BG_COLOR", (void *) "black");
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "GIF_ANIMATION", (void *) "no");
    PLOTTER (&DEVICE (f)) = pl_newpl_r ("png", NULL, STREAM (&DEVICE (f)), stderr, PLOTTER_PARAMS (&DEVICE (f)));
    if (PLOTTER (&DEVICE (f)) == NULL) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (pl_openpl_r (PLOTTER (&DEVICE (f))) < 0) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    (void) pl_space_r (PLOTTER (&DEVICE (f)), 0, 0, WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f)));
    (void) pl_bgcolorname_r (PLOTTER (&DEVICE (f)), "black");
    (void) pl_colorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_pencolorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_fillcolorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_filltype_r (PLOTTER (&DEVICE (f)), 0);
    DRAW_MOOD (f) = A68_TRUE;
    DEVICE_OPENED (&DEVICE (f)) = A68_TRUE;
    X_COORD (&DEVICE (f)) = 0;
    Y_COORD (&DEVICE (f)) = 0;
    return PLOTTER (&DEVICE (f));
#endif
  } else if (!strcmp (device_type, "ps")) {
#if defined (POSTSCRIPT_MISSING)
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_INVALID_PARAMETER, "postscript plotter missing", "");
    exit_genie (p, A68_RUNTIME_ERROR);
#else
//
// Supported plotter type - Postscript
//
// Open the output file for drawing.
    CHECK_REF (p, ref_filename, M_ROWS);
    filename = DEREF (char, &ref_filename);
    errno = 0;
    if ((STREAM (&DEVICE (f)) = fopen (filename, "w")) == NO_STREAM) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_CANNOT_OPEN_NAME, filename);
      exit_genie (p, A68_RUNTIME_ERROR);
    } else {
      READ_MOOD (f) = A68_FALSE;
      WRITE_MOOD (f) = A68_FALSE;
      CHAR_MOOD (f) = A68_FALSE;
      DRAW_MOOD (f) = A68_TRUE;
    }
// Set up ps plotter.
    PLOTTER_PARAMS (&DEVICE (f)) = pl_newplparams ();
    if (PLOTTER_PARAMS (&DEVICE (f)) == NULL) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_ALLOCATE);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    (void) pl_setplparam (PLOTTER_PARAMS (&DEVICE (f)), "PAGESIZE", DEREF (char, &A68_PAGE_SIZE (&DEVICE (f))));
    PLOTTER (&DEVICE (f)) = pl_newpl_r ("ps", NULL, STREAM (&DEVICE (f)), stderr, PLOTTER_PARAMS (&DEVICE (f)));
    if (PLOTTER (&DEVICE (f)) == NULL) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    if (pl_openpl_r (PLOTTER (&DEVICE (f))) < 0) {
      diagnostic (A68_RUNTIME_ERROR, p, ERROR_DEVICE_CANNOT_OPEN);
      exit_genie (p, A68_RUNTIME_ERROR);
    }
    WINDOW_X_SIZE (&DEVICE (f)) = 1000;
    WINDOW_Y_SIZE (&DEVICE (f)) = 1000;
    (void) pl_space_r (PLOTTER (&DEVICE (f)), 0, 0, WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f)));
    (void) pl_bgcolorname_r (PLOTTER (&DEVICE (f)), "black");
    (void) pl_colorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_pencolorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_fillcolorname_r (PLOTTER (&DEVICE (f)), "white");
    (void) pl_filltype_r (PLOTTER (&DEVICE (f)), 0);
    DRAW_MOOD (f) = A68_TRUE;
    DEVICE_OPENED (&DEVICE (f)) = A68_TRUE;
    X_COORD (&DEVICE (f)) = 0;
    Y_COORD (&DEVICE (f)) = 0;
    return PLOTTER (&DEVICE (f));
#endif
  } else {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_INVALID_PARAMETER, "unindentified plotter", device_type);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  return NULL;
}

//! @brief PROC (REF FILE) VOID draw erase

void genie_draw_clear (NODE_T * p)
{
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_flushpl_r (plotter);
  (void) pl_erase_r (plotter);
}

//! @brief PROC (REF FILE) VOID draw show

void genie_draw_show (NODE_T * p)
{
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_flushpl_r (plotter);
}

//! @brief PROC (REF FILE) REAL draw aspect

void genie_draw_aspect (NODE_T * p)
{
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  PUSH_VALUE (p, (REAL_T) WINDOW_Y_SIZE (&DEVICE (f)) / (REAL_T) WINDOW_X_SIZE (&DEVICE (f)), A68_REAL);
  (void) plotter;
}

//! @brief PROC (REF FILE, INT) VOID draw fillstyle

void genie_draw_fillstyle (NODE_T * p)
{
  A68_INT z;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &z, A68_INT);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_filltype_r (plotter, (int) VALUE (&z));
}

//! @brief PROC (INT) STRING draw get colour name

void genie_draw_get_colour_name (NODE_T * p)
{
  A68_INT z;
  int j;
  char *str;
  POP_OBJECT (p, &z, A68_INT);
  j = (VALUE (&z) - 1) % COLOUR_NAMES;
  str = NAME (&A68_COLOURS[j]);
  PUSH_REF (p, c_to_a_string (p, str, DEFAULT_WIDTH));
}

//! @brief PROC (REF FILE, REAL, REAL, REAL) VOID draw colour

void genie_draw_colour (NODE_T * p)
{
  A68_REAL x, y, z;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &z, A68_REAL);
  POP_OBJECT (p, &y, A68_REAL);
  POP_OBJECT (p, &x, A68_REAL);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  RED (&DEVICE (f)) = VALUE (&x);
  GREEN (&DEVICE (f)) = VALUE (&y);
  BLUE (&DEVICE (f)) = VALUE (&z);
  (void) pl_color_r (plotter, (int) (VALUE (&x) * COLOUR_MAX), (int) (VALUE (&y) * COLOUR_MAX), (int) (VALUE (&z) * COLOUR_MAX));
  (void) pl_pencolor_r (plotter, (int) (VALUE (&x) * COLOUR_MAX), (int) (VALUE (&y) * COLOUR_MAX), (int) (VALUE (&z) * COLOUR_MAX));
  (void) pl_fillcolor_r (plotter, (int) (VALUE (&x) * COLOUR_MAX), (int) (VALUE (&y) * COLOUR_MAX), (int) (VALUE (&z) * COLOUR_MAX));
}

//! @brief PROC (REF FILE, REAL, REAL, REAL) VOID draw background colour

void genie_draw_background_colour (NODE_T * p)
{
  A68_REAL x, y, z;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &z, A68_REAL);
  POP_OBJECT (p, &y, A68_REAL);
  POP_OBJECT (p, &x, A68_REAL);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_bgcolor_r (plotter, (int) (VALUE (&x) * COLOUR_MAX), (int) (VALUE (&y) * COLOUR_MAX), (int) (VALUE (&z) * COLOUR_MAX));
}

//! @brief PROC (REF FILE, STRING) VOID draw colour name

void genie_draw_colour_name (NODE_T * p)
{
  A68_REF ref_c, ref_file;
  A68_FILE *f;
  A68_REF name_ref;
  char *name;
  int iindex;
  REAL_T x, y, z;
  plPlotter *plotter;
  POP_REF (p, &ref_c);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  name_ref = heap_generator (p, M_C_STRING, 1 + a68_string_size (p, ref_c));
  name = DEREF (char, &name_ref);
  ASSERT (a_to_c_string (p, name, ref_c) != NO_TEXT);
  if (!string_to_colour (p, name, &iindex)) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_INVALID_PARAMETER, "unidentified colour name", name);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  x = (REAL_T) (RED (&A68_COLOURS[iindex])) / (REAL_T) (0xff);
  y = (REAL_T) (GREEN (&A68_COLOURS[iindex])) / (REAL_T) (0xff);
  z = (REAL_T) (BLUE (&A68_COLOURS[iindex])) / (REAL_T) (0xff);
  plotter = set_up_device (p, f);
  RED (&DEVICE (f)) = x;
  GREEN (&DEVICE (f)) = y;
  BLUE (&DEVICE (f)) = z;
  (void) pl_color_r (plotter, (int) (x * COLOUR_MAX), (int) (y * COLOUR_MAX), (int) (z * COLOUR_MAX));
  (void) pl_pencolor_r (plotter, (int) (x * COLOUR_MAX), (int) (y * COLOUR_MAX), (int) (z * COLOUR_MAX));
  (void) pl_fillcolor_r (plotter, (int) (x * COLOUR_MAX), (int) (y * COLOUR_MAX), (int) (z * COLOUR_MAX));
}

//! @brief PROC (REF FILE, STRING) VOID draw background colour name

void genie_draw_background_colour_name (NODE_T * p)
{
  A68_REF ref_c, ref_file;
  A68_FILE *f;
  A68_REF name_ref;
  char *name;
  int iindex;
  REAL_T x, y, z;
  plPlotter *plotter;
  POP_REF (p, &ref_c);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  name_ref = heap_generator (p, M_C_STRING, 1 + a68_string_size (p, ref_c));
  name = DEREF (char, &name_ref);
  ASSERT (a_to_c_string (p, name, ref_c) != NO_TEXT);
  if (!string_to_colour (p, name, &iindex)) {
    diagnostic (A68_RUNTIME_ERROR, p, ERROR_INVALID_PARAMETER, "unidentified colour name", name);
    exit_genie (p, A68_RUNTIME_ERROR);
  }
  x = (REAL_T) (RED (&A68_COLOURS[iindex])) / (REAL_T) (0xff);
  y = (REAL_T) (GREEN (&A68_COLOURS[iindex])) / (REAL_T) (0xff);
  z = (REAL_T) (BLUE (&A68_COLOURS[iindex])) / (REAL_T) (0xff);
  plotter = set_up_device (p, f);
  RED (&DEVICE (f)) = x;
  GREEN (&DEVICE (f)) = y;
  BLUE (&DEVICE (f)) = z;
  (void) pl_bgcolor_r (plotter, (int) (x * COLOUR_MAX), (int) (y * COLOUR_MAX), (int) (z * COLOUR_MAX));
}

//! @brief PROC (REF FILE, STRING) VOID draw linestyle

void genie_draw_linestyle (NODE_T * p)
{
  A68_REF txt, ref_file;
  A68_FILE *f;
  int size;
  A68_REF z_ref;
  char *z;
  plPlotter *plotter;
  POP_REF (p, &txt);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  size = a68_string_size (p, txt);
  z_ref = heap_generator (p, M_C_STRING, 1 + size);
  z = DEREF (char, &z_ref);
  ASSERT (a_to_c_string (p, z, txt) != NO_TEXT);
  (void) pl_linemod_r (plotter, z);
}

//! @brief PROC (REF FILE, INT) VOID draw linewidth

void genie_draw_linewidth (NODE_T * p)
{
  A68_REAL width;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &width, A68_REAL);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_linewidth_r (plotter, (int) (VALUE (&width) * (REAL_T) WINDOW_Y_SIZE (&DEVICE (f))));
}

//! @brief PROC (REF FILE, REAL, REAL) VOID draw move

void genie_draw_move (NODE_T * p)
{
  A68_REAL x, y;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &y, A68_REAL);
  POP_OBJECT (p, &x, A68_REAL);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_fmove_r (plotter, VALUE (&x) * WINDOW_X_SIZE (&DEVICE (f)), VALUE (&y) * WINDOW_Y_SIZE (&DEVICE (f)));
  X_COORD (&DEVICE (f)) = VALUE (&x);
  Y_COORD (&DEVICE (f)) = VALUE (&y);
}

//! @brief PROC (REF FILE, REAL, REAL) VOID draw line

void genie_draw_line (NODE_T * p)
{
  A68_REAL x, y;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &y, A68_REAL);
  POP_OBJECT (p, &x, A68_REAL);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_fline_r (plotter, X_COORD (&DEVICE (f)) * WINDOW_X_SIZE (&DEVICE (f)), Y_COORD (&DEVICE (f)) * WINDOW_Y_SIZE (&DEVICE (f)), VALUE (&x) * WINDOW_X_SIZE (&DEVICE (f)), VALUE (&y) * WINDOW_Y_SIZE (&DEVICE (f)));
  X_COORD (&DEVICE (f)) = VALUE (&x);
  Y_COORD (&DEVICE (f)) = VALUE (&y);
}

//! @brief PROC (REF FILE, REAL, REAL) VOID draw point

void genie_draw_point (NODE_T * p)
{
  A68_REAL x, y;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &y, A68_REAL);
  POP_OBJECT (p, &x, A68_REAL);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_fpoint_r (plotter, VALUE (&x) * WINDOW_X_SIZE (&DEVICE (f)), VALUE (&y) * WINDOW_Y_SIZE (&DEVICE (f)));
  X_COORD (&DEVICE (f)) = VALUE (&x);
  Y_COORD (&DEVICE (f)) = VALUE (&y);
}

//! @brief PROC (REF FILE, REAL, REAL) VOID draw rect

void genie_draw_rect (NODE_T * p)
{
  A68_REAL x, y;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &y, A68_REAL);
  POP_OBJECT (p, &x, A68_REAL);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_fbox_r (plotter, X_COORD (&DEVICE (f)) * WINDOW_X_SIZE (&DEVICE (f)), Y_COORD (&DEVICE (f)) * WINDOW_Y_SIZE (&DEVICE (f)), VALUE (&x) * WINDOW_X_SIZE (&DEVICE (f)), VALUE (&y) * WINDOW_Y_SIZE (&DEVICE (f)));
  X_COORD (&DEVICE (f)) = VALUE (&x);
  Y_COORD (&DEVICE (f)) = VALUE (&y);
}

//! @brief PROC (REF FILE, REAL, REAL, REAL) VOID draw circle

void genie_draw_circle (NODE_T * p)
{
  A68_REAL x, y, r;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &r, A68_REAL);
  POP_OBJECT (p, &y, A68_REAL);
  POP_OBJECT (p, &x, A68_REAL);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_fcircle_r (plotter, VALUE (&x) * WINDOW_X_SIZE (&DEVICE (f)), VALUE (&y) * WINDOW_Y_SIZE (&DEVICE (f)), VALUE (&r) * MAX (WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f))));
  X_COORD (&DEVICE (f)) = VALUE (&x);
  Y_COORD (&DEVICE (f)) = VALUE (&y);
}

//! @brief PROC (REF FILE, REAL, REAL, REAL) VOID draw atom

void genie_draw_atom (NODE_T * p)
{
  A68_REAL x, y, r;
  REAL_T frac;
  int j, k;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &r, A68_REAL);
  POP_OBJECT (p, &y, A68_REAL);
  POP_OBJECT (p, &x, A68_REAL);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  k = (int) (VALUE (&r) * MAX (WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f))));
  (void) pl_filltype_r (plotter, 1);
  for (j = k - 1; j >= 0; j--) {
    frac = (REAL_T) j / (REAL_T) (k - 1);
    frac = 0.6 + 0.3 * sqrt (1.0 - frac * frac);
    (void) pl_color_r (plotter, (int) (frac * RED (&DEVICE (f)) * COLOUR_MAX), (int) (frac * GREEN (&DEVICE (f)) * COLOUR_MAX), (int) (frac * BLUE (&DEVICE (f)) * COLOUR_MAX));
    (void) pl_fcircle_r (plotter, VALUE (&x) * WINDOW_X_SIZE (&DEVICE (f)), VALUE (&y) * WINDOW_Y_SIZE (&DEVICE (f)), (REAL_T) j);
  }
  (void) pl_filltype_r (plotter, 0);
  X_COORD (&DEVICE (f)) = VALUE (&x);
  Y_COORD (&DEVICE (f)) = VALUE (&y);
}

//! @brief PROC (REF FILE, REAL, REAL, REAL) VOID draw atom

void genie_draw_star (NODE_T * p)
{
  A68_REAL x, y, r;
  REAL_T z, frac;
  int j, k;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &r, A68_REAL);
  POP_OBJECT (p, &y, A68_REAL);
  POP_OBJECT (p, &x, A68_REAL);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  k = (int) (VALUE (&r) * MAX (WINDOW_X_SIZE (&DEVICE (f)), WINDOW_Y_SIZE (&DEVICE (f))));
  for (j = k; j >= 0; j--) {
    z = (REAL_T) j / (REAL_T) k;
    if (z < 0.2) {
      z = z / 0.2;
      frac = 0.5 * (1 + (cos (CONST_PI / 2 * z)));
    } else {
      z = (z - 0.2) / 0.8;
      frac = (1 - z) * 0.3;
    }
    (void) pl_color_r (plotter, (int) (frac * RED (&DEVICE (f)) * COLOUR_MAX), (int) (frac * GREEN (&DEVICE (f)) * COLOUR_MAX), (int) (frac * BLUE (&DEVICE (f)) * COLOUR_MAX));
    (void) pl_fcircle_r (plotter, VALUE (&x) * WINDOW_X_SIZE (&DEVICE (f)), VALUE (&y) * WINDOW_Y_SIZE (&DEVICE (f)), (REAL_T) j);
  }
  (void) pl_color_r (plotter, (int) (RED (&DEVICE (f)) * COLOUR_MAX), (int) (GREEN (&DEVICE (f)) * COLOUR_MAX), (int) (BLUE (&DEVICE (f)) * COLOUR_MAX));
  X_COORD (&DEVICE (f)) = VALUE (&x);
  Y_COORD (&DEVICE (f)) = VALUE (&y);
}

//! @brief PROC (REF FILE, CHAR, CHAR, STRING) VOID draw text

void genie_draw_text (NODE_T * p)
{
  A68_CHAR just_v, just_h;
  A68_REF txt, ref_file;
  A68_FILE *f;
  int size;
  A68_REF z_ref;
  char *z;
  plPlotter *plotter;
  POP_REF (p, &txt);
  POP_OBJECT (p, &just_v, A68_CHAR);
  POP_OBJECT (p, &just_h, A68_CHAR);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  size = a68_string_size (p, txt);
  z_ref = heap_generator (p, M_C_STRING, 1 + size);
  z = DEREF (char, &z_ref);
  ASSERT (a_to_c_string (p, z, txt) != NO_TEXT);
  size = pl_alabel_r (plotter, VALUE (&just_h), VALUE (&just_v), z);
}

//! @brief PROC (REF FILE, STRING) VOID draw fontname

void genie_draw_fontname (NODE_T * p)
{
  A68_REF txt, ref_file;
  A68_FILE *f;
  int size;
  A68_REF z_ref;
  char *z;
  plPlotter *plotter;
  POP_REF (p, &txt);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  size = a68_string_size (p, txt);
  z_ref = heap_generator (p, M_C_STRING, 1 + size);
  z = DEREF (char, &z_ref);
  ASSERT (a_to_c_string (p, z, txt) != NO_TEXT);
  (void) pl_fontname_r (plotter, z);
}

//! @brief PROC (REF FILE, INT) VOID draw fontsize

void genie_draw_fontsize (NODE_T * p)
{
  A68_INT size;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &size, A68_INT);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_fontsize_r (plotter, (int) VALUE (&size));
}

//! @brief PROC (REF FILE, INT) VOID draw textangle

void genie_draw_textangle (NODE_T * p)
{
  A68_INT angle;
  A68_REF ref_file;
  A68_FILE *f;
  plPlotter *plotter;
  POP_OBJECT (p, &angle, A68_INT);
  POP_REF (p, &ref_file);
  CHECK_REF (p, ref_file, M_REF_FILE);
  f = FILE_DEREF (&ref_file);
  plotter = set_up_device (p, f);
  (void) pl_textangle_r (plotter, (int) VALUE (&angle));
}

#endif
