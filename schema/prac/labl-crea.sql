 --  -*- sql -*-
--
--    $Id$

--  Copyright 2000 X=X Computer Software Trust
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
 

-- Specifications for print objects, which can be accessed by id. The object
-- has a coordinate set and a text label. This object can be incorporated into
-- larger print objects.

create table labl
(
 labl_id                 text,                 -- id of label set (eg INVOICE or HISTORY-1)
 labl_subtype            text,                 -- subtype (eg feet_code)
 labl_text_label         text,                 -- text for label item
 labl_class_name         text,                 -- class
 labl_attribute_name     text,                 -- attribute name
 labl_row                integer,              -- row
 labl_column             integer,              -- column
 -- standard headers
 labl__sequence          serial not null,                       -- Row-ID
 labl__timestamp         timestamp default now() not null,  -- Timestamp
 labl__user_entry        text default CURRENT_USER not null,    -- Entered by
 labl__status            char default text 'N' not null,        -- Record status
 -- constraints
 primary key(labl__sequence)
 );
 
 grant select,insert,update on labl to public;
 grant select,insert,update on labl_labl__sequence_seq to public;


select create_metadata('labl');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('labl','$Id:');
