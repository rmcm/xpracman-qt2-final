--  -*- sql -*-
--
--    $Id: form-crea.sql 884 2001-10-08 01:42:32Z source $

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

drop table form;
drop sequence form_form__sequence_seq;
create table form                                               -- Forms
(
 form_code text not null,                                       -- Form Code 
 form_desc text default text 'New Report',                      -- description
 form_classname text,                                           -- associated DB table/view
 form_page_attribute text,                                      -- page break on this attribute
 form_includes text,                                            -- include-forms (eg letterhead)
                                                                --   - a space-separated list
 form_pagesize integer default 0 not null,
 form_orientation integer default 0 not null,
 form_mg_top integer default 10 not null,
 form_mg_bottom integer default 10 not null,
 form_mg_left integer default 40 not null,
 form_mg_right integer default 40 not null,
 form_rh_height integer default 0 not null,
 form_rh_frequency integer default 0 not null,
 form_rf_height integer default 0 not null,
 form_rf_frequency integer default 0 not null,
 form_ph_height integer default 45 not null,
 form_ph_frequency integer default 1 not null,
 form_pf_height integer default 35 not null,
 form_pf_frequency integer default 1 not null,
 form_dt_height integer default 30 not null,
 --
 -- standard headers
 --
 form__sequence serial not null,                              -- Row-ID
 form__timestamp timestamp default text 'now' not null,       -- Timestamp
 form__user_entry text default CURRENT_USER not null,         -- Entered by
 form__status char default text 'A' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(form_code)
);

grant select,insert,update on form to public;

