--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
-- 
-- 
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--  
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--  
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--  
--  Report problems and direct all questions to:
--  
--      Rex McMaster, rmcm@compsoft.com.au
--  
-- 



create table mbst                       -- MBS Table of Fees
(
 mbst_item integer, 
 mbst_desc text,
 mbst_sch100 numeric(12,2),
 mbst_sch75 numeric(12,2),
 mbst_sch85 numeric(12,2),
 --
 -- standard headers
 --
 mbst__sequence serial not null,                                -- Row-ID
 mbst__timestamp timestamp default now() not null,              -- Timestamp
 mbst__user_entry text default CURRENT_USER not null,           -- Entered by
 mbst__status char default text 'N' not null,                   -- Record status
 --
 -- constraints
 --
 primary key(mbst__sequence)
 );

grant select,insert,update on mbst to public;
grant select,insert,update on mbst_mbst__sequence_seq to public;

select create_metadata('mbst');
update mtcl set mtcl_title = 'MBS Table of Fees' where mtcl_name = 'mbst';

-- update history;
\set mttb_name '\'mbst\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

