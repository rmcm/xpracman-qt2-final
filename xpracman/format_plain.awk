#!/usr/bin/awk -f
#
#     $Id: format_plain.awk 902 2001-10-14 09:00:30Z source $
# 
#   Copyright 2001 X=X Computer Software Trust
#    		  Kangaroo Ground Australia 3097
# 
# 
#  This is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License published by
#  the Free Software Foundation; either version 2, or (at your option)
#  any later version.
#  
#  This software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this software; see the file COPYING.  If not, write to
#  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
#  
#  Report problems and direct all questions to:
#  
#      Rex McMaster, rmcm@compsoft.com.au
#  
# 
## PCL code functions for invoice/statement layout.
##

function initformat() {
# The standard layout
    left_margin = 0;      # mm
    top_margin  = 65;     # mm
    bottom_margin = 60;   # mm
    page_length = 60;     # lines
# Section flags
    sv_flag = 0;
# Initialise printer code
    init_pr = "";
## init_pr = "E&a%dL&k2G&l0o7.27c66F(s12h4T"
# Some LJ font codes
    font_tm_8 = "";
    font_tm_10 = "";
    font_tm_10b = "";
    font_fx_10 = "";
    font_fx_10b = "";
# Local letterhead - overlay
    letterhead = ENVIRON["PRAC_LH"];
    if (letterhead != "" )
    {
        while (getline < letterhead > 0) {
            print $0;
        }
    }

}
########################################
# Cursor to right (mm from current position)
########################################
function rcurpos(x) {
  hcurpos_code = "";
  return(hcurpos_code);
}
########################################
# Cursor to left (mm from current position)
########################################
function lcurpos(x) {
  hcurpos_code = "";
  return(hcurpos_code);
}
########################################
# Horizontal Cursor Position (mm from left)
########################################
function hcurpos(x) {
  hcurpos_code = "";
  return(hcurpos_code);
}
########################################
# Cursor down (mm from current position)
########################################
function dcurpos(y) {
  vcurpos_code = "";
  return(vcurpos_code);
}
########################################
# Cursor up (mm from current position)
########################################
function ucurpos(y) {
  vcurpos_code = "";
  return(vcurpos_code);
}
########################################
# Vertical Cursor Position (mm from top)
########################################
function vcurpos(y) {
  vcurpos_code = "";
  return(vcurpos_code);
}
########################################
# Absolute Cursor Position (x,y mm)
########################################
function acurpos(x, y) {
  acurpos_code = "";
  return(acurpos_code);
}
########################################
# Rectangle (width,height,pattern,fill)
########################################
function rectangle(w, h, p, f) {
  rectangle_code = "";
  return(rectangle_code);
}




