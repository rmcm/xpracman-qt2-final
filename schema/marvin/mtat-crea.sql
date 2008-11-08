--  -*- sql -*-
--
 
--    $Id: mtat-crea.sql 1287 2002-04-29 23:52:23Z source $

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
  

drop table mtat;
drop sequence mtat_mtat__sequence_seq;
create table mtat                       -- ATTRIBUTE meta data
(
 mtat_access             text default text '31' not null,    -- access list to match phplib access
 mtat_name               text default text 'NAME' not null,  -- name of attribute
 mtat_type               text default text 'text' not null,  -- data type
 mtat_length             integer default 1 not null,         -- attribute length (for char())
 mtat_attributes         text,                               -- eg default or not null
 mtat_operator           text default text '=' not null,     -- an operator to apply as default in QBE
                                                                  -- (otherwise a default exists for data
                                                                  -- type - defined in sqlquery.inc??)
 mtat_title              text default text 'TITLE' not null, -- descriptive title for attribute
 mtat_class_name         text default text '-' not null,     -- name of parent class
 mtat_displen            integer default 10 not null,        -- display length
 mtat_browse_order       integer default 500 not null,       -- order of attribute in browse/view row
 mtat_view_order         integer default 500 not null,       -- order of attribute in browse/view column
 mtat_input_method       text,           -- input method
                                                 -- RO                = Read-only
                                                 -- integer/blank     = TEXT
                                                 -- FSL=NN            = Foreign select list 
                                                                        -- NN = max list size
                                                 -- SELECT=value,label;value,label;value,label;..
                                                 -- =foreign_attribute
 mtat_default            text,           -- defaut value for new row
                                                 -- text string
                                                 -- =mtat_name (eg attribute in parent record)
                                                 -- ~javascript_function
 mtat_htmltagatt         text,           -- extra fields to include in <input > tag - eg onclick=

--
 -- standard headers
 --
 mtat__sequence serial not null,                            -- Row-ID
 mtat__timestamp timestamp default text 'now' not null,     -- Timestamp
 mtat__user_entry text default CURRENT_USER not null,       -- Entered by
 mtat__status char default text 'A' not null,               -- Record status (Active)
 --
 -- constraints
 --
 primary key (mtat_name)
 );


grant select,insert,update on mtat to public;
-- this has to be granted for TABLE_insert_row() to function
grant select,insert,update on mtat_mtat__sequence_seq to public;
