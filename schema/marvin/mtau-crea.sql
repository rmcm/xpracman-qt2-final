--     $Id: mtau-crea.sql 2427 2005-02-05 22:59:06Z source $
-- 
--   Copyright 2004 X=X Computer Software Trust
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



drop table mtau;
drop sequence mtau_mtau__sequence_seq;
create table mtau                       -- Audit Log
(
 mtau_table_name text not null,                                 -- table name
 mtau_row_sequence integer not null,                            -- serial number of row.
 mtau_operation text not null,                                  -- operation type I, U, S, D
 mtau_attributes text,                                          -- attributes effected
 mtau_before text,                                              -- values before
 mtau_after text,                                               -- values after
 --
 -- standard headers
 --
 mtau__sequence serial not null,                                -- Row-ID
 mtau__timestamp timestamp default now() not null,              -- Timestamp
 mtau__user_entry text default CURRENT_USER not null,           -- Entered by
 mtau__status char default text 'N' not null,                   -- Record status
 --
 -- constraints
 --
 primary key(mtau__sequence)
 );

grant select,insert on mtau to public;
grant select,insert,update on mtau_mtau__sequence_seq to public;

select create_metadata('mtau');
update mtcl set mtcl_title = 'Audit Log' where mtcl_name = 'mtau';

-- update history;
\set mttb_name '\'mtau\''
\set mttb_rcs_header '\'$Id: mtau-crea.sql 2427 2005-02-05 22:59:06Z source $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

