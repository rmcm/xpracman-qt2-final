--     $Id: mtvs-crea.sql 2427 2005-02-05 22:59:06Z source $
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



drop table mtvs;
drop sequence mtvs_mtvs__sequence_seq;
create table mtvs                       -- metadata version - schema
(
 mtvs_schema_name text,                 -- schema name
 mtvs_schema_version text,              -- schema version
 mtvs_product_name text,                -- product name (eg XPracMan)
 mtvs_product_version text,             -- product version
 mtvs_desc text,                        -- Description
 --
 -- standard headers
 --
 mtvs__sequence serial not null,                                -- Row-ID
 mtvs__timestamp timestamp default now() not null,              -- Timestamp
 mtvs__user_entry text default CURRENT_USER not null,           -- Entered by
 mtvs__status char(1) default text '-' not null,                -- Record status
 --
 -- constraints
 --
 primary key(mtvs__sequence)
 );

grant select,insert,update on mtvs to public;
grant select,insert,update on mtvs_mtvs__sequence_seq to public;

select create_metadata('mtvs');
update mtcl set mtcl_title = 'Metadata Version History' where mtcl_name = 'mtvs';

-- update history;
\set mttb_name '\'mtvs\''
\set mttb_rcs_header '\'$Id: mtvs-crea.sql 2427 2005-02-05 22:59:06Z source $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

