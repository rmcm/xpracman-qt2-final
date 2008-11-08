--     $Id: mtop-crea.sql 2104 2004-02-10 22:12:46Z source $
-- 
--   Copyright 2003 X=X Computer Software Trust
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

drop table mtop;
drop sequence mtop_mtop__sequence_seq;
create table mtop                       -- Operators
(
 mtop_code     text,                                           -- comparison value
 mtop_desc     text,                                           -- attribute descriptor
 --
 -- standard headers
 --
 mtop__sequence serial not null,                              -- Row-ID
 mtop__timestamp timestamp default text 'now'::timestamp not null,       -- Timestamp
 mtop__user_entry text default CURRENT_USER not null,         -- Entered by
 mtop__status char default text 'N' not null,                 -- Record status
 --
 -- constraints
 --
 primary key(mtop_code)
 );

grant select on mtop to public;
grant select on mtop_mtop__sequence_seq to public;

delete from mtcl where mtcl_name = 'mtop';
select create_metadata('mtop');
update mtcl set mtcl_title = 'Comparison Operators' where mtcl_name = 'mtop';
update mtcl set mtcl_query_limit = 0 where mtcl_name = 'mtop';
update mtcl set mtcl_userlabel = 'mtop_desc' where mtcl_name = 'mtop';
update mtfn set mtfn_view = 'mtop_desc' where mtfn_other_class = 'mtop';
update mtat set mtat_title = 'Code' where mtat_name = 'mtop_code';
update mtat set mtat_title = 'Description' where mtat_name = 'mtop_desc';

-- Values NOTE: this must match the QmvAttribute::operator_types;
 insert into mtop(mtop_code, mtop_desc) values(0,'Matches');
 insert into mtop(mtop_code, mtop_desc) values(1,'Like');
 insert into mtop(mtop_code, mtop_desc) values(2,'Contains');
 insert into mtop(mtop_code, mtop_desc) values(3,'Equals');
 insert into mtop(mtop_code, mtop_desc) values(4,'GreaterThan');
 insert into mtop(mtop_code, mtop_desc) values(5,'LessThan');
 insert into mtop(mtop_code, mtop_desc) values(6,'GreaterThanOrEqual');
 insert into mtop(mtop_code, mtop_desc) values(7,'LessThanOrEqual');
 insert into mtop(mtop_code, mtop_desc) values(8,'NotEqual');
 insert into mtop(mtop_code, mtop_desc) values(9,'All');

-- update history;
\set mttb_name '\'mtop\''
\set mttb_rcs_header '\'$Id: mtop-crea.sql 2104 2004-02-10 22:12:46Z source $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
