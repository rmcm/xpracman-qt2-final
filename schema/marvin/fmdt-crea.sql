--  -*- sql -*-
--
--    $Id: fmdt-crea.sql 1287 2002-04-29 23:52:23Z source $

--  Copyright 2001 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 

 
-- Revision Log

drop table fmdt;
drop sequence fmdt_fmdt__sequence_seq;
create table fmdt                                               -- Letterheads
(
 fmdt_form_code text not null,                                  -- Form Code 
 fmdt_desc text default text 'New Item',                        -- description
 fmdt_field_type integer default 0 not null,                    -- 0=Label, 1=Field,
                                                                -- 2=line, 3=Calc, 4=Special
 fmdt_section integer default 1,                                -- 0=RH,1=PH,2=D,3=PF,4=RF
 fmdt_text text,                                                -- literal/attribute_name
 fmdt_x_coord integer default 0 not null,                       -- X Co-ordinate
 fmdt_y_coord integer default 0 not null,                       -- Y Co-ordinate
 fmdt_width integer default 0 not null,                         -- also used for Line
 fmdt_height integer default 12 not null,                       -- also used for Line
 fmdt_bg_colour text default text '255,255,255' not null,       -- background "NNN,NNN,NNN" (R,G,B)
 fmdt_fg_colour text default text '0,0,0' not null,             -- foreground "NNN,NNN,NNN" (R,G,B)
 fmdt_bd_colour text default text '0,0,0' not null,             -- border "NNN,NNN,NNN" (R,G,B) 
 fmdt_bd_width integer default 1 not null,                      -- also used for Line
 fmdt_bd_style integer default 0 not null,                      -- also used for Line
 fmdt_ft_family text default text 'Helvetica' not null,
 fmdt_ft_size integer default 8 not null,
 fmdt_ft_weight integer default 25 not null,
 fmdt_ft_Italic integer default 0 not null,
 fmdt_hz_align integer default 0 not null,
 fmdt_vt_align integer default 2 not null,
 fmdt_word_wrap integer default 0 not null,
 fmdt_data_type integer default 0 not null,
 fmdt_date_format integer default 7 not null,
 fmdt_precision integer default 0 not null,
 fmdt_currency integer default 36 not null,
 fmdt_neg_colour text default text '255,0,0' not null,          -- "NNN,NNN,NNN" (R,G,B)
 fmdt_comma_sep integer default 0 not null,
 --
 -- standard headers
 --
 fmdt__sequence serial not null,                                -- Row-ID
 fmdt__timestamp timestamp default text 'now' not null,         -- Timestamp
 fmdt__user_entry text default CURRENT_USER not null,           -- Entered by
 fmdt__status char default text 'A' not null,                   -- Record status
 --
 -- constraints
 --
 primary key(fmdt__sequence)
);

grant select,insert,update on fmdt to public;

