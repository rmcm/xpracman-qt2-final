--     $Id: mtal-crea.sql 2849 2006-05-22 01:56:19Z source $
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


drop table mtal;
drop sequence mtal_mtal__sequence_seq;
create table mtal                       -- Alerts
(
 mtal_mtat_name text,                                           -- attribute name
 mtal_mtop_code  text,                                          -- comparison operator
 mtal_value     text,                                           -- comparison value
 mtal_colour    text,                                           -- display colour
 mtal_priority int default 0,                                   -- order of evaluation (lowest = highest priority)
 --
 -- standard headers
 --
 mtal__sequence serial not null,                                -- Row-ID
 mtal__timestamp timestamp default now() not null,              -- Timestamp
 mtal__user_entry text default CURRENT_USER not null,           -- Entered by
 mtal__status char default text 'N' not null,                   -- Record status
 --
 -- constraints
 --
 primary key(mtal__sequence),
 constraint mtal_fk_mtop
            foreign key(mtal_mtop_code) references mtop
            on update cascade
            on delete restrict
            initially deferred,
 constraint mtal_fk_mtat
            foreign key(mtal_mtat_name) references mtat
            on update cascade
            on delete restrict
            initially deferred
 );

grant select,insert,update on mtal to public;
grant select,insert,update on mtal_mtal__sequence_seq to public;

delete from mtcl where mtcl_name = 'mtal';
select create_metadata('mtal');

delete from mtfn where mtfn_master_class = 'mtal' or mtfn_other_class = 'mtal';
select create_fk_links('mtal');
update mtfn set mtfn_view = 'mtat_title' where mtfn_master_class = 'mtal' and mtfn_other_class = 'mtat';

delete from mtrl where mtrl_master_class = 'mtal' or mtrl_other_class = 'mtal';
select create_rl_links('mtal');

update mtcl set mtcl_title = 'Alerts' where mtcl_name = 'mtal';
update mtcl set mtcl_query_limit = 0 where mtcl_name = 'mtal';

update mtat set mtat_displen = 10 where mtat_name = 'mtal_mtat_name';
update mtat set mtat_title = 'Attribute' where mtat_name = 'mtal_mtat_name';
update mtat set mtat_title = 'Operator' where mtat_name = 'mtal_mtop_code';
update mtat set mtat_title = 'Value' where mtat_name = 'mtal_value';
update mtat set mtat_title = 'Colour' where mtat_name = 'mtal_colour';
update mtat set mtat_title = 'Priority' where mtat_name = 'mtal_priority';
update mtat set mtat_input_method = 'COLOUR' where mtat_name = 'mtal_colour';
update mtat set mtat_input_method = 'FSL=100' where mtat_name = 'mtal_mtop_code';

-- testing
-- insert into mtal(mtal_mtat_name,mtal_mtop_code,mtal_value,mtal_colour, mtal_priority) values('patn_dob',4, '21/11/1999','#00FFFF', 1);
-- insert into mtal(mtal_mtat_name,mtal_mtop_code,mtal_value,mtal_colour, mtal_priority) values('patn_dob',4, '21/11/1989','#00EEEE', 2);
-- insert into mtal(mtal_mtat_name,mtal_mtop_code,mtal_value,mtal_colour, mtal_priority) values('patn_dob',4, '21/11/1979','#00DDDD', 3);
-- insert into mtal(mtal_mtat_name,mtal_mtop_code,mtal_value,mtal_colour, mtal_priority) values('patn_dob',4, '21/11/1969','#00CCCC', 4);
-- insert into mtal(mtal_mtat_name,mtal_mtop_code,mtal_value,mtal_colour, mtal_priority) values('patn_dob',4, '21/11/1959','#00BBBB', 5);
-- 


-- update history;
\set mttb_name '\'mtal\''
\set mttb_rcs_header '\'$Id: mtal-crea.sql 2849 2006-05-22 01:56:19Z source $\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
