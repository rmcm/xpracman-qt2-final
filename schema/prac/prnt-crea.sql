 --  -*- sql -*-
--
--    $Id$

--  Copyright 1996 X=X Computer Software Trust
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
 

 

create table prnt
(
 prnt_type               integer,              -- Type system/internal
 prnt_access             integer,            -- Access
 prnt_name               text,                 -- Printer Name
 prnt_desc               text,                 -- Description
 prnt_command            text,                -- Printer command
 -- standard headers
 prnt__sequence          serial not null,                       -- Row-ID
 prnt__timestamp         timestamp default now() not null,  -- Timestamp
 prnt__user_entry        text default CURRENT_USER not null,    -- Entered by
 prnt__status            char default text 'N' not null,        -- Record status
 -- constraints
 primary key(prnt_name)
 );
 
 grant select,insert,update on prnt to public;
 grant select,insert,update on prnt_prnt__sequence_seq to public;


select create_metadata('prnt');

-- update history;
insert into mttb(mttb_name, mttb_rcs_header) values('prnt','$Id:');
